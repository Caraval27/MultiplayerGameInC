#include "../include/network.h"

int initializeNetcode(NetworkData *pNetworkData, bool isHost) {
	pNetworkData->isHost = isHost;
	pNetworkData->pSocket = SDLNet_UDP_Open(isHost ? PORT : 0);
	pNetworkData->pPacket = SDLNet_AllocPacket(PACKET_SIZE);
	if (!pNetworkData->pSocket || !pNetworkData->pPacket) {
		printf("error: %s\n", SDLNet_GetError());
		return 0;
	}
	if (!isHost) {
		if (SDLNet_ResolveHost(&pNetworkData->serverIP, SERVER_IP, PORT) == -1) {
			printf("error: %s\n", SDLNet_GetError());
			return 0;
		}
	}
	printf("netcode initialization complete. isHost = %d\n", isHost);
	return 1;
}

void listenForNewClients(NetworkData *pNetworkData) {
	Uint32 inboundHost = pNetworkData->pPacket->address.host;
	Uint16 inboundPort = pNetworkData->pPacket->address.port;
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		int nofClients = 0;
		while (!pNetworkData->clients[nofClients].host) nofClients++;
		for (int i = 0; i < nofClients; i++) {
			if (inboundHost == pNetworkData->clients[nofClients].host
				&& inboundPort == pNetworkData->clients[nofClients].port) return;
		}
		pNetworkData->clients[nofClients].host = inboundHost;
		pNetworkData->clients[nofClients].port = inboundPort;
	}
}

void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	int nofClients = 0;
	while (!pNetworkData->clients[nofClients].host) nofClients++;
	for (int i; i < nofClients; i++) {
		memcpy(pNetworkData->pPacket->data, pGameplayData, sizeof(GameplayData));
		pNetworkData->pPacket->len = sizeof(GameplayData);
		pNetworkData->pPacket->address = pNetworkData->clients[i];
		SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket);
	}
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

int joinHost(NetworkData *pNetworkData) {
	if (!pNetworkData->serverIP.host) {
		printf("error: unable to send packet due to unresolved serverIP\n");
		return 0;
	}
	pNetworkData->pPacket->address.host = pNetworkData->serverIP.host;
	pNetworkData->pPacket->address.port = pNetworkData->serverIP.port;
	if (!SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket)) {
		printf("error: failed to send packet: %s\n", SDLNet_GetError());
		return 0;
	}
	printf("join request sent! waiting for reply...\n");
	return 1;
}

int receiveHostBroadcast(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		if (pNetworkData->pPacket->address.host != pNetworkData->serverIP.host
			|| pNetworkData->pPacket->address.port != pNetworkData->serverIP.port) continue;
		memcpy(pGameplayData, pNetworkData->pPacket->data, sizeof(GameplayData));
		return 1;
	}
	return 0;
}

void sendClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommand) {
	memcpy(pNetworkData->pPacket->data, pClientCommand, sizeof(ClientCommand));
	pNetworkData->pPacket->len = sizeof(ClientCommand);
	pNetworkData->pPacket->address = pNetworkData->serverIP;
	SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket);
}
