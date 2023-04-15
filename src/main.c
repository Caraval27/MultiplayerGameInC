#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "../include/world.h"
#include "../include/keybinds.h"
#include "../include/players.h"
#include "../include/platforms.h"

#define NR_OF_KEYBINDS 3

struct graphics{
    int windowWidth, windowHeight, keybinds[NR_OF_KEYBINDS];
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *pBackgroundTexture;
    SDL_Rect windowUpperRect, windowLowerRect, imageUpperRect, imageLowerRect;
    Player* pPlayer;
    Platform* pPlatform;
    SDL_Rect playerRect, platformRect;
};
typedef struct graphics Graphics;

int initiateGraphics(Graphics *pGraphics);
void runGame(Graphics *pGraphics);
void quitGame(Graphics *pGraphics);

int main(int argv, char** args){
    Graphics g = {0}; 
    if (!initiateGraphics(&g)){
        return 1;
    }
    runGame(&g);
    quitGame(&g);
    return 0;
}

int initiateGraphics(Graphics *pGraphics){
    srand(time(0));
    FILE *fp;
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGraphics);
        return 0;
    }
    pGraphics->windowWidth = displayMode.w;
    pGraphics->windowHeight = displayMode.h;
    pGraphics->pWindow = SDL_CreateWindow("totally not a doodle jump clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGraphics->windowWidth, pGraphics->windowHeight, 0);
    if(!pGraphics->pWindow){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGraphics);
        return 0;
    }
    pGraphics->pRenderer = SDL_CreateRenderer(pGraphics->pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!pGraphics->pRenderer){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGraphics);
        return 0;
    }
    pGraphics->pBackgroundTexture = initBackground(pGraphics->pWindow, pGraphics->pRenderer, &pGraphics->windowUpperRect, &pGraphics->windowLowerRect, &pGraphics->imageUpperRect, &pGraphics->imageLowerRect, pGraphics->windowWidth, pGraphics->windowHeight);
    if(!pGraphics->pBackgroundTexture){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGraphics);
        return 0;
    }
    
    pGraphics->playerRect.x = pGraphics->windowWidth - pGraphics->playerRect.w;
    pGraphics->playerRect.y = pGraphics->windowHeight - pGraphics->playerRect.h;
    pGraphics->playerRect.w = 50;
    pGraphics->playerRect.h = 50;
    pGraphics->pPlayer = createPlayer((pGraphics->windowWidth - pGraphics->playerRect.w) / 2, pGraphics->windowHeight - pGraphics->playerRect.h);
    
    pGraphics->platformRect.x = pGraphics->windowWidth;
    pGraphics->platformRect.y = 50;
    pGraphics->platformRect.w = PLATFORM_WIDTH;
    pGraphics->platformRect.h = PLATFORM_HEIGHT;
    pGraphics->pPlatform = createPlatform(pGraphics->windowWidth, pGraphics->windowHeight - 200);

    readFromFile(fp, pGraphics->keybinds);
    saveToFile(fp, pGraphics->keybinds);
    return 1;
}

void runGame(Graphics *pGraphics){
    bool isRunning = true, left = false, right = false;
    float currentPlatformY = 0, maxJumpHeight = 400;
    SDL_Event event;

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

        movePlayer(pGraphics->pPlayer, pGraphics->playerRect, left, right, pGraphics->windowWidth);
        platformCollidePlayer(pGraphics->pPlayer, pGraphics->playerRect, pGraphics->platformRect, 1, &currentPlatformY, &maxJumpHeight);
        jumpPlayer(pGraphics->pPlayer, pGraphics->playerRect, pGraphics->windowHeight, currentPlatformY, maxJumpHeight);

        updateBackground(&pGraphics->windowUpperRect, &pGraphics->windowLowerRect, &pGraphics->imageUpperRect, &pGraphics->imageLowerRect, pGraphics->windowHeight, pGraphics->pRenderer, pGraphics->pBackgroundTexture);
        updatePlayer(pGraphics->pPlayer, &pGraphics->playerRect);
        updatePlatform(pGraphics->pPlatform, &pGraphics->platformRect);
        
        SDL_SetRenderDrawColor(pGraphics->pRenderer, 0, 0, 255, 255);
        SDL_RenderFillRect(pGraphics->pRenderer, &pGraphics->playerRect);
        SDL_SetRenderDrawColor(pGraphics->pRenderer, 0, 255, 0, 255);
        SDL_RenderFillRect(pGraphics->pRenderer, &pGraphics->platformRect);

        SDL_RenderPresent(pGraphics->pRenderer);
        SDL_Delay(1000/60);
    }
}

void quitGame(Graphics *pGraphics){
    if(pGraphics->pPlayer){
        destroyPlayer(pGraphics->pPlayer);
    }
    if(pGraphics->pPlatform){
        destroyPlatform(pGraphics->pPlatform);
    }
    if(pGraphics->pBackgroundTexture){
        SDL_DestroyTexture(pGraphics->pBackgroundTexture);
    }
    if(pGraphics->pRenderer){
        SDL_DestroyRenderer(pGraphics->pRenderer);
    }
    if(pGraphics->pWindow){
        SDL_DestroyWindow(pGraphics->pWindow);
    } 
    SDL_Quit();
}