#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/world.h"
#include "../include/keybinds.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"

typedef enum {
    MAIN_MENU,
	SETTINGS_MENU,
	ONGOING,
	GAME_MENU,
	GAME_OVER
} GameState;

typedef struct {
    int windowWidth, windowHeight, keybinds[NR_OF_KEYBINDS];
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *pBackgroundTexture, *pMenuBackgroundTexture;
    SDL_Rect windowUpperRect, windowLowerRect, imageUpperRect, imageLowerRect;
    Player* pPlayer;
    Platform* pPlatform;
	Plank *planks[1000];
    SDL_Rect playerRect, platformRect;
    GameState state;
    SDL_Rect menuBackgroundRect;
} Game;

int initiateGraphics(Game *pGame);
void runGame(Game *pGame);
void quitGame(Game *pGame);

#endif
