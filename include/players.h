#pragma once

#include "../include/platforms.h"

#define SPEED 100
#define JUMP_HEIGHT 400

typedef struct {
    float xPos, yPos;
    float width, height;
    float xVelocity, yVelocity;
    SDL_Texture* playerTexture;
    bool alive;
} Player;

Player* createPlayer(int xPos, int yPos, int width, int height, int xVelocity, int yVelocity);
SDL_Texture* createPlayerCharacter(SDL_Renderer* pRenderer, SDL_Window* pWindow, char characterPicture[]);
void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth);
void jumpPlayer(Player* pPlayer, float jumpHeight, int height, Mix_Chunk* pJumpSound);
void playerCollidePlatform(Player* pPlayer, Platform** platforms, float* pJumpHeight, int windowHeight, Mix_Chunk* pJumpSound);
void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void destroyPlayer(Player** pPlayer);
void destroyPlayerTexture(SDL_Texture** pPlayerTexture);