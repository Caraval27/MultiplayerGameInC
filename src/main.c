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

	if (SDLNet_Init() == -1) {
		printf("Error: %s\n", SDLNet_GetError());
		return 0;
	}
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("Error: %s\n", Mix_GetError());
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

    pGame->pNrOfPlayers = MAX_PLAYERS;

    pGame->pWindow = SDL_CreateWindow("Totally not a doodle jump clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->windowWidth, pGame->windowHeight, 0);
    if (!handleError(pGame, pGame->pWindow, SDL_GetError)) return 0;

	Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	#if __APPLE__ || __LINUX__
		flags = (flags & !SDL_RENDERER_ACCELERATED) | SDL_RENDERER_SOFTWARE;
	#endif

    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, flags);
    if (!handleError(pGame, pGame->pRenderer, SDL_GetError)) return 0;

    pGame->pMainMenuTexture = createPicture(pGame->pWindow, pGame->pRenderer, MAIN_MENU_PICTURE);
    if (!handleError(pGame, pGame->pMainMenuTexture, SDL_GetError)) return 0;

    pGame->pBackgroundTexture = createPicture(pGame->pWindow, pGame->pRenderer, BACKGROUND_PICTURE);
    if (!handleError(pGame, pGame->pBackgroundTexture, SDL_GetError)) return 0;

    pGame->pStartPlatformTexture = createPicture(pGame->pWindow, pGame->pRenderer, STARTING_PLATFORM_PICTURE);
    if (!handleError(pGame, pGame->pStartPlatformTexture, SDL_GetError)) return 0;

    //pGame->pPlayerTexture[0] = createPlayerCharacter(pGame->pRenderer, pGame->pWindow, characterPicture1);
    //if (!handleError(pGame, pGame->pPlayer1Texture, SDL_GetError)) return 0;

    //pGame->pPlayerTexture[1] = createPlayerCharacter(pGame->pRenderer, pGame->pWindow, characterPicture2);
    //if (!handleError(pGame, pGame->pPlayer2Texture, SDL_GetError)) return 0;

    pGame->pMainMenuFont = TTF_OpenFont("../assets/Ticketing.ttf", 25);
    if (!handleError(pGame, pGame->pWindow, TTF_GetError)) return 0;

    pGame->pMainSound = Mix_LoadMUS("../assets/MainThemeSoundtrack.mp3");
    if (!handleError(pGame, pGame->pWindow, Mix_GetError)) return 0;

    pGame->pJumpSound = Mix_LoadWAV("../assets/JumpEffect.wav"); //for short sounds
    if (!handleError(pGame, pGame->pWindow, Mix_GetError)) return 0;

    FILE *fp;
    readFromFileKey(fp, pGame->keybinds);
    saveToFile(fp, pGame->keybinds);

    pGame->pBackground = createBackground(pGame->windowHeight);
    pGame->pStartButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pSettingsButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pQuitButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pResumeButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMainMenuButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pLanguageButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 - 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pEnglishButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 - 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pSwedishButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pReturnButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 200, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveRight1Button = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 - 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveLeft1Button = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 - 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveRight2Button = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 + 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveLeft2Button = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 + 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);



    pGame->pGameOverText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Game Over", pGame->windowWidth, pGame->windowHeight, 0, 0);

    pGame->pStartingPlatform = createPlatform(0, pGame->windowHeight - 100, pGame->windowWidth, 100);

    int startPosition = 2;
    for(int i=0; i<pGame->pNrOfPlayers-1; i++){ //m�ste vara -1 annars blir det malloc fel

        pGame->pPlayers[i] = createPlayer(pGame->windowWidth / startPosition, pGame->windowHeight, 60, 60, SPEED, 400);
        pGame->pPlayerTextures[i] = createPlayerCharacter(pGame->pRenderer, pGame->pWindow, CHARACTER_PICTURE); //g�r en str�ng av detta ist
        startPosition += 1;
    }

    // KRASCHAR P� MAC initiateLanguage(fp, pGame);

    pGame->state = MAIN_MENU;

    return 1;
}

