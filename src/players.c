#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/players.h"

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
    pPlayer->velocityY = 1;

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

void movePlayer (Player* pPlayer, SDL_Rect playerRect, bool left, bool right, int width)
{
    if (pPlayer->velocityX > SPEED) pPlayer->velocityX = 0;
    if (left && !right){
        pPlayer->velocityX += SPEED;
        pPlayer->posX -= (pPlayer->velocityX)/60; //uppdaterar nya xpositionen
        
    }
    else if(right && !left){
            pPlayer->velocityX += SPEED;
            pPlayer->posX += (pPlayer->velocityX)/60;
    }

    if(pPlayer->posX<0) pPlayer->posX = 0;
    if (pPlayer->posX > width-playerRect.w) pPlayer->posX = width-playerRect.w;
}

/* void platformCollidePlayer(Player player, SDL_Rect playerRect, SDL_Rect platforms[], int nrOfPlatforms, float *pPlatformHeight, float *pMaxJumpHeight){
    for (int i = 0; i < nrOfPlatforms; i++){
        if (player.posY + playerRect.h >= platform.y &&
            player.posY < platform.y + platform.h &&
            player.posX + platform.w >= platform.x &&
            player.posX < platform.x + platform.w &&
            player.velocityY >= 0){
            *pPlatformHeight = platform.h + 50;
            *pMaxJumpHeight += platform.h + 50; 
        }
        else {
            *pPlatformHeight = 0;
            *pMaxJumpHeight = 200;
        }
    }
} */

void platformCollidePlayer(Player* pPlayer, SDL_Rect playerRect, SDL_Rect platform, int nrOfPlatforms, float *pPlatformHeight, float *pMaxJumpHeight){
    for (int i = 0; i < nrOfPlatforms; i++){
        if (pPlayer->posY + playerRect.h >= platform.y &&
            pPlayer->posY < platform.y + platform.h &&
            pPlayer->posX + platform.w >= platform.x &&
            pPlayer->posX < platform.x + platform.w &&
            pPlayer->velocityY >= 0){
            *pPlatformHeight = platform.h + 50;
            *pMaxJumpHeight += platform.h + 50; 
        }
        else {
            *pPlatformHeight = 0;
            *pMaxJumpHeight = 400;
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