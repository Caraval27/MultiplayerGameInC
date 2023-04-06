#ifndef players_h
#define players_h

typedef struct Player;

Player* createPLayer(int x, int y);
void destroyPlayer(Player* pPlayer);

#endif