#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/menu.h"

SDL_Texture* initMenuBackground(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pWindowUpperRect, SDL_Rect* pWindowLowerRect, SDL_Rect* pImageUpperRect, SDL_Rect* pImageLowerRect, int windowWidth, int windowHeight){
    SDL_Surface* pSurface = IMG_Load("../assets/menuBackground.jpeg");
    if(!pSurface){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
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
    pWindowUpperRect->w = windowWidth;
    pWindowUpperRect->h = 0;
    pWindowLowerRect->x = 0;
    pWindowLowerRect->y = 0;
    pWindowLowerRect->w = windowWidth;
    pWindowLowerRect->h = windowHeight; 
    pImageUpperRect->x = 0;
    pImageUpperRect->y = MENU_IMAGE_HEIGHT;
    pImageUpperRect->w = windowWidth;
    pImageUpperRect->h = 0;
    pImageLowerRect->x = 0;
    pImageLowerRect->y = MENU_IMAGE_HEIGHT - windowHeight;
    pImageLowerRect->w = windowWidth; //windowWidth;
    pImageLowerRect->h = windowHeight;

    return pTexture;
}

void renderMenuBackground(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect imageUpperRect, SDL_Rect imageLowerRect, SDL_Rect windowUpperRect, SDL_Rect windowLowerRect){
    SDL_RenderCopy(pRenderer, pTexture, &imageUpperRect, &windowUpperRect);
    SDL_RenderCopy(pRenderer, pTexture, &imageLowerRect, &windowLowerRect);
    SDL_RenderPresent(pRenderer);
}