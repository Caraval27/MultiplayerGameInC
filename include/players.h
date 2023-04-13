#ifndef players_h
#define players_h

typedef struct player Player;

Player* createPlayer(int x, int y);
void jumpPlayer(Player* pOnePlayer, SDL_Rect onePlayerRect, int h, float platform_height, float max_jump_height);
void playerCollisionPlatform(Player player, SDL_Rect playerRect, SDL_Rect platforms[], int nrOfPlatforms, float *pPlatformHeight, float *pMaxJumpHeight);
void updatePlayer(Player* pPlayer, SDL_Rect* pPlayerRect);
void destroyPlayer(Player* pPlayer);

#endif