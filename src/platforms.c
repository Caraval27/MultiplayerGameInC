#include "../include/main.h"

Platform *createPlatform(float xPos, float yPos, float width, float height) {
	Platform *pPlatform = malloc(sizeof(Platform));

	pPlatform->xPos = xPos;
	pPlatform->yPos = yPos;
	pPlatform->width = width;
	pPlatform->height = height;

	return pPlatform;
}

void initPlatforms(Platform **pPlatforms, GameDisplay* pGameDisplay) {
    int i = 0, j = 0, width = 0, height = 0, x = 0, y = 0;
    width = PLATFORM_WIDTH;
    height = PLATFORM_HEIGHT;
    y = 0 - height;
    for (i = 0; i < NR_OF_PLATFORMS; i++) {
        x = (rand() % (pGameDisplay->windowWidth - width - (width / 4) * 2)) + width / 4;
        pPlatforms[i] = createPlatform(x, y, width, height);
        y -= PLATFORM_SPACING;
    }
}

void scrollPlatform(Platform* pPlatform) {
    static int time = 0;
    static float platform_speed = 2;
    time++;
    pPlatform->yPos += platform_speed;
    if(time % 20000 == 0 && platform_speed < 5){
        platform_speed+= 0.1;
    }
}

void cleanupPlatforms(Platform **pPlatforms, int windowHeight) {
	int limit = 20;
	int nPlatforms = 0;
	while (pPlatforms[nPlatforms] != NULL) nPlatforms++;
	int overflow = nPlatforms - limit;

	if (overflow <= 0) return;

	for (int i = 0; i < nPlatforms; i++) {
		if (i < overflow) {
			free(pPlatforms[i]);
		}
		if (i < limit) {
			pPlatforms[i] = pPlatforms[i + overflow];
		}
		if (limit <= i && i < nPlatforms) {
			pPlatforms[i] = NULL;
		}
	}
}

void renderPlatform(Platform* pPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture) {
	SDL_Rect rect = {pPlatform->xPos, pPlatform->yPos, pPlatform->width, pPlatform->height};

    SDL_RenderCopy(pRenderer, pTexture, NULL, &rect);
}

void handlePlatforms(Platform** pPlatforms, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight, bool isHost) {
    int width = 0, height = 0, x = 0, y = 0;

    for(int i = 0; i < NR_OF_PLATFORMS; i++){
        renderPlatform(pPlatforms[i], pRenderer, pTexture);
    }

    if(isHost){
        for(int i = 0; i < NR_OF_PLATFORMS; i++){
            if(pPlatforms[i]->yPos - PLATFORM_HEIGHT > windowHeight){
                y = (0 - (PLATFORM_HEIGHT + i*PLATFORM_SPACING));
                x = (rand() % (windowWidth - PLATFORM_WIDTH - (PLATFORM_WIDTH / 4) * 2)) + PLATFORM_WIDTH / 4;
                pPlatforms[i]->yPos = y;
                pPlatforms[i]->xPos = x;
            }else{
                scrollPlatform(pPlatforms[i]);
            }
        }
    }

    /*
    i = 0;
    while (pPlatforms[i]) {
        renderPlatform(pPlatforms[i], pRenderer, pTexture);
        if(isHost)
            scrollPlatform(pPlatforms[i]);
        i++;
    }
    */
}

void handleStartPlatform(Platform* pStartPlatform, Platform* pFirstPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowHeight, int* pTime){
    (*pTime)++;

    if (pStartPlatform->yPos < windowHeight) {
        if (pFirstPlatform && pFirstPlatform->yPos >= pStartPlatform->yPos - pFirstPlatform->height) {
            pStartPlatform->yPos += PLATFORM_SPEED;
        }
        renderPlatform(pStartPlatform, pRenderer, pTexture);
    }
}

void resetPlatforms(Platform** pPlatforms){
    for(int i = 0; i < NR_OF_PLATFORMS; i++){
        pPlatforms[i] = 0;
    }
}

void resetStartPlatform(Platform* pStartPlatform, int windowHeight, int* pTime){
    pStartPlatform->yPos = windowHeight - 100;
    *pTime = 0;
}

void destroyPlatform(Platform* pPlatform){
    if (pPlatform){
        free(pPlatform);
    }
}

void destroyPlatforms(Platform** pPlatforms){
    for (int i = 0; i < NR_OF_PLATFORMS; i++){
        destroyPlatform(pPlatforms[i]);
    }
}
