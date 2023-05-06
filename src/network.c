#include "../include/network.h"

int initializeNetcode(NetworkData *pNetworkData, bool isHost) {
	pNetworkData->isHost = isHost;
	if (!(pNetworkData->pPacket = SDLNet_AllocPacket(PACKET_SIZE))) {
		printf("failed to allocate packet: %s\n", SDLNet_GetError());
		return 0;
	}
	if (!(pNetworkData->pSocket = SDLNet_UDP_Open(isHost ? PORT : 0))) {
		printf("failed to open socket: %s\n", SDLNet_GetError());
		return 0;
	}
	if (!isHost) {
		pNetworkData->hasJoined = false;
		if (SDLNet_ResolveHost(&pNetworkData->server, SERVER_IP, PORT) == -1) {
			printf("failed to resolve host: %s\n", SDLNet_GetError());
			return 0;
		}
	}
	printf("netcode initialization complete. isHost = %d\n", isHost);
	return 1;
}

void runNetcode(NetworkData *pNetworkData, GameplayData *pGameplayData, ClientCommand *pClientCommands) {
	if (SDL_GetTicks64() % NETCODE_TICKRATE < 17) {
		if (pNetworkData->isHost) {
			// SERVER
			broadcastToClients(pNetworkData, pGameplayData);
			listenForClientCommands(pNetworkData, pClientCommands);

			// call on an interval of TIMEOUT_TICKRATE instead
			timeoutClients(pNetworkData);
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
		printf("----------\n");
	}
}

void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	memcpy(pNetworkData->pPacket->data, pGameplayData, sizeof(GameplayData));
	pNetworkData->pPacket->len = sizeof(GameplayData);
	int nBroadcasts = 0;
	for (int i = 0; pNetworkData->clients[i].ip.host && i < CLIENT_LIMIT; i++) {
		pNetworkData->pPacket->address = pNetworkData->clients[i].ip;
		if (SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket)) {
			nBroadcasts++;
		} else {
			printf("failed to send broadcast to client at index %d\n", i);
		}
	}
	if (nBroadcasts > 0) {
		printf("broadcast sent to %d client(s)\n", nBroadcasts);
	}
}

void listenForClientCommands(NetworkData *pNetworkData, ClientCommand *pClientCommands) {
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		int cIndex = checkExistingClient(pNetworkData);
		// if (checkExistingClient(pNetworkData)) {
		if (cIndex > -1) {
			retrieveClientCommand(pNetworkData, pClientCommands, cIndex);
		} else {
			addClient(pNetworkData);
		}
	}
}

int checkExistingClient(NetworkData *pNetworkData) {
	Uint32 packetHost = pNetworkData->pPacket->address.host;
	Uint16 packetPort = pNetworkData->pPacket->address.port;
	// printf("incoming packet ip host: %d\n", packetHost);
	// printf("incoming packet ip port: %d\n", packetPort);
	for (int i = 0; pNetworkData->clients[i].ip.host && i < CLIENT_LIMIT; i++) {
		if (packetHost == pNetworkData->clients[i].ip.host
			&& packetPort == pNetworkData->clients[i].ip.port) {
			// return true;
			return i;
		}
	}
	// return false;
	return -1;
}

void retrieveClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommands, int cIndex) {
	Uint32 packetHost = pNetworkData->pPacket->address.host;
	Uint16 packetPort = pNetworkData->pPacket->address.port;
	int iCommands = 0;
	while (pClientCommands[iCommands].ip.host && iCommands < COMMAND_BUFFER) {
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
	// printf("client command retrieved at buffer index %d\n", iCommands);
	// int iClients = 0;
	int iClients = cIndex;
	// while (packetHost != pNetworkData->clients[iClients].ip.host
	// 	|| packetPort != pNetworkData->clients[iClients].ip.port) iClients++;
	pNetworkData->clients[iClients].lastSeen = SDL_GetTicks64();
	// printf("set \"lastSeen\" for client at index %d to %d\n",
	// 	iClients,
	// 	(int)pNetworkData->clients[iClients].lastSeen);
}

void addClient(NetworkData *pNetworkData) {
	int nClients = pNetworkData->nClients;
	if (nClients >= CLIENT_LIMIT) {
		printf("client addition rejected: client limit reached\n");
		return;
	}
	pNetworkData->clients[nClients].ip = pNetworkData->pPacket->address;
	pNetworkData->clients[nClients].lastSeen = SDL_GetTicks64();
	{
		int i = 1;
		while (pNetworkData->pPlayers[i]->ip.host) i++;
		pNetworkData->pPlayers[i]->ip = pNetworkData->clients[nClients].ip;
	}
	pNetworkData->nClients++;
	printf("client added to list. lastSeen = %d\n", (int)pNetworkData->clients[nClients].lastSeen);
	printf("new total clients: %d\n", pNetworkData->nClients);
}

void timeoutClients(NetworkData *pNetworkData) {
	Uint64 currentTime = SDL_GetTicks64();
	for (int i = 0; pNetworkData->clients[i].ip.host && i < CLIENT_LIMIT; i++) {
		if (currentTime - pNetworkData->clients[i].lastSeen > CLIENT_TIMEOUT) {
			removeClient(pNetworkData, i);
			printf("client timed out at index %d\n", i);
			i = 0;
		}
	}
}

void removeClient(NetworkData *pNetworkData, int index) {
	{
		int i = 1;
		while (pNetworkData->pPlayers[i]->ip.host != pNetworkData->clients[index].ip.host
				|| pNetworkData->pPlayers[i]->ip.port != pNetworkData->clients[index].ip.port) i++;
		pNetworkData->pPlayers[i]->ip = (IPaddress){0};
	}
	for (int i = index; pNetworkData->clients[i].ip.host && i < CLIENT_LIMIT; i++) {
		if (i == CLIENT_LIMIT - 1) {
			pNetworkData->clients[i] = (Client){0};
		} else {
			pNetworkData->clients[i] = pNetworkData->clients[i + 1];
		}
	}
	pNetworkData->nClients--;
}

void sendClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommand) {
	memcpy(pNetworkData->pPacket->data, pClientCommand, sizeof(ClientCommand));
	pNetworkData->pPacket->len = sizeof(ClientCommand);
	pNetworkData->pPacket->address = pNetworkData->server;
	if (SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket)) {
		printf("command sent\n");
	} else {
		printf("failed to send command: %s\n", SDLNet_GetError());
	}
}

int listenForHostBroadcast(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	int nBroadcasts = 0;
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		if (pNetworkData->pPacket->address.host != pNetworkData->server.host
			|| pNetworkData->pPacket->address.port != pNetworkData->server.port) continue;
		memcpy(pGameplayData, pNetworkData->pPacket->data, sizeof(GameplayData));
		nBroadcasts++;
		printf("broadcast received\n");
	}
	return nBroadcasts;
}
