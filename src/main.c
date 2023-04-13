#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/platforms.h"
#include "../include/keybinds.h"
#include "../include/players.h"

#define NR_OF_KEYBINDS 3

int main(int argv, char** args){
    SDL_DisplayMode displayMode;
    int w, h, keybinds[NR_OF_KEYBINDS];
    SDL_Rect windowUpper, windowLower, imageUpper, imageLower;
    FILE *fp;
    bool isRunning = true;
    SDL_Event event;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }
    w = displayMode.w;
    h = displayMode.h;

    SDL_Window* pWindow = SDL_CreateWindow("totally not a doodle jump clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    if(!pWindow){
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
    if(!pRenderer){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *pBackgroundTexture = initBackground(pWindow, pRenderer, &windowUpper, &windowLower, &imageUpper, &imageLower, w, h);
    

    SDL_Rect playerRect = {50,50,50,50};

    readFromFile(fp, keybinds);
    saveToFile(fp, keybinds);

    while (isRunning){
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    isRunning = false;
                break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_UP){
                        isRunning = false;
                    }
            }
        }

        SDL_RenderClear(pRenderer);
   
        scrollBackground(&windowUpper, &windowLower, &imageUpper, &imageLower, h, pRenderer, pBackgroundTexture);
        
        SDL_SetRenderDrawColor(pRenderer, 0,255,0,255);
        SDL_RenderFillRect(pRenderer, &playerRect);

        SDL_RenderPresent(pRenderer);

        SDL_Delay(10);
    }

    SDL_DestroyTexture(pBackgroundTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}