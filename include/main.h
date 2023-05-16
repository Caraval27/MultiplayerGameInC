#pragma once

#include "../include/libraries.h"
#include "../include/world.h"
#include "../include/files.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"
#include "../include/text.h"
#include "../include/network.h"

typedef struct {
    GameDisplay gameDisplay;
    State state;
    Background* pBackground;
    Buttons buttons;
    DisplayText displayText;

    PlayersData *pPlayersData;

    Platform *pPlatforms[100], *pStartPlatform;

    Language language;

    Music music;

    LobbyConnect *pLobbyConnect;
	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand pClientCommands[COMMAND_BUFFER];
} Game;

int initiateGame(Game* pGame);

void runGame(Game* pGame);

void quitGame(Game* pGame);