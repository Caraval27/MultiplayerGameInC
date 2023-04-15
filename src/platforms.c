#include "../include/main.h"

struct platform{
    float posX, posY;
    float velocityY;
};

Platform* createPlatform(int windowWidth, int y){
    Platform* pPlatform = malloc(sizeof(Platform));
    pPlatform->posX = rand() % (windowWidth - PLATFORM_WIDTH);
    pPlatform->posY = y;
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
