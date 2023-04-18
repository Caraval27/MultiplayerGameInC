#include "../include/main.h"

SDL_Texture * initBackground(SDL_Window *pWindow, SDL_Renderer *pRenderer, SDL_Rect *pWindowUpperRect, SDL_Rect *pWindowLowerRect, SDL_Rect *pBackgroundUpperRect, SDL_Rect *pBackgroundLowerRect, int windowWidth, int windowHeight){
    SDL_Surface *pSurface = IMG_Load("../assets/background.png");
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
    
    pWindowUpperRect->x = 0;
    pWindowUpperRect->y = 0;
    pWindowUpperRect->w = windowWidth;
    pWindowUpperRect->h = 0;
    pWindowLowerRect->x = 0;
    pWindowLowerRect->y = 0;
    pWindowLowerRect->w = windowWidth;
    pWindowLowerRect->h = windowHeight;
    pBackgroundUpperRect->x = 0;
    pBackgroundUpperRect->y = IMAGE_HEIGHT;
    pBackgroundUpperRect->w = windowWidth;
    pBackgroundUpperRect->h = 0;
    pBackgroundLowerRect->x = 0;
    pBackgroundLowerRect->y = IMAGE_HEIGHT - windowHeight;
    pBackgroundLowerRect->w = windowWidth;
    pBackgroundLowerRect->h = windowHeight;

    return pTexture;
}

void updateBackground(SDL_Rect* pWindowUpperRect, SDL_Rect* pWindowLowerRect, SDL_Rect* pBackgroundUpperRect, SDL_Rect* pBackgroundLowerRect, int windowHeight, SDL_Renderer* pRenderer, SDL_Texture* pTexture){

    if (pBackgroundLowerRect->y < 0) {
        pWindowUpperRect->h += 1;
        pWindowLowerRect->y += 1;
        pWindowLowerRect->h -= 1;
        pBackgroundUpperRect->y -= 1;
        pBackgroundUpperRect->h += 1;
        pBackgroundLowerRect->h -= 1;
    }
    else{
        pBackgroundLowerRect->y -= 1;
    }

    if (pBackgroundLowerRect->h < 0) {
        pWindowUpperRect->y = 0;
        pWindowUpperRect->h = 0;
        pWindowLowerRect->y = 0;
        pWindowLowerRect->h = windowHeight;
        pBackgroundUpperRect->y = IMAGE_HEIGHT;
        pBackgroundUpperRect->h = 0;
        pBackgroundLowerRect->y = IMAGE_HEIGHT - windowHeight;
        pBackgroundLowerRect->h = windowHeight;
    }

    SDL_RenderCopy(pRenderer, pTexture, pBackgroundUpperRect, pWindowUpperRect);
    SDL_RenderCopy(pRenderer, pTexture, pBackgroundLowerRect, pWindowLowerRect);
}