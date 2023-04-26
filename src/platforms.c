#include "../include/main.h"

Platform *createPlatform(float xPos, float yPos, float width, float height) {
	Platform *pPlatform = malloc(sizeof(Platform));

	pPlatform->xPos = xPos;
	pPlatform->yPos = yPos;
	pPlatform->width = width;
	pPlatform->height = height;

	return pPlatform;
}

void scrollPlatform(Platform* pPlatform) {
	pPlatform->yPos += PLATFORM_SPEED;
	// remember to destroy plank if out of bounds
}

void renderPlatform(SDL_Renderer* pRenderer, Platform* pPlatform) {
	SDL_Rect rect = {pPlatform->xPos, pPlatform->yPos, pPlatform->width, pPlatform->height};

	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(pRenderer, &rect);
}

void handlePlatform(Platform** platforms, SDL_Renderer* pRenderer, int windowWidth){
    int i = 0;

    if (SDL_GetTicks64() % 1000 < 17) {
        i = 0;
        while (platforms[i]) i++; // Variabel som direkt visar antalet plattformar?
        int width = PLATFORM_WIDTH;
        int height = PLATFORM_HEIGHT;
        int x = (rand() % (windowWidth - width - (width / 4) * 2)) + width / 4;
        int y = 0 - height;
        platforms[i] = createPlatform(x, y, width, height);
    }

    i = 0;
    while (platforms[i]) {
        renderPlatform(pRenderer, platforms[i]);
        scrollPlatform(platforms[i]);
        i++;
    }
}

void destroyPlatform(Platform** platforms) {
    for (int i = 0; platforms[i] != 0; i++) {
        if (platforms[i]) {
            free(platforms[i]);
        }
    }
}
