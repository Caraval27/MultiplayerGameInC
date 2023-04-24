#pragma once

#include "../include/platforms.h"

#define SPEED 100
#define JUMP_HEIGHT 400

typedef struct {
    float xPos, yPos;
    float width, height;
    float xVelocity, yVelocity;
} Player;

Player* createPlayer(int xPos, int yPos, int width, int height, int xVelocity, int yVelocity);
SDL_Texture* createPlayerCharacter(SDL_Renderer* pRenderer, SDL_Window* pWindow, char characterPicture[]);
void jumpPlayer(Player* pPlayer, int windowHeight, float PlatformYPos, float jumpHeight);
void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth);
void playerCollidePlatform(Player* pPlayer, Platform** platforms, float* pPlatformYPos, float* pJumpHeight);
void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void destroyPlayer(Player* pPlayer);
