#pragma once

#include "../include/libraries.h"
#include "../include/world.h"
#include "../include/files.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"
#include "../include/text.h"
#include "../include/network.h"

#define MAX_PLAYERS 6
#define BACKGROUND_PICTURE "../assets/background.png"
#define PLATFORM_PICTURE "../assets/iceBlock.png"
#define START_PLATFORM_PICTURE "../assets/iceBlock.png"
#define CHARACTER_PICTURE "../assets/musse.png"
#define MAIN_MENU_PICTURE "../assets/mainMenuBackground.png"

typedef struct {
    State state;
    SDL_Window *pWindow;
    int windowWidth, windowHeight;
    SDL_Renderer *pRenderer;
    SDL_RendererFlip *flip;
    SDL_Texture *pBackgroundTexture, *pMenuTexture, *pStartPlatformTexture, *pPlatformTexture;
    TTF_Font *pMenuFont;
    Background* pBackground;
    Button *pStartButton, *pQuitButton, *pResumeButton, *pMainMenuButton, *pSettingsButton, *pLanguageButton, *pEnglishButton, *pSwedishButton, *pReturnButton,
    *pMoveLeft1Button, *pMoveRight1Button, *pMoveLeft2Button, *pMoveRight2Button;

    Text *pStartButtonText, *pQuitButtonText, *pResumeButtonText, *pMainMenuButtonText, *pSettingsButtonText, *pLanguageButtonText, *pEnglishButtonText,
    *pSwedishButtonText, *pReturnButtonText, *pMoveLeft1ButtonText, *pMoveRight1ButtonText, *pMoveLeft2ButtonText, *pMoveRight2ButtonText, *pGameOverText;

    Player* pPlayers[6];
    SDL_Texture* pPlayerTextures[6];
    int pNrOfPlayers, nrOfPlayersLeft;

    Platform *pPlatforms[1000], *pStartPlatform;

    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
    Mix_Chunk *pJumpSound;
    Mix_Music *pMainSound;

	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand *pClientCommand;
} Game;

int initiateGame(Game* pGame);
void initiateLanguage(FILE *fp, Game *pGame);
int handleError(Game* pGame, void* pMember, const char* (*GetError)(void));

void runGame(Game* pGame);

void handleMainMenu(Game* pGame, SDL_Event event, int* pTime);
void renderMainMenu(Game* pGame);
void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum);
void renderSettingsMenu(Game *pGame);
void handleLanguageMenu(Game* pGame, bool* pShowLang);
void renderLanguageMenu(Game* pGame);
void handleEnterInput(Game* pGame, SDL_Event event, int* pNum);
void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pRight, bool* pLeft, int* pTime);
void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft);
void handleGameMenu(Game* pGame, SDL_Event event);
void renderGameMenu(Game* pGame);
void handleGameOverMenu(Game* pGame, SDL_Event event);

void handlePlayers(Game* pGame, bool* pLeft, bool* pRight);

void resetGame(Game* pGame, int* pTime);

void quitGame(Game* pGame);