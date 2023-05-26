#include "../include/network.h"

int initializeNetcode(NetworkData *pNetworkData) {
	if (!(pNetworkData->pPacket = SDLNet_AllocPacket(PACKET_SIZE))) {
		printf("failed to allocate packet: %s\n", SDLNet_GetError());
		return 0;
	}

	if (!(pNetworkData->pPacketTemp = SDLNet_AllocPacket(PACKET_SIZE))) {
		printf("failed to allocate packet: %s\n", SDLNet_GetError());
		return 0;
	}

	if ((pNetworkData->pSocketServer = SDLNet_UDP_Open(PORT_SERVER))) {
		printf("server socket opened\n");
	} else {
		printf("failed to open server socket: %s\n", SDLNet_GetError());
	}

	if ((pNetworkData->pSocketClient = SDLNet_UDP_Open(PORT_CLIENT))) {
		printf("client socket opened\n");
	} else {
		printf("failed to open client socket: %s\n", SDLNet_GetError());
	}

	pNetworkData->hasJoined = false; // support leaving game

	printf("netcode initialization complete\n");
	return 1;
}

void setConnection(NetworkData *pNetworkData, char *host) {
	if (host == NULL) {
		printf("connection set as server\n");
		pNetworkData->isHost = true;
	} else {
		printf("connection set as client\n");
		pNetworkData->isHost = false;
		resolveNewHost(pNetworkData, host);
	}
}

int resolveNewHost(NetworkData *pNetworkData, char *host) {
	if (!SDLNet_ResolveHost(&pNetworkData->server, host, PORT_SERVER)) {
		printf("host resolved\n");
	} else {
		printf("failed to resolve host: %s\n", SDLNet_GetError());
		return 0;
	}
	return 1;
}

void runNetcode(NetworkData *pNetworkData, GameplayData *pGameplayData, ClientCommand *pClientCommands) {
	Uint64 tickDiff = SDL_GetTicks64() - pNetworkData->lastTicks;
	pNetworkData->relativeTicks += tickDiff;
	pNetworkData->lastTicks = SDL_GetTicks64();

	if (pNetworkData->relativeTicks < NETCODE_TICKRATE) return;

	if (pNetworkData->isHost) {
		// SERVER
		broadcastToClients(pNetworkData, pGameplayData);
		listenForClientCommands(pNetworkData, pClientCommands);
	} else {
		// CLIENT
		sendClientCommand(pNetworkData, &pClientCommands[0]);
		if (pNetworkData->hasJoined) {
			listenForHostBroadcast(pNetworkData, pGameplayData);
		} else {
			if (listenForHostBroadcast(pNetworkData, pGameplayData)) {
				pNetworkData->hasJoined = true;
				printf("server acknowledgement received\n");
			}
		}
	}
	printf("-\n");

	pNetworkData->relativeTicks = 0;
}

void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	memcpy(pNetworkData->pPacket->data, pGameplayData, sizeof(GameplayData));
	pNetworkData->pPacket->len = sizeof(GameplayData);
	int nBroadcasts = 0;
	int indexClient = 0;
	while (indexClient < pNetworkData->nClients) {
		pNetworkData->pPacket->address = pNetworkData->clients[indexClient].ip;
		if (SDLNet_UDP_Send(pNetworkData->pSocketServer, -1, pNetworkData->pPacket)) {
			nBroadcasts++;
		} else {
			printf("failed to send broadcast to client at index %d\n", indexClient);
		}
		if (checkClientTimeout(pNetworkData, indexClient)) indexClient++;
	}
	if (nBroadcasts > 0) {
		printf("brdcst %d\n", nBroadcasts);
	}
}

int checkClientTimeout(NetworkData *pNetworkData, int index) {
	if (SDL_GetTicks64() - pNetworkData->clients[index].lastSeen < CLIENT_TIMEOUT) return 1;

	int nPlayers = 1;
	while (pNetworkData->pPlayers[nPlayers]->ip.host != pNetworkData->clients[index].ip.host
			|| pNetworkData->pPlayers[nPlayers]->ip.port != pNetworkData->clients[index].ip.port) nPlayers++;
	pNetworkData->pPlayers[nPlayers]->ip = (IPaddress){0};

	for (int i = index; i < pNetworkData->nClients; i++) {
		if (i == pNetworkData->nClients - 1) {
			pNetworkData->clients[i] = (Client){0};
		} else {
			pNetworkData->clients[i] = pNetworkData->clients[i + 1];
		}
	}

	pNetworkData->nClients--;
	printf("client timed out at index %d\n", index);
	return 0;
}

