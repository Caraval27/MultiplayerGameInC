#include "../include/main.h"

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
    srand(time(NULL));
    FILE *fp;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init() != 0){
        printf("Error: %s\n", TTF_GetError());
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
    if (!pGame->pWindow){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (!pGame->pRenderer){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pMainMenuFont = TTF_OpenFont("../assets/Ticketing.ttf", 25);
    if (!pGame->pMainMenuFont){
        printf("Error: %s\n",TTF_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Quit", pGame->windowWidth, pGame->windowHeight, 100);
    pGame->pBackgroundTexture = initBackground(pGame->pWindow, pGame->pRenderer, &pGame->windowUpperRect, &pGame->windowLowerRect, &pGame->imageUpperRect, &pGame->imageLowerRect, pGame->windowWidth, pGame->windowHeight);
    if (!pGame->pBackgroundTexture){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pMenuBackgroundTexture = initMenuBackground(pGame->pWindow, pGame->pRenderer, &pGame->menuBackgroundRect, pGame->windowWidth, pGame->windowHeight);
    if (!pGame->pMenuBackgroundTexture){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }

    createPlayerRect(&pGame->playerRect, pGame->windowWidth, pGame->windowHeight);
    pGame->pPlayer = createPlayer((pGame->windowWidth - pGame->playerRect.w) / 2, pGame->windowHeight - pGame->playerRect.h);

    readFromFile(fp, pGame->keybinds);
    saveToFile(fp, pGame->keybinds);
    pGame->state = MAIN_MENU;

    return 1;
}

void runGame(Game *pGame){
    bool isRunning = true, left = false, right = false;
    float currentPlatformY = 0, maxJumpHeight = MAX_JUMP_HEIGHT;
    int mousePos;
    SDL_Event event;

    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU:
                while (SDL_PollEvent(&event)){
                    renderMenuBackground(pGame->pRenderer, pGame->pMenuBackgroundTexture, pGame->menuBackgroundRect);
                    pGame->pQuitButton = createButton(&pGame->quitButtonRect, &mousePos, pGame->windowWidth, pGame->windowHeight, 100);
                    handleButtonInput(pGame->pQuitButton, &isRunning, mousePos);
                    if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) isRunning = false;
                    else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE) pGame->state = ONGOING;
                    renderButton(pGame->pRenderer, pGame->quitButtonRect, 138, 43, 226);
                    renderText(pGame->pQuitButtonText);
                }
                SDL_RenderPresent(pGame->pRenderer);
            break;
            case SETTINGS_MENU:

            break;
            case ONGOING:
                while (SDL_PollEvent(&event)){
                    handleInputOngoing(&event, &right, &left, &isRunning);
                }
                movePlayer(pGame->pPlayer, pGame->playerRect, left, right, pGame->windowWidth);
                platformCollidePlayer(pGame->pPlayer, pGame->playerRect, pGame->planks, &currentPlatformY, &maxJumpHeight);
                jumpPlayer(pGame->pPlayer, pGame->playerRect, pGame->windowHeight, currentPlatformY, maxJumpHeight);

                updateBackground(&pGame->windowUpperRect, &pGame->windowLowerRect, &pGame->imageUpperRect, &pGame->imageLowerRect, pGame->windowHeight, pGame->pRenderer, pGame->pBackgroundTexture);
                updatePlayer(pGame->pPlayer, &pGame->playerRect);
                //updatePlatform(pGame->pPlatform, &pGame->platformRect);

                SDL_SetRenderDrawColor(pGame->pRenderer, 0, 0, 255, 255);
                SDL_RenderFillRect(pGame->pRenderer, &pGame->playerRect);
                SDL_SetRenderDrawColor(pGame->pRenderer, 0, 255, 0, 255);
                SDL_RenderFillRect(pGame->pRenderer, &pGame->platformRect);
                
                handlePlank(pGame->planks, pGame->pRenderer, pGame->windowWidth);

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
    if (pGame->pPlayer){
        destroyPlayer(pGame->pPlayer);
    }
    if (pGame->pPlatform){
        destroyPlatform(pGame->pPlatform);
    }
    destroyPlank(pGame->planks);
    if (pGame->pMenuBackgroundTexture){
        SDL_DestroyTexture(pGame->pMenuBackgroundTexture);
    }
    if (pGame->pBackgroundTexture){
        SDL_DestroyTexture(pGame->pBackgroundTexture);
    }
    if (pGame->pBackgroundTexture){
        destroyText(pGame->pQuitButtonText);
    }
    if (pGame->pRenderer){
        SDL_DestroyRenderer(pGame->pRenderer);
    }
    if (pGame->pWindow){
        SDL_DestroyWindow(pGame->pWindow);
    }
    TTF_Quit();
    SDL_Quit();
}

void handleInputOngoing(SDL_Event* event, bool* right, bool* left, bool* isRunning){

    switch (event->type){
        case SDL_QUIT:
            *isRunning = false;
        break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym){
                case SDLK_ESCAPE:
                    *isRunning = false;
                    break;
                case SDLK_RIGHT:
                    *right = true;
                    break;
                case SDLK_LEFT:
                    *left = true;
                    break;
            }
        break;
        case SDL_KEYUP:
            switch(event->key.keysym.sym){
                case SDLK_LEFT:
                    *left = false;
                    break;
                case SDLK_RIGHT:
                    *right = false;
                    break;
            }
        break;
    }
}