#ifndef players_h
#define players_h

typedef struct player Player;

Player* createPLayer(int x, int y);
void destroyPlayer(Player* pPlayer);


#endif