#pragma once

#include "../include/libraries.h"
#include "../include/players.h"

#define SERVER_IP "127.0.0.1"
#define PORT 3016
#define PACKET_SIZE 256
#define CLIENT_LIMIT MAX_PLAYERS - 1
#define COMMAND_BUFFER CLIENT_LIMIT
#define NETCODE_TICKRATE 1000
#define TIMEOUT_TICKRATE 1000
#define CLIENT_TIMEOUT 3000

typedef enum {
	MOVEMENT,
	LEAVE,
} CommandType;

// \param direction MOVEMENT: 1 if pointing right, -1 if pointing left, 0 if motionless
typedef struct {
	IPaddress ip;
	CommandType commandType;
	int direction;
} ClientCommand;

typedef struct {
	IPaddress ip;
	Uint64 lastSeen;
} Client;

typedef struct {
	UDPsocket pSocket;
	UDPpacket *pPacket;
	bool isHost;
	int hasJoined;
	Client clients[CLIENT_LIMIT];
	int nClients;
	IPaddress server;
	Player **pPlayers;
} NetworkData;

typedef struct {
	Player players[CLIENT_LIMIT];
	State gameState;
	// int nrOfPlayers, nrOfPlayersLeft;
} GameplayData;

// Prepare the application to communicate over a network.
// \param isHost True if the user is hosting a game, otherwise false.
// \return Return 1 upon success. Return 0 otherwise.
int initializeNetcode(NetworkData *pNetworkData, bool isHost);

// \param pGameplayData If host, this struct must be filled with data from the current state
// of the game BEFORE calling the function. If client, this struct will be filled with data from the
// host AFTER calling the function.
// \param pClientCommands If host, this pointer must reference an array, which will be filled with
// client commands AFTER calling the function. If client, the pointer must reference a struct
// containing a client command BEFORE calling the function.
void runNetcode(NetworkData *pNetworkData, GameplayData *pGameplayData, ClientCommand *pClientCommands);

// Package the current gameplay state and broadcast it to all tracked clients.
// \param pGameplayData Struct containing the gameplay state to be sent.
void broadcastToClients(NetworkData *pNetworkData, GameplayData *pGameplayData);

// Get any incoming packets and take appropriate action for each packet.
// \param pClientCommands If the packet contains a client command to be retrieved,
// store it in this array.
void listenForClientCommands(NetworkData *pNetworkData, ClientCommand *pClientCommands);

// Check if the IP of the current packet exists in the list of clients.
// \return Return true if there was a match. Return false otherwise.
int checkExistingClient(NetworkData *pNetworkData);

// Store the client command of the current packet.
// \param pClientCommands Store the client commands in this array.
void retrieveClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommands, int cIndex);

// Add a new client with the IP address of the current packet.
void addClient(NetworkData *pNetworkData);

// Iterate through client array and remove any clients that were last seen beyond a threshold.
void timeoutClients(NetworkData *pNetworkData);

// Remove a client at a given index.
void removeClient(NetworkData *pNetworkData, int index);

// Send a packet containing a client command to the server.
// \param pClientCommand Pointer to the client command.
void sendClientCommand(NetworkData *pNetworkData, ClientCommand *pClientCommand);

// Get any incoming packets from the server and store their gameplay data.
// \return The number of broadcasts received upon a singular call.
int listenForHostBroadcast(NetworkData *pNetworkData, GameplayData *pGameplayData);