void runGame(Game* pGame){
    bool isRunning = true, left = false, right = false;
    SDL_Event event;
    int num, time = 0;
    float jumpHeight = pGame->windowHeight - JUMP_HEIGHT;

    Mix_VolumeMusic(32);
    Mix_PlayMusic(pGame->pMainSound, -1);
    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU: handleMainMenu(pGame, event, &time);
            break;
            case SETTINGS_MENU: handleSettingsMenu(pGame, event, &num);
            break;
            case ENTER_INPUT: handleEnterInput(pGame, event, &num);
            break;
            case ONGOING: handleOngoing(pGame, event, &isRunning, &right, &left, &jumpHeight, &time);
            break;
            case GAME_MENU: handleGameMenu(pGame, event);
            break;
            case GAME_OVER:
                //renderText(pGame->pGameOverText);
            break;
            case QUIT: isRunning = false;
            break;
            default:
            break;
        }
        SDL_RenderPresent(pGame->pRenderer);
    }
}

void quitGame(Game* pGame){
    if (pGame->pJumpSound){
        destroyChunk(pGame->pJumpSound);
    }
    if (pGame->pMainSound){
        destroyMusic(pGame->pMainSound);
    }
    if (pGame->platforms[0]){
        destroyPlatform(pGame->platforms);
    }
    if (pGame->pPlayers[0]){
        destroyPlayer(&pGame->pPlayers[0]);
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
    if (pGame->pStartingPlatform){
        free(pGame->pStartingPlatform);
    }
    if (pGame->pBackground){
        destroyBackground(pGame->pBackground);
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
    //destro playerRenderer?
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

int handleError(Game* pGame, void* pMember, const char* (*GetError)(void)){
    if (!pMember){
        printf("Error: %s\n", (*GetError)());
        quitGame(pGame);
        return 0;
    }
    else return 1;
}

void initiateLanguage(FILE *fp, Game *pGame){
    readFromFileLang(fp, pGame->language);
    pGame->pStartButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[0], pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pSettingsButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[1], pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[2], pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pResumeButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[3], pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pMainMenuButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[4], pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pLanguageButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[5], pGame->windowWidth, pGame->windowHeight, -100, 0);
    pGame->pEnglishButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "English", pGame->windowWidth, pGame->windowHeight, -50, 0);
    pGame->pSwedishButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Svenska", pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pReturnButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[6], pGame->windowWidth, pGame->windowHeight, 200, 0);
    pGame->pMoveRight1ButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[7], pGame->windowWidth, pGame->windowHeight, 50, -80);
    pGame->pMoveLeft1ButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, pGame->language[8], pGame->windowWidth, pGame->windowHeight, 100, -80);
}

void handleMainMenu(Game* pGame, SDL_Event event, int* pTime){
    Mix_ResumeMusic();

    renderMenu(pGame->pRenderer, pGame->pMainMenuTexture, pGame->windowWidth, pGame->windowHeight);
    renderButton(pGame->pStartButton, pGame->pRenderer);
    renderButton(pGame->pSettingsButton, pGame->pRenderer);
    renderButton(pGame->pQuitButton, pGame->pRenderer);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pStartButton, pGame->pRenderer, &pGame->state, ONGOING);
        handleButton(pGame->pSettingsButton, pGame->pRenderer, &pGame->state, SETTINGS_MENU);
        handleButton(pGame->pQuitButton, pGame->pRenderer, &pGame->state, QUIT);

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
        resetGame(pGame, pTime);
    }
        // KRASHCAR MAC renderText(pGame->pStartButtonText);
        // KRASCHAR MAC renderText(pGame->pSettingsButtonText);
        // KRASCHAR MAC renderText(pGame->pQuitButtonText);
}

