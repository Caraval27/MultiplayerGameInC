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
    SDL_Texture *pMenuTexture, *pBackgroundTexture, *pPlatformTexture, *pStartPlatformTexture;
    TTF_Font *pMenuFont;
    Background* pBackground;
    Buttons buttons;
    bool chosenLanguage;
    Text *pYouAreDeadText, *pWhoWonText, *pWaitingText, *pInputIPText, *pEnterIPText, *pNrClientsText;

    Player* pPlayers[MAX_PLAYERS];
    SDL_Texture* pPlayerTextures[MAX_PLAYERS];
    int nrOfPlayers, nrOfPlayersLeft;
    Platform *pPlatforms[100], *pStartPlatform;

    char inputIP[INPUT_IP_LEN];
    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
    Mix_Chunk *pJumpSound, *pWinSound;
    Mix_Music *pMainSound;

	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand pClientCommands[COMMAND_BUFFER];
} Game;

int initiateGame(Game* pGame);
void initiateLanguage(FILE* fp, Game* pGame, Buttons* pButtons);
int handleError(Game* pGame, void* pMember, const char* (*GetError)(void));

void runGame(Game* pGame);

void handleMainMenu(Game* pGame, SDL_Event event, bool* pMute);
void renderMainMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Texture* pMenutexture);
void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum, bool *pShowLang);
void renderSettingsMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Texture* pMenutexture);
void handleLanguageMenu(Game* pGame, SDL_Event event, bool* pShowLang);
void renderLanguageMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void readKeybindString(Game *pGame, int input, GameDisplay* pGameDisplay, Buttons* pButtons);
void handleEnterInput(Game* pGame, SDL_Event event, int* pNum);
void handleLobbyMenu(Game* pGame, SDL_Event event, bool* pLeft, bool* pRight, int* pTime, int* pIndex, bool* pJoined);
void renderLobbyMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Texture* pMenutexture);
void handleLobby(Game* pGame, SDL_Event event, int* pIndex, bool* pJoined);
void fillZero(char inputIP[], int max);
void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool *pLeft, bool *pRight, int* pTime, bool* pMute);
void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pLeft, bool* pRight, bool* pMute);
void handleGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute);
void renderGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons);
void handleGameOver(Game* pGame, SDL_Event event);
void renderGameOver(GameDisplay* pGameDisplay, Buttons* pButtons, Text* pWhoWonText);

void resetGame(Game* pGame, bool* pLeft, bool* pRight, int* pTime);

void quitGame(Game* pGame);