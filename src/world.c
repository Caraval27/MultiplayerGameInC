#include "../include/main.h"

SDL_Texture* createPicture(GameDisplay* pGameDisplay, char picture[]) {
    SDL_Surface *pSurface = IMG_Load(picture);
    if (!pSurface) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pGameDisplay->pRenderer);
        SDL_DestroyWindow(pGameDisplay->pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pGameDisplay->pRenderer, pSurface);
    if (!pTexture) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pGameDisplay->pRenderer);
        SDL_DestroyWindow(pGameDisplay->pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_FreeSurface(pSurface);

    return pTexture;
}

void destroyTexture(SDL_Texture* pTexture) {
    if (pTexture) {
        SDL_DestroyTexture(pTexture);
    }
}

Background* createBackground(int windowHeight) {
    Background* pBackground = malloc(sizeof(Background));

    pBackground->upperSrcYPos = BACKGROUND_HEIGHT;
    pBackground->upperSrcHeight = 0;
    pBackground->lowerSrcYPos = BACKGROUND_HEIGHT - windowHeight;
    pBackground->lowerSrcHeight = windowHeight;
    pBackground->upperDstYPos = 0;
    pBackground->upperDstHeight = 0;
    pBackground->lowerDstYPos = 0;
    pBackground->lowerDstHeight = windowHeight;

    return pBackground;
}

void handleBackground(Background* pBackground, GameDisplay* pGameDisplay, SDL_Texture* pTexture) {

    if (pBackground->lowerSrcYPos < 0) {
        pBackground->upperSrcYPos -= BACKGROUND_SPEED;
        pBackground->upperSrcHeight += BACKGROUND_SPEED;
        pBackground->lowerSrcHeight -= BACKGROUND_SPEED;
        pBackground->upperDstHeight += BACKGROUND_SPEED;
        pBackground->lowerDstYPos += BACKGROUND_SPEED;
        pBackground->lowerDstHeight -= BACKGROUND_SPEED;
    } else {
        pBackground->lowerSrcYPos -= BACKGROUND_SPEED;
    }

    if (pBackground->lowerSrcHeight < 0) {
        pBackground->upperSrcYPos = BACKGROUND_HEIGHT;
        pBackground->upperSrcHeight = 0;
        pBackground->lowerSrcYPos = BACKGROUND_HEIGHT - pGameDisplay->windowHeight;
        pBackground->lowerSrcHeight = pGameDisplay->windowHeight;
        pBackground->upperDstYPos = 0;
        pBackground->upperDstHeight = 0;
        pBackground->lowerDstYPos = 0;
        pBackground->lowerDstHeight = pGameDisplay->windowHeight;
    }

    renderBackground(pBackground, pGameDisplay->pRenderer, pTexture, pGameDisplay->windowWidth);
}

void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth) {
    SDL_Rect upperSrcRect = {0, pBackground->upperSrcYPos, windowWidth, pBackground->upperSrcHeight};
    SDL_Rect lowerSrcRect = {0, pBackground->lowerSrcYPos, windowWidth, pBackground->lowerSrcHeight};
    SDL_Rect upperDstRect = {0, pBackground->upperDstYPos, windowWidth, pBackground->upperDstHeight};
    SDL_Rect lowerDstRect = {0, pBackground->lowerDstYPos, windowWidth, pBackground->lowerDstHeight};

    SDL_RenderCopy(pRenderer, pTexture, &upperSrcRect, &upperDstRect);
    SDL_RenderCopy(pRenderer, pTexture, &lowerSrcRect, &lowerDstRect);
}

void destroyBackground(Background* pBackground) {
    if (pBackground) {
        free(pBackground);
    }
}

void destroyMusic(Mix_Music* pMusic) {
    if (pMusic) {
        Mix_FreeMusic(pMusic);
    }
}

void destroyChunk(Mix_Chunk* pChunk) {
    if (pChunk) {
        Mix_FreeChunk(pChunk);
    }
}