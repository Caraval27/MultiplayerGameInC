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
		printf("--------------------\n");
	}
}

void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	int nBroadcasts = 0;
	for (int i = 0; pNetworkData->clients[i].ip.host && i < CLIENT_LIMIT; i++) {
		memcpy(pNetworkData->pPacket->data, pGameplayData, sizeof(GameplayData));
		pNetworkData->pPacket->len = sizeof(GameplayData);
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
		if (checkExistingClient(pNetworkData)) {
			retrieveClientCommand(pNetworkData, pClientCommands);
		} else {
			addClient(pNetworkData);
		}
	}
}

bool checkExistingClient(NetworkData *pNetworkData) {
	Uint32 packetHost = pNetworkData->pPacket->address.host;
	Uint16 packetPort = pNetworkData->pPacket->address.port;
	printf("incoming packet ip host: %d\n", packetHost);
	printf("incoming packet ip port: %d\n", packetPort);
	int nClients = 0;
	while (pNetworkData->clients[nClients].ip.host && nClients < CLIENT_LIMIT) {
		if (packetHost == pNetworkData->clients[nClients].ip.host
			&& packetPort == pNetworkData->clients[nClients].ip.port) {
			return true;
		}
		nClients++;
	}
	return false;
}

void retrieveClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommands) {
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
	printf("client command retrieved at buffer index %d\n", iCommands);
	int iClients = 0;
	while (packetHost != pNetworkData->clients[iClients].ip.host
		|| packetPort != pNetworkData->clients[iClients].ip.port) iClients++;
	pNetworkData->clients[iClients].lastSeen = SDL_GetTicks64();
	printf("set \"lastSeen\" for client at index %d to %d\n",
		iClients,
		(int)pNetworkData->clients[iClients].lastSeen);
}

void addClient(NetworkData *pNetworkData) {
	int nClients = 0;
	while (pNetworkData->clients[nClients].ip.host && nClients < CLIENT_LIMIT) nClients++;
	if (nClients < CLIENT_LIMIT) {
		pNetworkData->clients[nClients].ip = pNetworkData->pPacket->address;
		pNetworkData->clients[nClients].lastSeen = SDL_GetTicks64();
		printf("client added to list. lastSeen = %d\n", (int)pNetworkData->clients[nClients].lastSeen);
		printf("new total clients: %d\n", nClients + 1);
	} else {
		printf("client addition rejected: client limit reached\n");
	}
}

void removeClient(NetworkData *pNetworkData, int index) {
	for (int i = index; pNetworkData->clients[i].ip.host && i < CLIENT_LIMIT; i++) {
		if (i == CLIENT_LIMIT - 1) {
			pNetworkData->clients[i] = (Client){0};
		} else {
			pNetworkData->clients[i] = pNetworkData->clients[i + 1];
		}
	}
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
