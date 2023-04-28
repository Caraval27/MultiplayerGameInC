#pragma once

#include "../include/menu.h"
#include "../include/text.h"
#include "../include/platforms.h"

#define MAX_PLAYERS 10
#define CHARACTER_PICTURE "../assets/musse.png"
#define CHARACTER_WIDTH 60
#define CHARACTER_HEIGHT 60
#define MOVE_SPEED 300
#define JUMP_SPEED -700
#define GRAVITY 1200

typedef struct {
    float xPos, yPos;
    float width, height;
    float xVelocity, yVelocity;
    bool alive;
} Player;

Player* createPlayer(float xPos, float yPos, float width, float height, float xVelocity, float yVelocity);
void initPlayers(Player** pPlayers, int pNrOfPlayers, int windowWidth, int windowHeight, SDL_Texture** pPlayerTextures, SDL_Window* pWindow, SDL_Renderer* pRenderer);
void handlePlayers(Player** pPlayers, int pNrOfPlayers, int *nrOfPlayersLeft, bool *pLeft, bool *pRight, int windowWidth, int windowHeight, Platform* pStartPlatform, Mix_Chunk *pJumpSound, State* pState, SDL_Renderer* pRenderer, SDL_Texture** pPlayerTextures, SDL_RendererFlip flip, Platform** pPlatforms, Text* pGameOverText);
void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth);
void jumpPlayer(Player* pPlayer, int startPlatformHeight, Mix_Chunk* pJumpSound);
void playerCollidePlatform(Player* pPlayer, Platform** pPlatforms, Mix_Chunk* pJumpSound);
void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_RendererFlip flip);
void destroyPlayers(Player** pPlayer);
void destroyPlayerTextures(SDL_Texture** pPlayerTexture);
int playerIsDead(Player* pPlayer, int windowHeight);
void checkIfPlayerDead(Player* pPlayer, int windowHeight, State* pState, int* nrOfPlayersLeft);
void handleWin(int nrOfPlayersLeft, State* pState);