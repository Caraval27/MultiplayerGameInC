#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/world.h"

SDL_Texture * initBackground(SDL_Window *pWindow, SDL_Renderer *pRenderer, SDL_Rect *pWindowUpperRect, SDL_Rect *pWindowLowerRect, SDL_Rect *pImageUpperRect, SDL_Rect *pImageLowerRect, int w, int h){
    SDL_Surface *pSurface = IMG_Load("../assets/background.png");
    if(!pSurface){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if(!pTexture){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    pWindowUpperRect->x = 0;
    pWindowUpperRect->y = 0;
    pWindowUpperRect->w = w;
    pWindowUpperRect->h = 0;
    pWindowLowerRect->x = 0;
    pWindowLowerRect->y = 0;
    pWindowLowerRect->w = w;
    pWindowLowerRect->h = h;
    pImageUpperRect->x = 0;
    pImageUpperRect->y = IMAGE_HEIGHT;
    pImageUpperRect->w = w;
    pImageUpperRect->h = 0;
    pImageLowerRect->x = 0;
    pImageLowerRect->y = IMAGE_HEIGHT - h;
    pImageLowerRect->w = w;
    pImageLowerRect->h = h;

    return pTexture;
}

void updateBackground(SDL_Rect* pWindowUpperRect, SDL_Rect* pWindowLowerRect, SDL_Rect* pImageUpperRect, SDL_Rect* pImageLowerRect, int h, SDL_Renderer* pRenderer, SDL_Texture* pBackgroundTexture){

    if(pImageLowerRect->y < 0){
        pWindowUpperRect->h += 1;
        pWindowLowerRect->y += 1;
        pWindowLowerRect->h -= 1;
        pImageUpperRect->y -= 1;
        pImageUpperRect->h += 1;
        pImageLowerRect->h -= 1;
    }
    else{
        pImageLowerRect->y -= 1;
    }

    if(pImageLowerRect->h < 0){
        pWindowUpperRect->y = 0;
        pWindowUpperRect->h = 0;
        pWindowLowerRect->y = 0;
        pWindowLowerRect->h = h;
        pImageUpperRect->y = IMAGE_HEIGHT;
        pImageUpperRect->h = 0;
        pImageLowerRect->y = IMAGE_HEIGHT - h;
        pImageLowerRect->h = h;
    }

    SDL_RenderCopy(pRenderer, pBackgroundTexture, pImageUpperRect, pWindowUpperRect);
    SDL_RenderCopy(pRenderer, pBackgroundTexture, pImageLowerRect, pWindowLowerRect);
}