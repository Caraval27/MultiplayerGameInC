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
    pPlayer->velocityY = 0;
    pPlayer->accelerationY = 0;
    pPlayer->accelerationX = 0;

    return pPlayer;
}

void jumpPlayer(Player* pOnePlayer, SDL_Rect onePlayerRect, int h, float platform_height, float max_jump_height){

    pOnePlayer->posY += pOnePlayer->velocityY;
    
    if (pOnePlayer->posY + onePlayerRect.h >= h - platform_height) {
        pOnePlayer->posY = h - onePlayerRect.h - platform_height;
        pOnePlayer->velocityY = -(pOnePlayer->velocityY);
    }
    if (onePlayerRect.y <= 0){
        pOnePlayer->posY = 0;
        pOnePlayer->velocityY = -(pOnePlayer->velocityY);
    }
    if (pOnePlayer->posY + onePlayerRect.h < max_jump_height) {
        pOnePlayer->posY = max_jump_height - onePlayerRect.h;
        pOnePlayer->velocityY = -(pOnePlayer->velocityY);
    }
}

void destroyPlayer(Player* pPlayer){
    free(pPlayer);
}

void updatePlayer(Player* pPlayer, SDL_Rect playerRect){
    pPlayer->posX = playerRect.x;
    pPlayer->posY = playerRect.y;
}