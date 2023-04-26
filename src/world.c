#include "../include/main.h"

SDL_Texture* createPicture(SDL_Window* pWindow, SDL_Renderer* pRenderer, char picture[]){
    SDL_Surface *pSurface = IMG_Load(picture);
    if (!pSurface) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if (!pTexture) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_FreeSurface(pSurface);

    return pTexture;
}

Background* createBackground(int windowHeight){
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

void handleBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight){

    if (pBackground->lowerSrcYPos < 0) {
        pBackground->upperSrcYPos -= BACKGROUND_SPEED;
        pBackground->upperSrcHeight += BACKGROUND_SPEED;
        pBackground->lowerSrcHeight -= BACKGROUND_SPEED;
        pBackground->upperDstHeight += BACKGROUND_SPEED;
        pBackground->lowerDstYPos += BACKGROUND_SPEED;
        pBackground->lowerDstHeight -= BACKGROUND_SPEED;
    }
    else {
        pBackground->lowerSrcYPos -= BACKGROUND_SPEED;
    }

    if (pBackground->lowerSrcHeight < 0) {
        pBackground->upperSrcYPos = BACKGROUND_HEIGHT;
        pBackground->upperSrcHeight = 0;
        pBackground->lowerSrcYPos = BACKGROUND_HEIGHT - windowHeight;
        pBackground->lowerSrcHeight = windowHeight;
        pBackground->upperDstYPos = 0;
        pBackground->upperDstHeight = 0;
        pBackground->lowerDstYPos = 0;
        pBackground->lowerDstHeight = windowHeight;
    }

    renderBackground(pBackground, pRenderer, pTexture, windowWidth);
}

void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth){
    SDL_Rect upperSrcRect = {0, pBackground->upperSrcYPos, windowWidth, pBackground->upperSrcHeight};
    SDL_Rect lowerSrcRect = {0, pBackground->lowerSrcYPos, windowWidth, pBackground->lowerSrcHeight};
    SDL_Rect upperDstRect = {0, pBackground->upperDstYPos, windowWidth, pBackground->upperDstHeight};
    SDL_Rect lowerDstRect = {0, pBackground->lowerDstYPos, windowWidth, pBackground->lowerDstHeight};

    SDL_RenderCopy(pRenderer, pTexture, &upperSrcRect, &upperDstRect);
    SDL_RenderCopy(pRenderer, pTexture, &lowerSrcRect, &lowerDstRect);
}

void destroyBackground(Background* pBackground){
    free(pBackground);
}

void destroyMusic(Mix_Music* pMusic){
    Mix_FreeMusic(pMusic);
    Mix_CloseAudio();
}

void destroyChunk(Mix_Chunk* pChunk){
    Mix_FreeChunk(pChunk);
    Mix_CloseAudio();
}

void handleStartPlatform(Platform* pStartPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowHeight, int* pTime){
    (*pTime)++;

    if (pStartPlatform->yPos < windowHeight) {
        if (*pTime > 500) { 
            pStartPlatform->yPos += PLATFORM_SPEED;
        }
        renderPlatform(pStartPlatform, pRenderer, pTexture);
    }
}

void resetStartPlatform(Platform* pStartPlatform, int windowHeight, int* pTime){
    pStartPlatform->yPos = windowHeight - 100;
    *pTime = 0;
}