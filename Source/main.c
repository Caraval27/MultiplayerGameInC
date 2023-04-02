#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/keybinds.h"

#define NR_OF_KEYBINDS 3

int main(int argv, char** args){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    FILE *fp;
    int keybinds[NR_OF_KEYBINDS];
    SDL_DisplayMode dm;
    readFromFile(fp, keybinds);
    saveToFile(fp, keybinds);

    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }

    int w, h;
    w = dm.w;
    h = dm.h;

    SDL_Window *window = SDL_CreateWindow("helloworld", 0, 0, w, h, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Font *font = TTF_OpenFont("Montserrat-Black.ttf", 1000);
    SDL_Color colour = {255, 255, 255 };
    SDL_Surface *surface = TTF_RenderText_Solid(font, "Hello World!", colour);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    
    bool isRunning = true;
    SDL_Event event;
    
    while (isRunning){
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    isRunning = false;
            }
        }

        const Uint8* kb = SDL_GetKeyboardState(NULL);
        
        if (kb[keybinds[0]]){
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        } else if (kb[keybinds[1]]){
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        } else if (kb[keybinds[2]]){
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        

        
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}