#include "../include/network.h"

int initializeNetcode(NetworkData *pNetworkData, bool isHost) {
	pNetworkData->isHost = isHost;
	if (!(pNetworkData->pPacket = SDLNet_AllocPacket(PACKET_SIZE))) {
		printf("error: failed to allocate packet: %s\n", SDLNet_GetError());
		return 0;
	}
	if (!(pNetworkData->pSocket = SDLNet_UDP_Open(isHost ? PORT : 0))) {
		printf("error: failed to open socket: %s\n", SDLNet_GetError());
		return 0;
	}
	if (!isHost) {
		if (SDLNet_ResolveHost(&pNetworkData->server, SERVER_IP, PORT) == -1) {
			printf("error: failed to resolve host: %s\n", SDLNet_GetError());
			return 0;
		}
	}
	printf("netcode initialization complete. isHost = %d\n", isHost);
	return 1;
}

void runNetcode(NetworkData *pNetworkData, GameplayData *pGameplayData, ClientCommand *pClientCommand) {
	if (SDL_GetTicks64() % 1000 < 17) {
		if (pNetworkData->isHost) {
			listenForNewClients(pNetworkData);
			broadcastToClients(pNetworkData, pGameplayData);
		} else {
			if (pNetworkData->hasJoined) {
				listenForHostBroadcast(pNetworkData, pGameplayData);
			} else {
				sendClientCommand(pNetworkData, pClientCommand);
				if (listenForHostBroadcast(pNetworkData, pGameplayData)) {
					pNetworkData->hasJoined = true;
					printf("acknowledgement received\n");
				}
			}
		}
	}
}

void sendClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommand) {
	memcpy(pNetworkData->pPacket->data, pClientCommand, sizeof(ClientCommand));
	pNetworkData->pPacket->len = sizeof(ClientCommand);
	pNetworkData->pPacket->address = pNetworkData->server;
	if (SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket)) {
		printf("packet sent\n");
	} else {
		printf("failed to send packet: %s\n", SDLNet_GetError());
	}
}

int listenForNewClients(NetworkData *pNetworkData) {
	int newClients = 0;
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		bool unique = true;
		Uint32 packetHost = pNetworkData->pPacket->address.host;
		Uint16 packetPort = pNetworkData->pPacket->address.port;
		printf("incoming packet ip host: %d\n", packetHost);
		printf("incoming packet ip port: %d\n", packetPort);
		int nClients = 0;
		while (pNetworkData->clients[nClients].host) {
			if (packetHost == pNetworkData->clients[nClients].host
				&& packetPort == pNetworkData->clients[nClients].port) {
				unique = false;
				break;
			}
			nClients++;
		}
		if (unique) {
			pNetworkData->clients[nClients] = pNetworkData->pPacket->address;
			newClients++;
			printf("client added to list\n");
			printf("new total clients: %d\n", nClients + 1);
		} else {
			printf("join request rejected: duplicate ip\n");
		}
	}
	return newClients;
}

void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	for (int i = 0; pNetworkData->clients[i].host; i++) {
		memcpy(pNetworkData->pPacket->data, pGameplayData, sizeof(GameplayData));
		pNetworkData->pPacket->len = sizeof(GameplayData);
		pNetworkData->pPacket->address = pNetworkData->clients[i];
		if (SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket)) {
			printf("broadcast sent to client at index %d\n", i);
		} else {
			printf("failed to send broadcast\n");
		}
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

void handleClientCommands(NetworkData *pNetworkData, ClientCommand *pClientCommand, GameplayData *pGameplayData) {
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		int validClient = 0;
		{
			int i = 0;
			while (!validClient && pNetworkData->clients[i].host) {
				if (pNetworkData->clients[i].host != pNetworkData->pPacket->address.host
					&& pNetworkData->clients[i].port != pNetworkData->pPacket->address.port) {
					validClient = 1;
				} else {
					i++;
				}
			}
		}
		if (validClient) {
			memcpy(pClientCommand, pNetworkData->pPacket->data, sizeof(ClientCommand));
			// if legal command, modify pGameplayData
			// perhaps call another function
		}
	}
}
