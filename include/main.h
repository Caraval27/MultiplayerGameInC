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
    //SDL_Window *pWindow;
    //int windowWidth, windowHeight;
    //SDL_Renderer *pRenderer;
    State state;
    SDL_Texture *pMenuTexture, *pBackgroundTexture, *pButtonTexture, *pButtonExitTexture, *pPlatformTexture, *pStartPlatformTexture;
    TTF_Font *pMenuFont;
    Background* pBackground;
    Button *pStartButton,  *pSettingsButton, *pQuitButton, *pLanguageButton, *pMoveLeftButton, *pMoveRightButton, *pReturnButton, *pEnglishButton, *pSwedishButton, *pResumeButton,
    *pMainMenuButton, *pCreateLobbyButton, *pJoinLobbyButton, *pMuteButton;

    bool chosenLanguage;

    Text *pStartButtonText, *pSettingsButtonText, *pQuitButtonText, *pLanguageButtonText,  *pMoveLeftButton1Text, *pMoveRightButton1Text, *pReturnButtonText, *pEnglishButtonText,
    *pSwedishButtonText, *pResumeButtonText, *pMainMenuButtonText, *pMoveLeftButton2Text, *pMoveRightButton2Text,*pYouAreDeadText, *pCreateLobbyButtonText, *pJoinLobbyButtonText,
    *pMuteButton1Text, *pMuteButton2Text, *pWhoWonText, *pWaitingText, *pInputIPText, *pEnterIPText, *pNrClientsText;
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
void initiateLanguage(FILE *fp, Game *pGame);
int handleError(Game* pGame, void* pMember, const char* (*GetError)(void));

void runGame(Game* pGame);

void handleMainMenu(Game* pGame, SDL_Event event, bool* pMute);
void renderMainMenu(Game* pGame, GameDisplay* pGameDisplay);
void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum, bool *pShowLang);
void renderSettingsMenu(Game *pGame, GameDisplay* pGameDisplay);
void handleLanguageMenu(Game* pGame, SDL_Event event, bool* pShowLang);
void renderLanguageMenu(Game* pGame, GameDisplay* pGameDisplay);
void readKeybindString(Game *pGame, int input);
void handleEnterInput(Game* pGame, SDL_Event event, int* pNum);
void handleLobbyMenu(Game* pGame, SDL_Event event, bool* pLeft, bool* pRight, int* pTime, int* pIndex, bool* pJoined);
void renderLobbyMenu(Game* pGame, GameDisplay* pGameDisplay);
void handleLobby(Game* pGame, SDL_Event event, int* pIndex, bool* pJoined);
void fillZero(Game* pGame, int max);
void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool *pLeft, bool *pRight, int* pTime, bool* pMute);
void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pLeft, bool* pRight, bool* pMute);
void handleGameMenu(Game* pGame, SDL_Event event, bool* pMute);
void renderGameMenu(Game* pGame);
void handleGameOver(Game* pGame, SDL_Event event);
void renderGameOver(Game* pGame, GameDisplay* pGameDisplay);

void resetGame(Game* pGame, bool* pLeft, bool* pRight, int* pTime);

void quitGame(Game* pGame);