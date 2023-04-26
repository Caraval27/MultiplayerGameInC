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
#define CHARACTER_PICTURE "../assets/musse.png"
#define BACKGROUND_PICTURE "../assets/background.png"
#define STARTING_PLATFORM_PICTURE "../assets/iceBlock.png"
#define MAIN_MENU_PICTURE "../assets/menuBackground.jpeg"

typedef struct {
    State state;
    SDL_Window *pWindow;
    int windowWidth, windowHeight;
    SDL_Renderer *pRenderer;
    SDL_RendererFlip *flip;
    SDL_Texture *pBackgroundTexture, *pMainMenuTexture, *pStartPlatformTexture;
    TTF_Font *pMainMenuFont;
    Background* pBackground;
    Button *pStartButton, *pQuitButton, *pResumeButton, *pMainMenuButton, *pSettingsButton, *pLanguageButton, *pEnglishButton, *pSwedishButton, *pReturnButton,
    *pMoveLeft1Button, *pMoveRight1Button, *pMoveLeft2Button, *pMoveRight2Button;

    Text *pStartButtonText, *pQuitButtonText, *pResumeButtonText, *pMainMenuButtonText, *pSettingsButtonText, *pLanguageButtonText, *pEnglishButtonText,
    *pSwedishButtonText, *pReturnButtonText, *pMoveLeft1ButtonText, *pMoveRight1ButtonText, *pMoveLeft2ButtonText, *pMoveRight2ButtonText, *pGameOverText;

    Player *pPlayers[6];
    SDL_Texture* pPlayerTextures[6];
    int pNrOfPlayers;

    Platform *platforms[1000], *pStartingPlatform;

    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
    Mix_Chunk *pJumpSound;
    Mix_Music *pMainSound;

	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand *pClientCommand;
} Game;

int initiateGame(Game* pGame);
void runGame(Game* pGame);
void quitGame(Game* pGame);

int handleError(Game* pGame, void* pMember, const char* (*GetError)());
void initiateLanguage(FILE *fp, Game *pGame);

void handleMainMenu(Game* pGame, SDL_Event event, int* pSec);
void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum);
void renderSettingsMenu(Game *pGame);
void handleEnterInput(Game* pGame, SDL_Event event, int* pNum);
void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pRight, bool* pLeft, float* pMaxJumpHeight, int* pSec);
void handleInputOngoing(State* pState, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft, int keybinds[]);
void handleGameMenu(Game* pGame, SDL_Event event);
void handlePlayers(Game* pGame, bool* pLeft, bool* pRight, float *pJumpHeight);
