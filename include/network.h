#pragma once

#include "../include/libraries.h"
#include "../include/players.h"

#define PORT 3000
#define PACKET_SIZE 512
#define CLIENT_LIMIT 6

typedef struct {
	UDPsocket pSocket;
	UDPpacket *pPacket;
	IPaddress clients[CLIENT_LIMIT];
} NetworkData;

typedef struct {
	Player **players;
	// ...
} GameplayData;

// Prepare the application to communicate over a network.
// \return Success = 1, Failure = 0
int initializeNetcode(NetworkData *pNetworkData, GameplayData *pGameplayData);

// Gets the IP-address of any incoming packets and, if unique, adds it to the list of clients.
// The function is non-interrupting and should be ran repeatedly.
void listenForNewClients(NetworkData *pNetworkData);

// Package the current gameplay state and broadcast it to all connected clients.
// \param *pGameplayData Struct containing the gameplay state to be sent.
void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData);
