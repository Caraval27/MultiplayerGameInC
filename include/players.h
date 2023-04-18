#ifndef PLAYERS_H
#define PLAYERS_H
#include "../include/platforms.h"

#define MAX_JUMP_HEIGHT 400
typedef struct player Player;

Player* createPlayer(int x, int y);
SDL_Texture* createPlayerCharacter(SDL_Rect* pPlayerRect, SDL_Renderer* pRenderer, SDL_Window* pWindow, int windowWidth, int windowHeight);
void renderPlayer(SDL_Renderer* pRenderer, Player* pPlayer);
void jumpPlayer(Player* pPlayer, SDL_Rect playerRect, int windowHeight, float platformY, float maxJumpHeight);
void movePlayer (Player* pPlayer, SDL_Rect playerRect, bool left, bool right, int width); //Funktion som hanterar att spelaren kan rörar sig i sidled med tangenttryck
void platformCollidePlayer(Player* pPlayer, SDL_Rect playerRect, Plank** platforms, float* pPlatformY, float* pMaxJumpHeight);
void updatePlayer(Player* pPlayer, SDL_Rect* pPlayerRect);
void destroyPlayer(Player* pPlayer);

#endif