#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "platforms.h"

SDL_Texture * initBackground(SDL_Window *pWindow, SDL_Renderer *pRenderer){
    SDL_Surface *pSurface = IMG_Load("resources/background.png");
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

    return pTexture;
}

void scrollBackground(SDL_Renderer *pRenderer, SDL_Texture *pTexture){
    static SDL_Rect windowUpper = {0, 0, WINDOW_WIDTH, 0};
    static SDL_Rect windowLower = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    static SDL_Rect imageUpper = {0, IMAGE_HEIGHT, WINDOW_WIDTH, 0};
    static SDL_Rect imageLower = {0, IMAGE_HEIGHT - WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT};

    if(imageLower.y < 0){
        windowUpper.h += 1;
        windowLower.y += 1;
        windowLower.h -= 1;
        imageUpper.y -= 1;
        imageUpper.h += 1;
        imageLower.h -= 1;
    }
    else{
        imageLower.y -= 1;
    }

    if(imageLower.h < 0){
        windowUpper.y = 0;
        windowUpper.h = 0;
        windowLower.y = 0;
        windowLower.h = WINDOW_HEIGHT;
        imageUpper.y = IMAGE_HEIGHT;
        imageUpper.h = 0;
        imageLower.y = IMAGE_HEIGHT - WINDOW_HEIGHT;
        imageLower.h = WINDOW_HEIGHT;
    }

    SDL_RenderCopy(pRenderer, pTexture, &imageUpper, &windowUpper);
    SDL_RenderCopy(pRenderer, pTexture, &imageLower, &windowLower);
}