#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/world.h"
#include "../include/keybinds.h"
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
    Button *pStartButton, *pQuitButton, *pResumeButton, *pMainMenuButton;
    Text *pStartButtonText, *pQuitButtonText, *pResumeButtonText, *pMainMenuButtonText;
    Player* pPlayer1, *pPlayer2, *pPlayer3, *pPlayer4;
	Platform *platforms[1000];
    SDL_Rect player1Rect, player2Rect, platformRect, quitButtonRect, startButtonRect, resumeButtonRect, mainMenuButtonRect;
    int keybinds[NR_OF_KEYBINDS];
} Game;

int initiateGame(Game* pGame);
void runGame(Game* pGame);
void quitGame(Game* pGame);
void handleInputOngoing(State* pState, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft);

#endif
