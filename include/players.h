#ifndef players_h
#define players_h

typedef struct player Player;

Player* createPlayer(int x, int y);
void destroyPlayer(Player* pPlayer);
void updatePlayer(Player* pPlayer, SDL_Rect playerRect);

#endif