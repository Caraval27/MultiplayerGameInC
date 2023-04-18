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

typedef enum {
    MAIN_MENU,
	SETTINGS_MENU,
	ONGOING,
	GAME_MENU,
	GAME_OVER
} GameState;

typedef struct {
    GameState state;
    SDL_Window *pWindow;
    int windowWidth, windowHeight;
    SDL_Renderer *pRenderer;
    SDL_Texture *pBackgroundTexture, *pMainMenuTexture;
    TTF_Font *pMainMenuFont;
    SDL_Rect windowUpperRect, windowLowerRect, backgroundUpperRect, backgroundLowerRect, mainMenuRect;
    Button* pQuitButton, pStartButton, pResumeButton, pMainMenuButton;
    Text* pQuitButtonText;
    Player* pPlayer;
	Plank *planks[1000];
    Platform* pPlatform;
    SDL_Rect playerRect, platformRect, quitButtonRect;
    int keybinds[NR_OF_KEYBINDS];
} Game;

int initiateGraphics(Game *pGame);
void runGame(Game *pGame);
void quitGame(Game *pGame);
void handleInputOngoing(Game *pGame, SDL_Event* event, bool* right, bool* left, bool* isRunning);

#endif