void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum){
    char chosenLang[LANG_LENGTH];
    bool showLang = false;

    while (SDL_PollEvent(&event)){
        getMousePos(pGame->pLanguageButton);
        if (pGame->pLanguageButton->mouseDistance < BUTTON_HEIGHT && pGame->pLanguageButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            showLang = !showLang;
        }
        getMousePos(pGame->pMoveRight2Button);
        if (pGame->pMoveRight2Button->mouseDistance < BUTTON_HEIGHT && pGame->pMoveRight2Button->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            *pNum = 0;
            pGame->state = ENTER_INPUT;
        }
        getMousePos(pGame->pMoveLeft2Button);
        if (pGame->pMoveLeft2Button->mouseDistance < BUTTON_HEIGHT && pGame->pMoveLeft2Button->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            *pNum = 1;
            pGame->state = ENTER_INPUT;
        }
        getMousePos(pGame->pReturnButton);
        if (pGame->pReturnButton->mouseDistance < BUTTON_HEIGHT && pGame->pReturnButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            pGame->state = MAIN_MENU;
            showLang = false;
        }

        renderMenu(pGame->pRenderer, pGame->pMainMenuTexture, pGame->windowWidth, pGame->windowHeight);
        if (showLang){
            getMousePos(pGame->pEnglishButton);
            if (pGame->pEnglishButton->mouseDistance < BUTTON_HEIGHT && pGame->pEnglishButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
                showLang = false;
                strcpy(chosenLang, "english.txt");
            }
            getMousePos(pGame->pSwedishButton);
            if (pGame->pSwedishButton->mouseDistance < BUTTON_HEIGHT && pGame->pSwedishButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
                showLang = false;
                strcpy(chosenLang, "svenska.txt");
            }

            if (!showLang){
                FILE *fp;
                changeLanguageInFile(fp, chosenLang);
                initiateLanguage(fp, pGame);
            }

            //char moveLeft[50] = SDL_GetKeyName(pGame->keybinds[1]);
            //pGame->pMoveRight2ButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, moveRight, pGame->windowWidth, pGame->windowHeight, 100, 80);
            //pGame->pMoveLeft2ButtonText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, moveLeft, pGame->windowWidth, pGame->windowHeight, 100, 80);

            renderButton(pGame->pEnglishButton, pGame->pRenderer);
            renderButton(pGame->pSwedishButton, pGame->pRenderer);
            renderText(pGame->pEnglishButtonText);
            renderText(pGame->pSwedishButtonText);
        }
        renderButton(pGame->pLanguageButton, pGame->pRenderer);
        renderButton(pGame->pMoveRight2Button, pGame->pRenderer);
        renderButton(pGame->pMoveLeft2Button, pGame->pRenderer);
        renderButton(pGame->pReturnButton, pGame->pRenderer);
        renderSettingsMenu(pGame);
    }
}

void renderSettingsMenu(Game *pGame){
    renderButton(pGame->pLanguageButton, pGame->pRenderer);
    renderText(pGame->pLanguageButtonText);
    renderButton(pGame->pReturnButton, pGame->pRenderer);
    renderText(pGame->pReturnButtonText);
    renderButton(pGame->pMoveRight1Button, pGame->pRenderer);
    renderText(pGame->pMoveRight1ButtonText);
    renderButton(pGame->pMoveLeft1Button, pGame->pRenderer);
    renderText(pGame->pMoveLeft1ButtonText);
    renderButton(pGame->pMoveRight2Button, pGame->pRenderer);
    //renderText(pGame->pMoveRight2ButtonText);
    renderButton(pGame->pMoveLeft2Button, pGame->pRenderer);
}

void handleEnterInput(Game* pGame, SDL_Event event, int* pNum){
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_KEYDOWN:
            if ((event.key.keysym.sym) == (SDLK_ESCAPE)){
                pGame->state = SETTINGS_MENU;
            } else{
                pGame->keybinds[*pNum] = (event.key.keysym.sym);
                FILE *fp;
                saveToFile(fp, pGame->keybinds);
                pGame->state = MAIN_MENU;
            }
            break;
        }
    }
}

