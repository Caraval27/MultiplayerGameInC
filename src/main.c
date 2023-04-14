#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "../include/world.h"
#include "../include/keybinds.h"
#include "../include/players.h"
#include "../include/platforms.h"

int main(int argv, char** args){
    SDL_DisplayMode displayMode;
    int windowWidth, windowHeight, keybinds[NR_OF_KEYBINDS];
    SDL_Rect windowUpperRect, windowLowerRect, imageUpperRect, imageLowerRect;
    FILE *fp;
    bool isRunning = true, left = false, right = false;
    SDL_Event event;
    float currentPlatformY = 0, maxJumpHeight = 400;
    srand(time(0));

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    SDL_Rect playerRect = {(windowWidth - playerRect.w), (windowHeight - playerRect.h), 50, 50};
    Player* pPlayer = createPlayer((windowWidth - playerRect.w) / 2, windowHeight - playerRect.h);
    Platform* pPlatform = createPlatform(windowWidth, windowHeight - 200);
    SDL_Rect platformRect = {windowWidth, 50, PLATFORM_WIDTH, PLATFORM_HEIGHT};
    Player* plarrr[10];

    SDL_Window* pWindow = SDL_CreateWindow("Totally not a doodle jump clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
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

    SDL_Texture* pBackgroundTexture = initBackground(pWindow, pRenderer, &windowUpperRect, &windowLowerRect, &imageUpperRect, &imageLowerRect, windowWidth, windowHeight);

    readFromFile(fp, keybinds);
    saveToFile(fp, keybinds);

    while (isRunning){
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    isRunning = false;
                break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            isRunning = false;
                           break;
                        case SDLK_RIGHT:
                        //case SDLK_D:
                            right = true;
                            break;
                        case SDLK_LEFT:
                        //case SDLK_A:
                            left = true;
                            break;
                    }   
                break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_LEFT:
                            left = false;
                            //pPlayer->velocityX = 0;
                            break;
                        case SDLK_RIGHT:
                            right = false;
                            //pPlayer->velocityX = 0;
                            break;
                    }
                break;
            }
        }

        movePlayer(pPlayer, playerRect, left, right, windowWidth);
        jumpPlayer(pPlayer, playerRect, windowHeight, currentPlatformY, maxJumpHeight);
        platformCollidePlayer(pPlayer, playerRect, platformRect, 1, &currentPlatformY, &maxJumpHeight);

        updateBackground(&windowUpperRect, &windowLowerRect, &imageUpperRect, &imageLowerRect, windowHeight, pRenderer, pBackgroundTexture);
        updatePlayer(pPlayer, &playerRect);
        updatePlatform(pPlatform, &platformRect);
        
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(pRenderer, &playerRect);
        SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
        SDL_RenderFillRect(pRenderer, &platformRect);

        SDL_RenderPresent(pRenderer);
        SDL_Delay(1000/60);
    }

    SDL_DestroyTexture(pBackgroundTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}