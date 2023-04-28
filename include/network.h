#pragma once

#include "../include/libraries.h"
#include "../include/players.h"

#define PORT 3000
#define PACKET_SIZE 512
#define CLIENT_LIMIT 6
#define SERVER_IP "127.0.0.1"

typedef struct {
	bool isHost;
	UDPsocket pSocket;
	UDPpacket *pPacket;
	IPaddress clients[CLIENT_LIMIT];
	IPaddress serverIP;
} NetworkData;

typedef struct {
	Player **players;
	// ...
} GameplayData;

typedef enum {
	MOVEMENT,
	JUMP,
	LEAVE
} CommandType;

// \param direction MOVEMENT: True if pointing right, False if pointing left.
// \param mode MOVEMENT: True if the movement is to be started, False if it is to be stopped.
typedef struct {
	CommandType commandType;
	bool direction;
	bool mode;
} ClientCommand;

// Prepare the application to communicate over a network.
// \param isHost True if the user is hosting a game, otherwise false.
// \return Return 1 upon success. Return 0 otherwise.
int initializeNetcode(NetworkData *pNetworkData, bool isHost);

// Gets the IP-address of any incoming packets and, if unique, adds it to the list of clients.
// The function is non-interrupting and should be ran repeatedly.
void listenForNewClients(NetworkData *pNetworkData);

// Package the current gameplay state and broadcast it to all connected clients.
// \param *pGameplayData Struct containing the gameplay state to be sent.
void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData);

// Gets any incoming packet and, if from a valid client, integrate its command into the game.
void handleClientCommands(NetworkData *pNetworkData, ClientCommand *pClientCommand, GameplayData *pGameplayData);

// Sends empty packet to the host to become acknowledged as a client.
//  \return Returns 1 if packet was sent successfully. Returns 0 otherwise.
int joinHost(NetworkData *pNetworkData);

// Check for any packets from the server and copy the packet data to the local GameplayData.
// \return Return 1 if a packet was received from the server. Return 0 otherwise.
// This may be used to check if the server has acknowledged the client.
int receiveHostBroadcast(NetworkData *pNetworkData, GameplayData *pGameplayData);

// Copy the ClientCommand object to a packet and send it to the server.
// \param *pClientCommand Object containing the command.
void sendClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommand);
