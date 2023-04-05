typedef struct{
    float posX, posY;
    float velocityY, velocityX;
    float accelerationY, accelerationX;
}Player;


Player* createPLayer(int x, int y){
    Player *pPlayer = malloc(sizeof(Player));
    pPlayer->posX = x;
    pPlayer->posX = y;
    pPlayer->velocityX = 0; 
    pPlayer->velocityY = 0;
    pPlayer->accelerationY = 0;
    pPlayer->accelerationX = 0;

    return pPlayer;
}

void destroyPlayer(Player* pPlayer){
    free(pPlayer);
}