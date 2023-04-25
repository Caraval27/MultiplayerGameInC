#include "../include/network.h"

int initializeNetcode(NetworkData *pNetworkData, GameplayData *pGameplayData, bool isHost) {
	pNetworkData = malloc(sizeof(NetworkData));
	pGameplayData = malloc(sizeof(GameplayData));
	pNetworkData->isHost = isHost;
	pNetworkData->pSocket = SDLNet_UDP_Open(isHost ? PORT : 0);
	pNetworkData->pPacket = SDLNet_AllocPacket(PACKET_SIZE);
	if (!pNetworkData->pSocket || !pNetworkData->pPacket) {
		printf("%s\n", SDLNet_GetError());
		return 0;
	}
	if (!isHost) {
		if (SDLNet_ResolveHost(&pNetworkData->serverIP, SERVER_IP, PORT) == -1) {
			printf("%s\n", SDLNet_GetError());
			return 0;
		}
	}
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

void handleClientCommands(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		// check if command is legal
		// modify pGameplayData
	}
}

void joinHost(NetworkData *pNetworkData) {
	if (!pNetworkData->serverIP.host) {}
	pNetworkData->pPacket->address.host = pNetworkData->serverIP.host;
	pNetworkData->pPacket->address.port = pNetworkData->serverIP.port;
	SDLNet_UDP_Send(pNetworkData->pSocket, -1, pNetworkData->pPacket);
}

int receiveHostBroadcast(NetworkData *pNetworkData, GameplayData *pGameplayData) {
	while (SDLNet_UDP_Recv(pNetworkData->pSocket, pNetworkData->pPacket)) {
		if (pNetworkData->pPacket->address.host != pNetworkData->serverIP.host
			|| pNetworkData->pPacket->address.port != pNetworkData->serverIP.port) return 0;
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
