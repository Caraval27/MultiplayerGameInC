#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/platforms.h"

struct platform{
    float posX, posY;
    float velocityY;
};

Platform* createPlatform(int windowHeight, int y){
    Platform* pPlatform = malloc(sizeof(Platform));
    pPlatform->posX = rand()%windowHeight;
    pPlatform->posX = y;
    pPlatform->velocityY = 5;

    return pPlatform;
}

void updatePlatform(Platform* pPlatform, SDL_Rect* pPlatformRect){
    pPlatformRect->x = pPlatform->posX;
    pPlatformRect->y = pPlatform->posY;
}

void destroyPlatform(Platform* pPlatform){
    free(pPlatform);
}
