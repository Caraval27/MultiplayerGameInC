#include "../include/main.h"

Platform *createPlatform(float xPos, float yPos, float width, float height) {
	Platform *pPlatform = malloc(sizeof(Platform));

	pPlatform->xPos = xPos;
	pPlatform->yPos = yPos;
	pPlatform->width = width;
	pPlatform->height = height;
    pPlatform->created = true;

	return pPlatform;
}

void scrollPlatform(Platform* pPlatform) {
	pPlatform->yPos += PLATFORM_SPEED;
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
    int i = 0, j = 0, width = 0, height = 0, x = 0, y = 0;
    if (SDL_GetTicks64() % 1000 < 17) {
        cleanupPlatforms(pPlatforms, windowHeight);
    if(isHost){

        i = 0;

        while (pPlatforms[i] != NULL) i++;
        width = PLATFORM_WIDTH;
        height = PLATFORM_HEIGHT;
        y = 0 - height;
        for (j = 0; j < Y_NR_OF_PLATFORMS; j++)
        {
            x = (rand() % (windowWidth - width - (width / 4) * 2)) + width / 4;
            if (i > 0 && x + PLATFORM_WIDTH >= pPlatforms[i-1]->xPos && x <= pPlatforms[i-1]->xPos + PLATFORM_WIDTH){
                j--;
            }
            else{
                pPlatforms[i] = createPlatform(x, y, width, height);

                if(j != Y_NR_OF_PLATFORMS - 1) i++;
            }
        }
    }
    }

    i = 0;
    while (pPlatforms[i]) {
        renderPlatform(pPlatforms[i], pRenderer, pTexture);
        if(isHost)
            scrollPlatform(pPlatforms[i]);
        i++;
    }
}

void handleStartPlatform(Platform* pStartPlatform, Platform* pFirstPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowHeight, int* pTime){
    (*pTime)++;

    if (pStartPlatform->yPos < windowHeight) {
        if (pFirstPlatform && pFirstPlatform->yPos == pStartPlatform->yPos - pFirstPlatform->height) {
            pStartPlatform->yPos += PLATFORM_SPEED;
        }
        renderPlatform(pStartPlatform, pRenderer, pTexture);
    }
}

void resetPlatforms(Platform** pPlatforms){
    for(int i = 0; pPlatforms[i] != 0; i++){
        pPlatforms[i] = 0;
    }
}

void resetStartPlatform(Platform* pStartPlatform, int windowHeight, int* pTime){
    pStartPlatform->yPos = windowHeight - 100;
    *pTime = 0;
}

void destroyPlatform(Platform* pPlatform){
    if (pPlatform) {
        free(pPlatform);
    }
}

void destroyPlatforms(Platform** pPlatforms){
    int i;

    for (i = 0; pPlatforms[i] != 0; i++) {
        destroyPlatform(pPlatforms[i]);
    }
}
