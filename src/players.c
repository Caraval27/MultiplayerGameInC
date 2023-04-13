#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/players.h"

struct player{
    float posX, posY;
    float velocityY, velocityX;
    float accelerationY, accelerationX;
};

Player* createPlayer(int x, int y){
    Player* pPlayer = malloc(sizeof(Player));
    pPlayer->posX = x;
    pPlayer->posX = y;
    pPlayer->velocityX = 0;
    pPlayer->velocityY = 5;
    pPlayer->accelerationY = 0;
    pPlayer->accelerationX = 0;

    return pPlayer;
}

void jumpPlayer(Player* pPlayer, SDL_Rect playerRect, int h, float platformHeight, float maxJumpHeight){

    pPlayer->posY += pPlayer->velocityY;
    
    if (pPlayer->posY + playerRect.h >= h - platformHeight){
        pPlayer->posY = h - playerRect.h - platformHeight;
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

void playerCollisionPlatform(Player player, SDL_Rect playerRect, SDL_Rect platforms[], int nrOfPlatforms, float *pPlatformHeight, float *pMaxJumpHeight){
    for (int i = 0; i < nrOfPlatforms; i++){
        if (player.posY + playerRect.h >= platforms[i].y &&
            player.posY < platforms[i].y + platforms[i].h &&
            player.posX + platforms[i].w >= platforms[i].x &&
            player.posX < platforms[i].x + platforms[i].w &&
            player.velocityY >= 0){
            *pPlatformHeight = platforms[i].h + 50;
            *pMaxJumpHeight += platforms[i].h + 50; 
        }
        else {
            *pPlatformHeight = 0;
            *pMaxJumpHeight = 200;
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