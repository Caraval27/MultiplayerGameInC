#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "../include/world.h"
#include "../include/keybinds.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"

typedef enum {
    MAIN_MENU,
	SETTINGS_MENU,
	ONGOING,
	GAME_MENU,
	GAME_OVER
} GameState;

typedef struct {
    int windowWidth, windowHeight, keybinds[NR_OF_KEYBINDS];
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *pBackgroundTexture, *pMenuBackgroundTexture;
    SDL_Rect windowUpperRect, windowLowerRect, imageUpperRect, imageLowerRect;
    Player* pPlayer;
    Platform* pPlatform;
	Platform **platforms;
    SDL_Rect playerRect, platformRect;
    GameState state;

    SDL_Rect menuBackgroundRect;

} Game;

int initiateGraphics(Game *pGame);
void runGame(Game *pGame);
void quitGame(Game *pGame);

int main(int argv, char** args){
    Game game = {0};
    if (!initiateGraphics(&game)){
        return 1;
    }
    runGame(&game);
    quitGame(&game);
    return 0;
}

int initiateGraphics(Game *pGame){
    srand(time(0));
    FILE *fp;
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->windowWidth = displayMode.w;
    pGame->windowHeight = displayMode.h;
    pGame->pWindow = SDL_CreateWindow("totally not a doodle jump clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->windowWidth, pGame->windowHeight, 0);
    if(!pGame->pWindow){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!pGame->pRenderer){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pBackgroundTexture = initBackground(pGame->pWindow, pGame->pRenderer, &pGame->windowUpperRect, &pGame->windowLowerRect, &pGame->imageUpperRect, &pGame->imageLowerRect, pGame->windowWidth, pGame->windowHeight);
    if(!pGame->pBackgroundTexture){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pMenuBackgroundTexture = initMenuBackground(pGame->pWindow, pGame->pRenderer, &pGame->menuBackgroundRect, pGame->windowWidth, pGame->windowHeight);
    if(!pGame->pMenuBackgroundTexture){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }

    pGame->playerRect.x = pGame->windowWidth - pGame->playerRect.w;
    pGame->playerRect.y = pGame->windowHeight - pGame->playerRect.h;
    pGame->playerRect.w = 50;
    pGame->playerRect.h = 50;
    pGame->pPlayer = createPlayer((pGame->windowWidth - pGame->playerRect.w) / 2, pGame->windowHeight - pGame->playerRect.h);

    pGame->platformRect.x = pGame->windowWidth;
    pGame->platformRect.y = 50;
    pGame->platformRect.w = PLATFORM_WIDTH;
    pGame->platformRect.h = PLATFORM_HEIGHT;
    pGame->pPlatform = createPlatform(pGame->windowWidth, pGame->windowHeight - 200);

    readFromFile(fp, pGame->keybinds);
    saveToFile(fp, pGame->keybinds);
    pGame->state = MAIN_MENU;

    return 1;
}

void runGame(Game *pGame){
    bool isRunning = true, left = false, right = false;
    float currentPlatformY = 0, maxJumpHeight = 400;
    SDL_Event event;

    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU:
                while (SDL_PollEvent(&event)){
                    renderMenuBackground(pGame->pRenderer, pGame->pMenuBackgroundTexture, pGame->menuBackgroundRect);
                    if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) isRunning = false;
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                        /* resetAsteroids(pGame);
                        resetRocket(pGame->pRocket);
                        pGame->startTime = SDL_GetTicks64();
                        pGame->gameTime = -1; */
                        pGame->state = ONGOING;
                    }
                }
            break;
            case SETTINGS_MENU:

            break;
            case ONGOING:
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
                movePlayer(pGame->pPlayer, pGame->playerRect, left, right, pGame->windowWidth);
                platformCollidePlayer(pGame->pPlayer, pGame->playerRect, pGame->platformRect, 1, &currentPlatformY, &maxJumpHeight);
                jumpPlayer(pGame->pPlayer, pGame->playerRect, pGame->windowHeight, currentPlatformY, maxJumpHeight);

                updateBackground(&pGame->windowUpperRect, &pGame->windowLowerRect, &pGame->imageUpperRect, &pGame->imageLowerRect, pGame->windowHeight, pGame->pRenderer, pGame->pBackgroundTexture);
                updatePlayer(pGame->pPlayer, &pGame->playerRect);
                updatePlatform(pGame->pPlatform, &pGame->platformRect);

                SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 255, 255);
                SDL_RenderFillRect(pGame->pRenderer, &pGame->playerRect);
                SDL_SetRenderDrawColor(pGame->pRenderer, 0, 255, 0, 255);
                SDL_RenderFillRect(pGame->pRenderer, &pGame->platformRect);

                SDL_RenderPresent(pGame->pRenderer);
                SDL_Delay(1000/60);
            break;
            case GAME_MENU:

            break;
            case GAME_OVER:

            break;
        }
    }
}

void quitGame(Game *pGame){
    if(pGame->pPlayer){
        destroyPlayer(pGame->pPlayer);
    }
    if(pGame->pPlatform){
        destroyPlatform(pGame->pPlatform);
    }
    if(pGame->pMenuBackgroundTexture){
        SDL_DestroyTexture(pGame->pMenuBackgroundTexture);
    }
    if(pGame->pBackgroundTexture){
        SDL_DestroyTexture(pGame->pBackgroundTexture);
    }
    if(pGame->pRenderer){
        SDL_DestroyRenderer(pGame->pRenderer);
    }
    if(pGame->pWindow){
        SDL_DestroyWindow(pGame->pWindow);
    }
    SDL_Quit();
}