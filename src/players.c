#include "../include/main.h"

#define SPEED 100

struct player{
    float posX, posY;
    float velocityY, velocityX;
};

Player* createPlayer(int x, int y){
    Player* pPlayer = malloc(sizeof(Player));

    pPlayer->posX = x;
    pPlayer->posY = y;
    pPlayer->velocityX = 0;
    pPlayer->velocityY = 5;

    return pPlayer;
}

void createPlayerRect(SDL_Rect* pPlayerRect, int windowWidth, int windowHeight){
    pPlayerRect->w = 50;
    pPlayerRect->h = 50;
    pPlayerRect->x = windowWidth - pPlayerRect->w;
    pPlayerRect->y = windowHeight - pPlayerRect->h;
}

void jumpPlayer(Player* pPlayer, SDL_Rect playerRect, int windowHeight, float platformY, float maxJumpHeight){
    pPlayer->posY += pPlayer->velocityY;

    if (pPlayer->posY + playerRect.h >= windowHeight - platformY){
        pPlayer->posY = windowHeight - playerRect.h - platformY;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
    if (playerRect.y <= 0){
        pPlayer->posY = 0;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
    if (pPlayer->posY + playerRect.h < maxJumpHeight){
        pPlayer->posY = maxJumpHeight - playerRect.h;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
}

void movePlayer(Player* pPlayer, SDL_Rect playerRect, bool left, bool right, int windowWidth){
    if (pPlayer->velocityX > SPEED) pPlayer->velocityX = 0;
    if (left && !right){
        pPlayer->velocityX += SPEED;
        pPlayer->posX -= (pPlayer->velocityX) / 60;

    }
    else if(right && !left){
        pPlayer->velocityX += SPEED;
        pPlayer->posX += (pPlayer->velocityX) / 60;
    }

    if(pPlayer->posX<0) pPlayer->posX = 0;
    if (pPlayer->posX > windowWidth-playerRect.w) pPlayer->posX = windowWidth - playerRect.w;
}

void platformCollidePlayer(Player* pPlayer, SDL_Rect playerRect, Plank **platforms, float* pPlatformY, float* pMaxJumpHeight){
    for (int i = 0; platforms[i] != 0; i++){
        if (pPlayer->posY + playerRect.h >= platforms[i]->yPos &&
            pPlayer->posY < platforms[i]->yPos + platforms[i]->height &&
            pPlayer->posX + playerRect.w >= platforms[i]->xPos &&
            pPlayer->posX < platforms[i]->xPos + platforms[i]->width &&
            pPlayer->velocityY >= 0){
            pPlayer->posY = platforms[i]->yPos - playerRect.h; // vet ej om behövs
            *pPlatformY = platforms[i]->yPos + platforms[i]->height;
            *pMaxJumpHeight = *pPlatformY - (MAX_JUMP_HEIGHT - 100);
            if (*pMaxJumpHeight < 1) *pMaxJumpHeight = 10 + playerRect.h;
            pPlayer->velocityY = -(pPlayer->velocityY);
        }
        else {
            *pPlatformY = 0;
            //*pMaxJumpHeight = MAX_JUMP_HEIGHT;
        }
    }
} 

void updatePlayer(Player* pPlayer, SDL_Rect* pPlayerRect){
    pPlayerRect->x = pPlayer->posX;
    pPlayerRect->y = pPlayer->posY;
}

void destroyPlayer(Player* pPlayer){
    free(pPlayer);
}