#pragma once

#include "../include/platforms.h"

#define SPEED 300
#define JUMP_HEIGHT 400

typedef struct {
    float xPos, yPos;
    float width, height;
    float xVelocity, yVelocity;
} Player;

Player* createPlayer(float xPos, float yPos, float width, float height, float xVelocity, float yVelocity);
SDL_Texture* createPlayerCharacter(SDL_Renderer* pRenderer, SDL_Window* pWindow, char characterPicture[]);
void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth);
void jumpPlayer(Player* pPlayer, float jumpHeight, int windowHeight);
void playerCollidePlatform(Player* pPlayer, Platform** platforms, float* pJumpHeight, int windowHeight);
void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void destroyPlayer(Player* pPlayer);
