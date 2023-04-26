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

void renderPlatform(Platform* pPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture) {
	SDL_Rect rect = {pPlatform->xPos, pPlatform->yPos, pPlatform->width, pPlatform->height};

    SDL_RenderCopy(pRenderer, pTexture, NULL, &rect);
}

void handlePlatforms(Platform** pPlatforms, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth){
    int i = 0;

    if (SDL_GetTicks64() % 1000 < 17) {
        i = 0;
        while (pPlatforms[i]) i++; // Variabel som direkt visar antalet plattformar?
        int width = PLATFORM_WIDTH;
        int height = PLATFORM_HEIGHT;
        int x = (rand() % (windowWidth - width - (width / 4) * 2)) + width / 4;
        int y = 0 - height;
        pPlatforms[i] = createPlatform(x, y, width, height);
    }

    i = 0;
    while (pPlatforms[i]) {
        renderPlatform(pPlatforms[i], pRenderer, pTexture);
        scrollPlatform(pPlatforms[i]);
        i++;
    }
}

void resetPlatforms(Platform** pPlatforms){
    destroyPlatforms(pPlatforms);
    for(int i = 0; pPlatforms[i] != 0; i++){
        pPlatforms[i] = 0;
    }
}

void destroyPlatform(Platform* pPlatform){
    free(pPlatform);
}

void destroyPlatforms(Platform** pPlatforms){
    for (int i = 0; pPlatforms[i] != 0; i++) {
        if (pPlatforms[i]) {
            destroyPlatform(pPlatforms[i]);
        }
    }
}