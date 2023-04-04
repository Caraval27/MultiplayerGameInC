#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/platforms.h"
#include "../include/keybinds.h"

#define NR_OF_KEYBINDS 3

int main(int argv, char** args){
    SDL_DisplayMode displayMode;
    int w, h, keybinds[NR_OF_KEYBINDS];
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

    SDL_Window* pWindow = SDL_CreateWindow("Our game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
    SDL_Texture *pBackgroundTexture = initBackground(pWindow, pRenderer);

    readFromFile(fp, keybinds);
    saveToFile(fp, keybinds);

    while (isRunning){
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    isRunning = false;
            }
        }

        SDL_RenderClear(pRenderer);

        scrollBackground(pRenderer, pBackgroundTexture);

        SDL_RenderPresent(pRenderer);

        SDL_Delay(10);
    }

    SDL_DestroyTexture(pBackgroundTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}