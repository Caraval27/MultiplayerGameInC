#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/platforms.h"

SDL_Texture * initBackground(SDL_Window *pWindow, SDL_Renderer *pRenderer, SDL_Rect *pWindowUpper, SDL_Rect *pWindowLower, SDL_Rect *pImageUpper, SDL_Rect *pImageLower, int w, int h){
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
    pWindowUpper->x = 0;
    pWindowUpper->y = 0;
    pWindowUpper->w = w;
    pWindowUpper->h = 0;
    pWindowLower->x = 0;
    pWindowLower->y = 0;
    pWindowLower->w = w;
    pWindowLower->h = h;
    pImageUpper->x = 0;
    pImageUpper->y = IMAGE_HEIGHT;
    pImageUpper->w = w;
    pImageUpper->h = 0;
    pImageLower->x = 0;
    pImageLower->y = IMAGE_HEIGHT - h;
    pImageLower->w = w;
    pImageLower->h = h;

    return pTexture;
}

void scrollBackground(SDL_Rect *pWindowUpper, SDL_Rect *pWindowLower, SDL_Rect *pImageUpper, SDL_Rect *pImageLower, int h, SDL_Renderer *pRenderer, SDL_Texture *pTexture){

    if(pImageLower->y < 0){
        pWindowUpper->h += 1;
        pWindowLower->y += 1;
        pWindowLower->h -= 1;
        pImageUpper->y -= 1;
        pImageUpper->h += 1;
        pImageLower->h -= 1;
    }
    else{
        pImageLower->y -= 1;
    }

    if(pImageLower->h < 0){
        pWindowUpper->y = 0;
        pWindowUpper->h = 0;
        pWindowLower->y = 0;
        pWindowLower->h = h;
        pImageUpper->y = IMAGE_HEIGHT;
        pImageUpper->h = 0;
        pImageLower->y = IMAGE_HEIGHT - h;
        pImageLower->h = h;
    }

    SDL_RenderCopy(pRenderer, pTexture, pImageUpper, pWindowUpper);
    SDL_RenderCopy(pRenderer, pTexture, pImageLower, pWindowLower);
}