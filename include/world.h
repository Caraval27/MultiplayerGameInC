#pragma once

#include "../include/platforms.h"

#define BACKGROUND_WIDTH 1024
#define BACKGROUND_HEIGHT 1536
#define BACKGROUND_SPEED 1

typedef struct background Background;

Background* createBackground(int windowHeight);
SDL_Texture* createBackgroundImage(SDL_Window* pWindow, SDL_Renderer* pRenderer, char picture[]);
void handleBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth);
void destroyBackground(Background* pBackground);
void destroyMusic(Mix_Music* pMainSound);
void destroyChuck(Mix_Chunk* pMusicChuck);
void handleStartingPlatform(Platform* pStartingPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowHeight, int* pSec);
void resetStartingPlatform(Platform* pStartingPlatform, int windowHeight, int* pSec);