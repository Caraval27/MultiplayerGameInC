#pragma once

#include "../include/platforms.h"

#define BACKGROUND_WIDTH 1024
#define BACKGROUND_HEIGHT 1536
#define BACKGROUND_SPEED 1

typedef struct{
    float upperSrcYPos, upperSrcHeight;
    float lowerSrcYPos, lowerSrcHeight;
    float upperDstYPos, upperDstHeight;
    float lowerDstYPos, lowerDstHeight;
} Background;

Background* createBackground(int windowHeight);
SDL_Texture* createPicture(SDL_Window* pWindow, SDL_Renderer* pRenderer, char picture[]);
void handleBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth);
void destroyBackground(Background* pBackground);
void destroyMusic(Mix_Music* pMainSound);
void destroyChunk(Mix_Chunk* pMusicChunk);
void handleStartPlatform(Platform* pStartingPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowHeight, int* pTime);
void resetStartPlatform(Platform* pStartingPlatform, int windowHeight, int* pTime);