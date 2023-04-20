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
    pGame->pPlayer1Texture = createPlayerCharacter(pGame->pRenderer, pGame->pWindow);
    if (!pGame->pPlayer1Texture){
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
    pGame->pStartButton = createButton(&pGame->startButtonRect, pGame->windowHeight, pGame->windowWidth, 50);
    pGame->pQuitButton = createButton(&pGame->quitButtonRect, pGame->windowHeight, pGame->windowWidth, 100);
    pGame->pResumeButton = createButton(&pGame->resumeButtonRect, pGame->windowHeight, pGame->windowWidth, 50);
    pGame->pMainMenuButton = createButton(&pGame->mainMenuButtonRect, pGame->windowHeight, pGame->windowWidth, 100);
    pGame->pStartButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Start game", pGame->windowWidth, pGame->windowHeight, 50);
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Quit", pGame->windowWidth, pGame->windowHeight, 100);
    pGame->pResumeButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Resume game", pGame->windowWidth, pGame->windowHeight, 50);
    pGame->pMainMenuButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Main menu", pGame->windowWidth, pGame->windowHeight, 100);
    pGame->pPlayer1 = createPlayer((pGame->windowWidth - pGame->player1Rect.w) / 2, pGame->windowHeight - pGame->player1Rect.h, &pGame->player1Rect, pGame->windowWidth, pGame->windowHeight);
    pGame->pPlayer2 = createPlayer((pGame->windowWidth - pGame->player2Rect.w) / 2, pGame->windowHeight - pGame->player2Rect.h, &pGame->player2Rect, pGame->windowWidth, pGame->windowHeight);


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
                    mousePos = getMousePos(pGame->startButtonRect, mousePos, pGame->pStartButton);
                    handleButtonInput(pGame->pStartButton, mousePos, event, &pGame->state, ONGOING);
                    mousePos = getMousePos(pGame->quitButtonRect, mousePos, pGame->pQuitButton);
                    handleButtonInput(pGame->pQuitButton, mousePos, event, &pGame->state, QUIT);
                    renderMainMenu(pGame->pRenderer, pGame->pMainMenuTexture, pGame->mainMenuRect);
                    renderButton(pGame->pRenderer, pGame->startButtonRect, 250, 43, 226);
                    renderButton(pGame->pRenderer, pGame->quitButtonRect, 138, 43, 226);
                    renderText(pGame->pStartButtonText);
                    renderText(pGame->pQuitButtonText);
                }
            break;
            case SETTINGS_MENU:

            break;
            case ONGOING:
                while (SDL_PollEvent(&event)){
                    handleInputOngoing(&pGame->state, &event, &isRunning, &right, &left);
                }

                movePlayer(pGame->pPlayer1, pGame->player1Rect, left, right, pGame->windowWidth);
                jumpPlayer(pGame->pPlayer1, pGame->player1Rect, pGame->windowHeight, currentPlatformY, maxJumpHeight);
                platformCollidePlayer(pGame->pPlayer1, pGame->player1Rect, pGame->platforms, &currentPlatformY, &maxJumpHeight);
            
                handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight);
                renderPlayer(pGame->pRenderer, pGame->pPlayer1Texture, pGame->pPlayer1, &pGame->player1Rect); //player 1

                pGame->pPlayer2Texture = createPlayerCharacter(pGame->pRenderer, pGame->pWindow); //player 2
                renderPlayer(pGame->pRenderer, pGame->pPlayer2Texture, pGame->pPlayer2, &pGame->player2Rect); //player 2
                handlePlatform(pGame->platforms, pGame->pRenderer, pGame->windowWidth);

                SDL_Delay(1000/60);
            break;
            case GAME_MENU:
                mousePos = getMousePos(pGame->resumeButtonRect, mousePos, pGame->pResumeButton);
                handleButtonInput(pGame->pResumeButton, mousePos, event, &pGame->state, ONGOING);
                mousePos = getMousePos(pGame->mainMenuButtonRect, mousePos, pGame->pMainMenuButton);
                handleButtonInput(pGame->pMainMenuButton, mousePos, event, &pGame->state, MAIN_MENU);
                renderButton(pGame->pRenderer, pGame->mainMenuButtonRect, 138, 43, 226);
                renderButton(pGame->pRenderer, pGame->resumeButtonRect, 250, 43, 226);
                renderText(pGame->pMainMenuButtonText);
                renderText(pGame->pResumeButtonText);
            break;
            case GAME_OVER:

            break;
            case QUIT:
                isRunning = false;
            break;
            default:

            break;
        }
        SDL_RenderPresent(pGame->pRenderer);
    }
}

void quitGame(Game* pGame){
    
    destroyPlatform(pGame->platforms);
    if (pGame->pPlayer1){
        destroyPlayer(pGame->pPlayer1);
    }
    if (pGame->pQuitButtonText){
        destroyText(pGame->pQuitButtonText);
    }
    if (pGame->pStartButtonText){
        destroyText(pGame->pStartButtonText);
    }
    if (pGame->pResumeButtonText){
        destroyText(pGame->pResumeButtonText);
    }
    if (pGame->pMainMenuButtonText){
        destroyText(pGame->pMainMenuButtonText);
    }
    if (pGame->pBackground){
        destroyBackground(pGame->pBackground);
    }
    if (pGame->pPlayer1Texture){
        SDL_DestroyTexture(pGame->pPlayer1Texture);
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