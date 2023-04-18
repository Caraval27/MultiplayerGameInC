#include "../include/main.h"

#define PLANK_SCROLL_SPEED 2

Plank *createPlank(float x, float y, float w, float h) {
	Plank *pPlank = malloc(sizeof(Plank));
	pPlank->xPos = x;
	pPlank->yPos = y;
	pPlank->width = w;
	pPlank->height = h;
	return pPlank;
}

void scrollPlank(Plank *pPlank) {
	pPlank->yPos += PLANK_SCROLL_SPEED;
	// remember to destroy plank if out of bounds
}

void renderPlank(SDL_Renderer *pRenderer, Plank *pPlank) {
	SDL_Rect rect;
	rect.x = pPlank->xPos;
	rect.y = pPlank->yPos;
	rect.w = pPlank->width;
	rect.h = pPlank->height;
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(pRenderer, &rect);
}

void handlePlank(Plank** planks, SDL_Renderer* pRenderer, int windowWidth){
    if (SDL_GetTicks64() % 2000 < 17) {
        int i = 0;
        while (planks[i]) i++;
        int width = PLATFORM_WIDTH;
        int height = PLATFORM_HEIGHT;
        int x = (rand() % (windowWidth - width - (width / 4) * 2)) + width / 4;
        int y = 0 - height;
        planks[i] = createPlank(x, y, width, height);
    }

    {
        int i = 0;
        while (planks[i]) {
            renderPlank(pRenderer, planks[i]);
            scrollPlank(planks[i]);
            i++;
        }
    }
}

void destroyPlank(Plank** planks) {
    for (int i = 0; planks[i] != 0; i++) {
        if (planks[i]) {
            free(planks[i]);
        }
    }
}

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