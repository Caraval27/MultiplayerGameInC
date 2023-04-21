#include "../include/network.h"

int initializeNetcode(NetworkData *pNetworkData, GameplayData *pGameplayData, bool isHost) {
	pNetworkData = malloc(sizeof(NetworkData));
	pGameplayData = malloc(sizeof(GameplayData));
	pNetworkData->isHost = isHost;
	pNetworkData->pSocket = SDLNet_UDP_Open(PORT);
	pNetworkData->pPacket = SDLNet_AllocPacket(PACKET_SIZE);
	if (!pNetworkData->pSocket || !pNetworkData->pPacket) return 0;
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
		memcpy(pNetworkData->pPacket->data, pGameplayData, sizeof(pGameplayData));
		pNetworkData->pPacket->len = sizeof(pGameplayData);
		pNetworkData->pPacket->address = pNetworkData->clients[i];
		SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket);
	}
}

void handleClientCommands(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		// check if command is legal
		// modify pGameplayData
	}
}
