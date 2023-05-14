#pragma once

#include "../include/text.h"
#include "../include/platforms.h"
#include "../include/world.h"

#define MAX_PLAYERS 6
#define CHARACTER_PICTURE "../assets/musse.png"
#define CHARACTER_WIDTH 60
#define CHARACTER_HEIGHT 60
#define MOVE_SPEED 300
#define JUMP_SPEED -700
#define GRAVITY 1200

typedef struct {
    IPaddress ip;
    bool alive;
    float xPos, yPos;
    float width, height;
    float xVelocity, yVelocity;
	bool moveLeft, moveRight;
	SDL_RendererFlip flip;
} Player;

Player* createPlayer(float xPos, float yPos, float width, float height, float xVelocity, float yVelocity);
void initPlayer(Player** pPlayers, int* pNrOfPlayers, int *pNrOfPlayersLeft, int windowWidth, float startPlatformHeight, SDL_Texture** pPlayerTextures, SDL_Window* pWindow, SDL_Renderer* pRenderer, int* pSubtractXpos, int* pIncreaseXpos, char avatar[]);
void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth);
void jumpPlayer(Player* pPlayer, int startPlatformYPos, Mix_Chunk* pJumpSound, bool* pMute);
void playerCollidePlatform(Player* pPlayer, Platform** pPlatforms, Mix_Chunk* pJumpSound, bool* pMute);
int playerIsDead(Player* pPlayer, int windowHeight);
void checkIfPlayerDead(Player* pPlayer, int windowHeight, State* pState, int* nrOfPlayersLeft);
void handleWin(int nrOfPlayersLeft, State* pState, Mix_Chunk* pWinSound, bool* pMute);
void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void handlePlayers(Player** pPlayers, int pNrOfPlayers, int *nrOfPlayersLeft, bool *pLeft, bool *pRight, bool* pMute, int windowWidth, int windowHeight, Platform* pStartPlatform, Mix_Chunk *pJumpSound, Mix_Chunk* pWinSound, State* pState, SDL_Renderer* pRenderer, SDL_Texture** pPlayerTextures, Platform** pPlatforms, Text* pGameOverText, bool* isHost);
void resetPlayers(Player** pPlayers, int* pNrOfPlayers, int* pNrOfPlayersLeft);
void destroyPlayers(Player** pPlayer);
void destroyPlayerTextures(SDL_Texture** pPlayerTexture);