void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pRight, bool* pLeft, float* pJumpHeight, int* pTime){
    Mix_ResumeMusic();
    while (SDL_PollEvent(&event)){
        handleInputOngoing(&pGame->state, &event, pIsRunning, pRight, pLeft, pGame->keybinds);
    }

    handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight); //denna m�ste ligga f�re allt med player

    handlePlayers(pGame, pLeft, pRight, pJumpHeight);

    handlePlatform(pGame->platforms, pGame->pRenderer, pGame->windowWidth);
    handleStartingPlatform(pGame->pStartingPlatform, pGame->pRenderer, pGame->pStartPlatformTexture, pGame->windowHeight, pTime);
    //checkIfPlayerDead(pGame->pPlayers[0], pGame->windowHeight, &pGame->state);
    SDL_Delay(1000/60);
}

void handleInputOngoing(State* pState, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft, int keybinds[]){
    switch (event->type){
        case SDL_QUIT: *pIsRunning = false;
        break;
        case SDL_KEYDOWN:
            if ((event->key.keysym.sym) == (SDLK_ESCAPE)){
                *pState = GAME_MENU;
            } else if ((event->key.keysym.sym) == keybinds[0]){
                *pRight = true;
            } else if ((event->key.keysym.sym) == keybinds[1]){
                *pLeft = true;

            }
            // switch (event->key.keysym.sym){
            //     case SDLK_ESCAPE: *pState = GAME_MENU;
            //         break;
            //     case SDLK_RIGHT: *pRight = true;
            //         break;
            //     case SDLK_LEFT: *pLeft = true;
            //         break;
            // }
        break;
        case SDL_KEYUP:
            if ((event->key.keysym.sym) == keybinds[0]){
                *pRight = false;
            } else if ((event->key.keysym.sym) == keybinds[1]){
                *pLeft = false;
            }
            // switch(event->key.keysym.sym){
            //     case SDLK_LEFT: *pLeft = false;
            //         break;
            //     case SDLK_RIGHT: *pRight = false;
            //         break;
            // }
        break;

    }
}

void handleGameMenu(Game* pGame, SDL_Event event){
    Mix_PauseMusic();

    renderButton(pGame->pResumeButton, pGame->pRenderer);
    renderButton(pGame->pMainMenuButton, pGame->pRenderer);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pResumeButton, pGame->pRenderer, &pGame->state, ONGOING);
        handleButton(pGame->pMainMenuButton, pGame->pRenderer, &pGame->state, MAIN_MENU);

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }

    // G�R S� ATT MAN INTE KAN KOMMA TILL RESUMEMENU renderText(pGame->pMainMenuButtonText);
    // G�R S� ATT MAN INTE KAN KOMMA TILL RESUMEMENU renderText(pGame->pResumeButtonText);
}

void handlePlayers(Game* pGame, bool *pLeft, bool *pRight, float *pJumpHeight){

    for (int i = 0; i < pGame->pNrOfPlayers - 1; i++) //av n�gon anledning dyker inte player 2 upp, f�rmodligen pga samma bild och position, samt b�da r�r sig med tangenttrycken
    {
        if (i == 0) //bara f�r att prova om spelare 2 dyker upp i loopen
        {
            movePlayer(pGame->pPlayers[i], *pLeft, *pRight, pGame->windowWidth);
            jumpPlayer(pGame->pPlayers[i], *pJumpHeight, pGame->pStartingPlatform->yPos, pGame->pJumpSound);
            playerCollidePlatform(pGame->pPlayers[i], pGame->platforms, pJumpHeight, pGame->windowHeight, pGame->pJumpSound);
            renderPlayer(pGame->pPlayers[i], pGame->pRenderer, pGame->pPlayerTextures[i]);
        }
        else
        {
            jumpPlayer(pGame->pPlayers[i], *pJumpHeight, pGame->pStartingPlatform->yPos, pGame->pJumpSound);
            playerCollidePlatform(pGame->pPlayers[i], pGame->platforms, pJumpHeight, pGame->windowHeight, pGame->pJumpSound);
            renderPlayer(pGame->pPlayers[i], pGame->pRenderer, pGame->pPlayerTextures[i]);
        }

    }
}

void resetGame(Game* pGame, int* pTime){
    if (pGame->state == ONGOING){
        resetStartingPlatform(pGame->pStartingPlatform, pGame->windowHeight, pTime);
        resetPlatform(pGame->platforms);
    }
}