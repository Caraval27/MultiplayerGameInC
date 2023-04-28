#pragma once

#include "../include/platforms.h"

#define CHARACTER_WIDTH 60
#define CHARACTER_HEIGHT 60
#define MOVE_SPEED 300
#define JUMP_SPEED -1000
#define GRAVITY 1200

typedef struct {
    float xPos, yPos;
    float width, height;
    float xVelocity, yVelocity;
    bool alive;
} Player;

Player* createPlayer(float xPos, float yPos, float width, float height, float xVelocity, float yVelocity);
void initPlayer(Player** pPlayers, int nrOfPlayersLeft, int pNrOfPlayers, int windowWidth, int windowHeight, SDL_Texture** pPlayerTextures, SDL_Window* pWindow, SDL_Renderer* pRenderer);
//void handlePlayers(Player** pPlayers, int pNrOfPlayers, int *nrOfPlayersLeft, bool *pLeft, bool *pRight, int windowWidth, int windowHeight, Platform** pStartPlatform, Mix_Chunk *pJumpSound, State state, SDL_Renderer* pRenderer, SDL_Texture** pPlayerTextures);
void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth);
void jumpPlayer(Player* pPlayer, int startPlatformHeight, Mix_Chunk* pJumpSound);
void playerCollidePlatform(Player* pPlayer, Platform** pPlatforms, Mix_Chunk* pJumpSound);
void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_RendererFlip flip);
void destroyPlayers(Player** pPlayer);
void destroyPlayerTextures(SDL_Texture** pPlayerTexture);
