#pragma once

#include "../include/platforms.h"

#define SPEED 100
#define MAX_JUMP_HEIGHT 400

typedef struct player Player;

Player* createPlayer(int x, int y, SDL_Rect* playerRect, int windowWidth, int windowHeight);
SDL_Texture* createPlayerCharacter(SDL_Renderer* pRenderer, SDL_Window* pWindow);
void jumpPlayer(Player* pPlayer, SDL_Rect playerRect, int windowHeight, float platformY, float maxJumpHeight);
void movePlayer (Player* pPlayer, SDL_Rect playerRect, bool left, bool right, int width); //Funktion som hanterar att spelaren kan rörar sig i sidled med tangenttryck
void platformCollidePlayer(Player* pPlayer, SDL_Rect playerRect, Platform** platforms, float* pPlatformY, float* pMaxJumpHeight);
void renderPlayer(SDL_Renderer* pRenderer, SDL_Texture* pTexture, Player* pPlayer, SDL_Rect* pPlayerRect);
void destroyPlayer(Player* pPlayer);
