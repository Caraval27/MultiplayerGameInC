#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/menu.h"

//menuBackground
/* pGraphics->pMenuBackgroundTexture = initMenuBackground(pGraphics->pWindow, pGraphics->pRenderer);
if(!pGraphics->pMenuBackgroundTexture){
    printf("Error: %s\n", SDL_GetError());
    quitGame(pGraphics);
    return 0;
} */
SDL_Texture* initMenuBackground(SDL_Window *pWindow, SDL_Renderer *pRenderer){
    SDL_Surface *pSurface = IMG_Load("../assets/menuBackground.jpeg");
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
   /*  pWindowUpperRect->x = 0;
    pWindowUpperRect->y = 0;
    pWindowUpperRect->w = windowWidth;
    pWindowUpperRect->h = 0;
    pWindowLowerRect->x = 0;
    pWindowLowerRect->y = 0;
    pWindowLowerRect->w = windowWidth;
    pWindowLowerRect->h = windowHeight;
    pImageUpperRect->x = 0;
    pImageUpperRect->y = IMAGE_HEIGHT;
    pImageUpperRect->w = windowWidth;
    pImageUpperRect->h = 0;
    pImageLowerRect->x = 0;
    pImageLowerRect->y = IMAGE_HEIGHT - windowHeight;
    pImageLowerRect->w = windowWidth;
    pImageLowerRect->h = windowHeight; */

    return pTexture;
}