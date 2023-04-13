#ifndef players_h
#define players_h

typedef struct player Player;

Player* createPlayer(int x, int y);
void jumpPlayer(Player* pOnePlayer, SDL_Rect onePlayerRect, int h, float platform_height, float max_jump_height);
void destroyPlayer(Player* pPlayer);
void updatePlayer(Player* pPlayer, SDL_Rect playerRect);

#endif