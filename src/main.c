#include "../include/main.h"

int main(int argv, char** args){
    Game game = {0};

    if (!initiateGame(&game)){
        return 1;
    }
    runGame(&game);
    quitGame(&game);

    return 0;
}

int initiateGame(Game* pGame){
    srand(time(NULL));

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

    pGame->pWindow = SDL_CreateWindow("Totally not a doodle jump clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->windowWidth, pGame->windowHeight, 0);
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
    pGame->pMainMenuTexture = createMainMenuImage(pGame->pWindow, pGame->pRenderer, &pGame->mainMenuRect, pGame->windowWidth, pGame->windowHeight);
    if (!pGame->pMainMenuTexture){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pBackgroundTexture = createBackgroundImage(pGame->pWindow, pGame->pRenderer);
    if (!pGame->pBackgroundTexture){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->pPlayerTexture = createPlayerCharacter(&pGame->playerRect, pGame->pRenderer, pGame->pWindow, pGame->windowWidth, pGame->windowHeight);
    if (!pGame->pPlayerTexture){
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

    pGame->pBackground = createBackground(pGame->windowHeight);
    pGame->pQuitButton = createButton(&pGame->quitButtonRect, pGame->windowHeight, pGame->windowWidth, 100);
    pGame->pStartButton = createButton(&pGame->startButtonRect, pGame->windowHeight, pGame->windowWidth, 50);
    pGame->pStartButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Start Game", pGame->windowWidth, pGame->windowHeight, 50);
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Quit", pGame->windowWidth, pGame->windowHeight, 100);
    pGame->pPlayer = createPlayer((pGame->windowWidth - pGame->playerRect.w) / 2, pGame->windowHeight - pGame->playerRect.h);

    FILE *fp;
    readFromFile(fp, pGame->keybinds);
    saveToFile(fp, pGame->keybinds);

    pGame->state = MAIN_MENU;

    

    return 1;
}

void runGame(Game* pGame){
    bool isRunning = true, left = false, right = false;
    SDL_Event event;
    int mousePos;
    float currentPlatformY = 0, maxJumpHeight = MAX_JUMP_HEIGHT;

    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU:
                while (SDL_PollEvent(&event)){
                    mousePos = getMousePos(&pGame->quitButtonRect, mousePos, pGame->windowWidth, pGame->windowHeight, 100, pGame->pQuitButton);
                    handleButtonInput(pGame->pStartButton, mousePos, event, &pGame->state, ONGOING);
                    handleButtonInput(pGame->pQuitButton, mousePos, event, &pGame->state, QUIT);
                    renderMainMenu(pGame->pRenderer, pGame->pMainMenuTexture, pGame->mainMenuRect);
                    renderButton(pGame->pRenderer, pGame->quitButtonRect, 138, 43, 226);
                    renderButton(pGame->pRenderer, pGame->startButtonRect, 250, 43, 226);
                    renderText(pGame->pQuitButtonText);
                    renderText(pGame->pStartButtonText);
                }
            break;
            case SETTINGS_MENU:

            break;
            case ONGOING:
                while (SDL_PollEvent(&event)){
                    handleInputOngoing(&pGame->state, &event, &isRunning, &right, &left);
                }

                movePlayer(pGame->pPlayer, pGame->playerRect, left, right, pGame->windowWidth);
                jumpPlayer(pGame->pPlayer, pGame->playerRect, pGame->windowHeight, currentPlatformY, maxJumpHeight);
                platformCollidePlayer(pGame->pPlayer, pGame->playerRect, pGame->platforms, &currentPlatformY, &maxJumpHeight);
            
                handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight);
                renderPlayer(pGame->pRenderer, pGame->pPlayerTexture, pGame->pPlayer, &pGame->playerRect);
                handlePlatform(pGame->platforms, pGame->pRenderer, pGame->windowWidth);

                SDL_Delay(1000/60);
            break;
            case GAME_MENU:
                /*handleButtonInput();
                renderButton();
                renderText();*/
            break;
            case GAME_OVER:
                
            break;
            case QUIT:
                isRunning = false;
            break;
        }
        SDL_RenderPresent(pGame->pRenderer);
    }
}

void quitGame(Game *pGame){
    
    if (pGame->platforms){
        destroyPlatform(pGame->platforms);
    }
    if (pGame->pPlayer){
        destroyPlayer(pGame->pPlayer);
    }
    if (pGame->pQuitButtonText){
        destroyText(pGame->pQuitButtonText);
    }
    if (pGame->pBackground){
        destroyBackground(pGame->pBackground);
    }
    if (pGame->pPlayerTexture){
        SDL_DestroyTexture(pGame->pPlayerTexture);
    }
    if (pGame->pBackgroundTexture){
        SDL_DestroyTexture(pGame->pBackgroundTexture);
    }
    if (pGame->pMainMenuTexture){
        SDL_DestroyTexture(pGame->pMainMenuTexture);
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

void handleInputOngoing(State* pState, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft){
    switch (event->type){
        case SDL_QUIT: *pIsRunning = false;
        break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym){
                case SDLK_ESCAPE: *pState = GAME_MENU;
                    break;
                case SDLK_RIGHT: *pRight = true;
                    break;
                case SDLK_LEFT: *pLeft = true;
                    break;
            }
        break;
        case SDL_KEYUP:
            switch(event->key.keysym.sym){
                case SDLK_LEFT: *pLeft = false;
                    break;
                case SDLK_RIGHT: *pRight = false;
                    break;
            }
        break;
    }
}