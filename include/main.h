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

    /* Player* pPlayers[MAX_PLAYERS];
    SDL_Texture* pPlayerTextures[MAX_PLAYERS];
    int nrOfPlayers, nrOfPlayersLeft; */

    PlayersData *pPlayersData;

    Platform *pPlatforms[100], *pStartPlatform;

    Language language;

    Mix_Chunk *pJumpSound, *pWinSound;
    Mix_Music *pMainSound;

    LobbyConnect *pLobbyConnect;
	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand pClientCommands[COMMAND_BUFFER];
} Game;

int initiateGame(Game* pGame);
void initiateLanguage(FILE* fp, Language* pLanguage, GameDisplay* pGameDisplay, Buttons* pButtons, DisplayText* pDisplayText);
int handleError(Game* pGame, void* pMember, const char* (*GetError)(void));

void runGame(Game* pGame);

void handleMainMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute);
void renderMainMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void handleSettingsMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState);
void renderSettingsMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void handleLanguageMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event);
void renderLanguageMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void readKeybindString(Language* pLanguage, int index, GameDisplay* pGameDisplay, Buttons* pButtons);
void handleEnterInput(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState);
void handleLobbyMenu(Game* pGame, GameDisplay* pGameDisplay, NetworkData* pNetworkData, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect, int* pTime);
void renderLobbyMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void handleLobby(GameDisplay* pGameDisplay, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect);
void fillZero(char inputIP[], int max);
void handleOngoing(Game* pGame, PlayersData* pPlayersData, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, Language* pLanguage, SDL_Event event, State* pState, bool* pIsRunning, int *pTime, bool* pMute);
void handleOngoingInput(PlayersData* pPlayersData, NetworkData* pNetworkData, ClientCommand* pClientCommands, Language* pLanguage, SDL_Event* event, State* pState, bool* pIsRunning, bool* pMute);
void handleGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute);
void renderGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void handleGameOver(PlayersData* pPlayersData, GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButton, DisplayText* pDisplayText, SDL_Event event, State* pState);
void renderGameOver(GameDisplay* pGameDisplay, Buttons* pButtons, Text* pWhoWonText);

void resetGame(Game* pGame, PlayersData *pPlayersData, int* pTime);

void quitGame(Game* pGame);