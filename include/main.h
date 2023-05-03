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
    State state;
    SDL_Window *pWindow;
    int windowWidth, windowHeight;
    SDL_Renderer *pRenderer;
    SDL_Texture *pBackgroundTexture, *pMenuTexture, *pStartPlatformTexture, *pPlatformTexture;
    TTF_Font *pMenuFont;
    Background* pBackground;
    Button *pStartButton,  *pSettingsButton, *pQuitButton, *pLanguageButton, *pMoveLeftButton, *pMoveRightButton, *pReturnButton, *pEnglishButton, *pSwedishButton, *pResumeButton, *pMainMenuButton,
    *pMoveLeft1Button, *pMoveRight1Button, *pCreateLobbyButton, *pJoinLobbyButton;

    bool chosenLanguage;

    Text *pStartButtonText, *pSettingsButtonText, *pQuitButtonText, *pLanguageButtonText,  *pMoveLeftButtonText, *pMoveRightButtonText, *pReturnButtonText, *pEnglishButtonText,
    *pSwedishButtonText, *pResumeButtonText, *pMainMenuButtonText, *pMoveLeft1ButtonText, *pMoveRight1ButtonText,*pGameOverText, *pCreateLobbyButtonText, *pJoinLobbyButtonText;
    Text* pWhoWonText[MAX_PLAYERS + 1];
    Player* pPlayers[MAX_PLAYERS];
    SDL_Texture* pPlayerTextures[MAX_PLAYERS];
    int nrOfPlayers, nrOfPlayersLeft;
    SDL_RendererFlip flip;
    Platform *pPlatforms[100], *pStartPlatform;

    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
    Mix_Chunk *pJumpSound, *pWinSound;
    Mix_Music *pMainSound;

	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand pClientCommands[COMMAND_BUFFER];
} Game;

int initiateGame(Game* pGame);
void readKeybindString(Game *pGame, int input);
void initiateLanguage(FILE *fp, Game *pGame);
int handleError(Game* pGame, void* pMember, const char* (*GetError)(void));

void runGame(Game* pGame);

void handleMainMenu(Game* pGame, SDL_Event event, bool* pMute);
void renderMainMenu(Game* pGame);
void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum, bool *pShowLang);
void renderSettingsMenu(Game *pGame);
void handleLanguageMenu(Game* pGame, SDL_Event event, bool* pShowLang);
void renderLanguageMenu(Game* pGame);
void handleLobbyMenu(Game* pGame, SDL_Event event, int* pTime);
void renderLobbyMenu(Game* pGame);
void handleEnterInput(Game* pGame, SDL_Event event, int* pNum);
void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool *pLeft, bool *pRight, int* pTime, bool* pMute);
void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pLeft, bool* pRight, bool* pMute);
void handleGameMenu(Game* pGame, SDL_Event event, bool* pMute);
void renderGameMenu(Game* pGame);
void handleGameOver(Game* pGame, SDL_Event event);
void renderGameOver(Game* pGame);

void resetGame(Game* pGame, int* pTime);

void quitGame(Game* pGame);