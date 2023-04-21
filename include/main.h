#pragma once

#include "../include/libraries.h"
#include "../include/world.h"
#include "../include/fileManagement.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"
#include "../include/text.h"

typedef struct {
    State state;
    SDL_Window *pWindow;
    int windowWidth, windowHeight;
    SDL_Renderer *pRenderer;
    SDL_Texture *pBackgroundTexture, *pMainMenuTexture, *pPlayer1Texture, *pPlayer2Texture;
    TTF_Font *pMainMenuFont;
    SDL_Rect mainMenuRect;
    Background* pBackground;
    Button *pStartButton, *pQuitButton, *pResumeButton, *pMainMenuButton, *pSettingsButton, *pLanguageButton, *pEnglishButton, *pSwedishButton, *pReturnButton;
    Text *pStartButtonText, *pQuitButtonText, *pResumeButtonText, *pMainMenuButtonText, *pSettingsButtonText, *pLanguageButtonText, *pEnglishButtonText, *pSwedishButtonText, *pReturnButtonText;
    Player* pPlayer1, *pPlayer2;
	Platform *platforms[1000];
    SDL_Rect player1Rect, player2Rect, platformRect, quitButtonRect, startButtonRect, resumeButtonRect, mainMenuButtonRect, settingsButtonRect, languageButtonRect, englishButtonRect, swedishButtonRect, returnButtonRect;
    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
    Mix_Chunk *pJumpSound;
    Mix_Music *pMainSound;
} Game;

void initiateLanguage(FILE *fp, Game *pGame);
int initiateGame(Game* pGame);
void runGame(Game* pGame);
void quitGame(Game* pGame);
void handleInputOngoing(State* pState, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft);
int handleError(Game* pGame, void* pMember, const char* (*GetError)());