void listenForClientCommands(NetworkData *pNetworkData, ClientCommand *pClientCommands) {
	int packets = 0;
	while (SDLNet_UDP_Recv(pNetworkData->pSocketServer, pNetworkData->pPacket)) {
		packets++;
		int indexClient = checkExistingClient(pNetworkData);
		if (indexClient > -1) {
			retrieveClientCommand(pNetworkData, pClientCommands, indexClient);
		} else {
			addClient(pNetworkData);
		}
	}
	printf("pckts %d\n", packets);
}

int checkExistingClient(NetworkData *pNetworkData) {
	Uint32 packetHost = pNetworkData->pPacket->address.host;
	Uint16 packetPort = pNetworkData->pPacket->address.port;
	for (int i = 0; pNetworkData->clients[i].ip.host && i < CLIENT_LIMIT; i++) {
		if (packetHost == pNetworkData->clients[i].ip.host
			&& packetPort == pNetworkData->clients[i].ip.port) {
			return i;
		}
	}
	return -1;
}

void retrieveClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommands, int indexClient) {
	int packets = 0;
	Uint32 packetHost = pNetworkData->pPacket->address.host;
	Uint16 packetPort = pNetworkData->pPacket->address.port;
	int iCommands = 0;
	while (pClientCommands[iCommands].ip.host
			&& iCommands < COMMAND_BUFFER
			&& packets < COMMAND_BUFFER * 2) {
		packets++;
		if (packetHost == pClientCommands[iCommands].ip.host
			&& packetPort == pClientCommands[iCommands].ip.port) {
			break;
		}
		iCommands++;
	}
	if (iCommands >= COMMAND_BUFFER) {
		printf("client command rejected: buffer overflow\n");
		return;
	}
	memcpy(&pClientCommands[iCommands], pNetworkData->pPacket->data, sizeof(ClientCommand));
	pClientCommands[iCommands].ip = pNetworkData->pPacket->address;
	printf("cmd indx %d\n", iCommands);
	pNetworkData->clients[indexClient].lastSeen = SDL_GetTicks64();
}

void addClient(NetworkData *pNetworkData) {
	int nClients = pNetworkData->nClients;
	if (nClients >= CLIENT_LIMIT) {
		printf("client addition rejected: client limit reached\n");
		return;
	}
	pNetworkData->clients[nClients].ip = pNetworkData->pPacket->address;
	pNetworkData->clients[nClients].lastSeen = SDL_GetTicks64();

	int nPlayers = 1;
	while (pNetworkData->pPlayers[nPlayers]->ip.host && nPlayers < MAX_PLAYERS) nPlayers++;
	pNetworkData->pPlayers[nPlayers]->ip = pNetworkData->clients[nClients].ip;

	pNetworkData->nClients++;
	printf("client added to list. lastSeen = %d\n", (int)pNetworkData->clients[nClients].lastSeen);
	printf("new total clients: %d\n", pNetworkData->nClients);
}

void sendClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommand) {
	memcpy(pNetworkData->pPacket->data, pClientCommand, sizeof(ClientCommand));
	pNetworkData->pPacket->len = sizeof(ClientCommand);
	pNetworkData->pPacket->address = pNetworkData->server;
	if (SDLNet_UDP_Send(pNetworkData->pSocketClient, -1, pNetworkData->pPacket)) {
		printf("cmd\n");
	} else {
		printf("failed to send command: %s\n", SDLNet_GetError());
	}
}

int listenForHostBroadcast(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	int nBroadcasts = 0;
	int nPackets = 0;
	while (SDLNet_UDP_Recv(pNetworkData->pSocketClient, pNetworkData->pPacket)) {
		nPackets++;

		if (pNetworkData->pPacket->address.host == pNetworkData->server.host
			&& pNetworkData->pPacket->address.port == pNetworkData->server.port) {

			memcpy(pGameplayData, pNetworkData->pPacket->data, sizeof(GameplayData));
			nBroadcasts++;

		}
	}
	// memcpy(pGameplayData, pNetworkData->pPacket->data, sizeof(GameplayData));
	printf("pckts %d\n", nPackets);
	printf("csts %d\n", nBroadcasts);
	return nBroadcasts;
}
