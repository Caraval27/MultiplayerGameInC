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

    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }

	pGame->pNetworkData = malloc(sizeof(NetworkData));
	pGame->pGameplayData = malloc(sizeof(GameplayData));
	pGame->pClientCommand = malloc(sizeof(ClientCommand));

    pGame->windowWidth = displayMode.w;
    pGame->windowHeight = displayMode.h;

    pGame->pWindow = SDL_CreateWindow("Mental breakdown", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->windowWidth, pGame->windowHeight, 0);
    if (!handleError(pGame, pGame->pWindow, SDL_GetError)) return 0;

	Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	#if __APPLE__ || __LINUX__
		flags = (flags & !SDL_RENDERER_ACCELERATED) | SDL_RENDERER_SOFTWARE;
	#endif
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, flags);
    if (!handleError(pGame, pGame->pRenderer, SDL_GetError)) return 0;

    pGame->pMenuTexture = createPicture(pGame->pWindow, pGame->pRenderer, MAIN_MENU_PICTURE);
    if (!handleError(pGame, pGame->pMenuTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pBackgroundTexture = createPicture(pGame->pWindow, pGame->pRenderer, BACKGROUND_PICTURE);
    if (!handleError(pGame, pGame->pBackgroundTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pPlatformTexture = createPicture(pGame->pWindow, pGame->pRenderer, PLATFORM_PICTURE);
    if (!handleError(pGame, pGame->pPlatformTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pStartPlatformTexture = createPicture(pGame->pWindow, pGame->pRenderer, START_PLATFORM_PICTURE);
    if (!handleError(pGame, pGame->pStartPlatformTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pMenuFont = TTF_OpenFont("../assets/Ticketing.ttf", 25);
    if (!handleError(pGame, pGame->pWindow, TTF_GetError)) return 0;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("Error: %s\n", Mix_GetError());
        return 0;
    }
    pGame->pMainSound = Mix_LoadMUS("../assets/MainThemeSoundtrack.mp3");
    if (!handleError(pGame, pGame->pWindow, Mix_GetError)) return 0;
    pGame->pJumpSound = Mix_LoadWAV("../assets/JumpEffect.wav");
    if (!handleError(pGame, pGame->pWindow, Mix_GetError)) return 0;
    pGame->pWinSound = Mix_LoadWAV("../assets/tempWinSound.wav");
    if (!handleError(pGame, pGame->pWindow, Mix_GetError)) return 0;

    pGame->pBackground = createBackground(pGame->windowHeight);
    pGame->pStartButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pSettingsButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pQuitButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pLanguageButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 - 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveLeftButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 + 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveRightButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 + 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pReturnButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 200, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pEnglishButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 - 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pSwedishButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pResumeButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMainMenuButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    //pGame->pMoveLeft1Button = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 - 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    //pGame->pMoveRight1Button = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2 - 80, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pCreateLobbyButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 0, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pJoinLobbyButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);

    pGame->pStartPlatform = createPlatform(0, pGame->windowHeight - 100, pGame->windowWidth, 100);

    pGame->pGameOverText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "You are dead", pGame->windowWidth, pGame->windowHeight, -200, 0);
    pGame->pWhoWonText[0] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 0 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[1] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 1 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[2] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 2 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[3] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 3 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[4] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 4 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[5] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 5 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[6] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 6 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[7] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 7 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[8] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 8 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[9] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Player 9 won", pGame->windowWidth, pGame->windowHeight, -300, 0);
    pGame->pWhoWonText[10] = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "You won!", pGame->windowWidth, pGame->windowHeight, -300, 0);

    initPlayers(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pGame->windowWidth, pGame->pStartPlatform->yPos, pGame->pPlayerTextures, pGame->pWindow, pGame->pRenderer);

    FILE *fp;
    readFromFileKey(fp, pGame->keybinds);
    saveToFile(fp, pGame->keybinds);
    initiateLanguage(fp, pGame);

    pGame->flip = SDL_FLIP_NONE;
    pGame->state = MAIN_MENU;

    return 1;
}

void initiateLanguage(FILE *fp, Game *pGame){
    //readFromFileLang(fp, pGame->language);
    pGame->pStartButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Start Game", pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pSettingsButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Settings", pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Quit", pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pResumeButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Resume Game", pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pMainMenuButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Main Menu", pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pLanguageButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Language", pGame->windowWidth, pGame->windowHeight, -100, 0);
    pGame->pReturnButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Return", pGame->windowWidth, pGame->windowHeight, 200, 0);
    pGame->pMoveRightButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Move right:", pGame->windowWidth, pGame->windowHeight, 50, -80);
    pGame->pMoveLeftButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Move left:", pGame->windowWidth, pGame->windowHeight, 100, -80);
    pGame->pGameOverText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Game Over", pGame->windowWidth, pGame->windowHeight, -200, 0);
    pGame->pEnglishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "English", pGame->windowWidth, pGame->windowHeight, -50, 0);
    pGame->pSwedishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Svenska", pGame->windowWidth, pGame->windowHeight, 0, 0);
    //pGame->pMoveLeft1ButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, moveLeft, pGame->windowWidth, pGame->windowHeight, 100, 80);
    //pGame->pMoveRight1ButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, moveRight, pGame->windowWidth, pGame->windowHeight, 100, 80);
    pGame->pCreateLobbyButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Create Lobby", pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pJoinLobbyButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Join Lobby", pGame->windowWidth, pGame->windowHeight, 50, 0);
}

/*void initiateLanguage(FILE *fp, Game *pGame){
    readFromFileLang(fp, pGame->language);
    pGame->pStartButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[0], pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pSettingsButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[1], pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[2], pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pResumeButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[3], pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pMainMenuButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[4], pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pLanguageButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[5], pGame->windowWidth, pGame->windowHeight, -100, 0);
    pGame->pReturnButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[6], pGame->windowWidth, pGame->windowHeight, 200, 0);
    pGame->pMoveRightButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[7], pGame->windowWidth, pGame->windowHeight, 50, -80);
    pGame->pMoveLeftButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[8], pGame->windowWidth, pGame->windowHeight, 100, -80);
    pGame->pGameOverText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[9], pGame->windowWidth, pGame->windowHeight, 100, -80);
    pGame->pEnglishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "English", pGame->windowWidth, pGame->windowHeight, -50, 0);
    pGame->pSwedishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Svenska", pGame->windowWidth, pGame->windowHeight, 0, 0);
    //pGame->pMoveLeft1ButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, moveLeft, pGame->windowWidth, pGame->windowHeight, 100, 80);
    //pGame->pMoveRight1ButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, moveRight, pGame->windowWidth, pGame->windowHeight, 100, 80);
}*/

int handleError(Game* pGame, void* pMember, const char* (*GetError)(void)){
    if (!pMember){
        printf("Error: %s\n", (*GetError)());
        quitGame(pGame);
        return 0;
    }
    else return 1;
}

void runGame(Game* pGame){
    bool isRunning = true, left = false, right = false;
    SDL_Event event;
    int num, time = 0;

    Mix_VolumeMusic(75);
    Mix_PlayMusic(pGame->pMainSound, -1);

    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU: handleMainMenu(pGame, event);
                break;
            case SETTINGS_MENU: handleSettingsMenu(pGame, event, &num);
                break;
            case ENTER_INPUT: handleEnterInput(pGame, event, &num);
                break;
            case LOBBY_MENU: handleLobbyMenu(pGame, event, &time);
                break;
            case ONGOING: handleOngoing(pGame, event, &isRunning, &right, &left, &time);
                break;
            case GAME_MENU: handleGameMenu(pGame, event);
                break;
            case GAME_OVER: handleGameOverMenu(pGame, event);
                break;
            case QUIT: isRunning = false;
                break;
            default:
                break;
        }

        SDL_RenderPresent(pGame->pRenderer);
    }
}

void handleMainMenu(Game* pGame, SDL_Event event){
    bool buttonPressed = false;

    Mix_ResumeMusic();

    renderMainMenu(pGame);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pStartButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = LOBBY_MENU;
            buttonPressed = false;
        }
        handleButton(pGame->pSettingsButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = SETTINGS_MENU;
            buttonPressed = false;
        }
        handleButton(pGame->pQuitButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = QUIT;
            buttonPressed = false;
        }

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderMainMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pStartButton, pGame->pRenderer);
    renderButton(pGame->pSettingsButton, pGame->pRenderer);
    renderButton(pGame->pQuitButton, pGame->pRenderer);

    renderText(pGame->pStartButtonText);
    renderText(pGame->pSettingsButtonText);
    renderText(pGame->pQuitButtonText);
}

void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum){
    bool buttonPressed = false, showLang = false;

    renderSettingsMenu(pGame);

    while (SDL_PollEvent(&event)){
        handleButton(pGame->pLanguageButton, &buttonPressed);
        if (buttonPressed) {
            showLang = true;
            buttonPressed = false;
        }
        handleButton(pGame->pMoveLeftButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 1;
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->pMoveRightButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 0;
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->pReturnButton, &buttonPressed);
        if (buttonPressed){
            pGame->state = MAIN_MENU;
            showLang = false;
            buttonPressed = false;
        }

        if (showLang){
            handleLanguageMenu(pGame, &showLang);
        }
        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderSettingsMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pLanguageButton, pGame->pRenderer);
    renderButton(pGame->pMoveLeftButton, pGame->pRenderer);
    renderButton(pGame->pMoveRightButton, pGame->pRenderer);
    renderButton(pGame->pReturnButton, pGame->pRenderer);
    //renderButton(pGame->pMoveRight1Button, pGame->pRenderer);
    //renderButton(pGame->pMoveLeft1Button, pGame->pRenderer);

    renderText(pGame->pLanguageButtonText);
    renderText(pGame->pMoveLeftButtonText);
    renderText(pGame->pMoveRightButtonText);
    renderText(pGame->pReturnButtonText);
    //renderText(pGame->pMoveLeft1ButtonText);
    //renderText(pGame->pMoveRight1ButtonText);

}

void handleLanguageMenu(Game* pGame, bool* pShowLang){
    bool buttonPressed = false;
    char chosenLang[LANG_LENGTH];
    FILE *fp;

    renderLanguageMenu(pGame);

    handleButton(pGame->pEnglishButton, &buttonPressed);
    if (buttonPressed){
        strcpy(chosenLang, "english.txt");
        *pShowLang = false;
        buttonPressed = false;
    }
    handleButton(pGame->pSwedishButton, &buttonPressed);
    if (buttonPressed){
        strcpy(chosenLang, "svenska.txt");
        *pShowLang = false;
        buttonPressed = false;
    }

    if (!*pShowLang){
        changeLanguageInFile(fp, chosenLang);
        initiateLanguage(fp, pGame);
    }

    //char moveLeft[50] = SDL_GetKeyName(pGame->keybinds[1]);
}

void renderLanguageMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pEnglishButton, pGame->pRenderer);
    renderButton(pGame->pSwedishButton, pGame->pRenderer);

    renderText(pGame->pEnglishButtonText);
    renderText(pGame->pSwedishButtonText);
}

void handleLobbyMenu(Game* pGame, SDL_Event event, int* pTime){
    bool buttonPressed = false;

    renderLobbyMenu(pGame);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pCreateLobbyButton, &buttonPressed);
        if (buttonPressed) {
            initializeNetcode(pGame->pNetworkData, true);
            pGame->state = ONGOING;
            buttonPressed = false;
        }
        handleButton(pGame->pJoinLobbyButton, &buttonPressed);
        if (buttonPressed) {
            initializeNetcode(pGame->pNetworkData, false);
            pGame->state = ONGOING;
            buttonPressed = false;
        }
        handleButton(pGame->pReturnButton, &buttonPressed);
        if (buttonPressed){
            pGame->state = MAIN_MENU;
            buttonPressed = false;
        }

        resetGame(pGame, pTime);
    }
}

void renderLobbyMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pCreateLobbyButton, pGame->pRenderer);
    renderButton(pGame->pJoinLobbyButton, pGame->pRenderer);
    renderButton(pGame->pReturnButton, pGame->pRenderer);

    renderText(pGame->pCreateLobbyButtonText);
    renderText(pGame->pJoinLobbyButtonText);
    renderText(pGame->pReturnButtonText);
}

void handleEnterInput(Game* pGame, SDL_Event event, int* pNum){
    FILE *fp;

    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    pGame->state = SETTINGS_MENU;
                }
                else {
                    pGame->keybinds[*pNum] = (event.key.keysym.sym);
                    saveToFile(fp, pGame->keybinds);
                    pGame->state = SETTINGS_MENU;
                }
                break;
        }
    }
}

void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pRight, bool* pLeft, int *pTime){
    Mix_ResumeMusic();

    while (SDL_PollEvent(&event)){
        handleOngoingInput(pGame, &event, pIsRunning, pRight, pLeft);
    }

    handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight); //denna måste ligga före allt med player
    handlePlatforms(pGame->pPlatforms, pGame->pRenderer, pGame->pPlatformTexture, pGame->windowWidth, pGame->windowHeight);
    handleStartPlatform(pGame->pStartPlatform, pGame->pPlatforms[0], pGame->pRenderer, pGame->pStartPlatformTexture, pGame->windowHeight, pTime);
    handlePlayers(pGame->pPlayers, pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pLeft, pRight, pGame->windowWidth, pGame->windowHeight, pGame->pStartPlatform, pGame->pJumpSound, pGame->pWinSound, &pGame->state, pGame->pRenderer, pGame->pPlayerTextures, pGame->flip, pGame->pPlatforms, pGame->pGameOverText);

	if (pGame->pNetworkData->isHost) {
		int newClients = listenForNewClients(pGame->pNetworkData);
	} else {
		if (SDL_GetTicks64() % 1000 < 17) {
			ClientCommand temp;
			*pGame->pClientCommand = temp;
			sendClientCommand(pGame->pNetworkData, pGame->pClientCommand);
		}
	}

    SDL_Delay(1000/60);
}

void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft){
    switch (event->type){
        case SDL_QUIT: *pIsRunning = false;
            break;
        case SDL_KEYDOWN:
            if ((event->key.keysym.sym) == (SDLK_ESCAPE)){
                pGame->state = GAME_MENU;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[0]) {
                *pRight = true;
                pGame->flip = SDL_FLIP_HORIZONTAL;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
                *pLeft = true;
                pGame->flip = SDL_FLIP_HORIZONTAL;

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
            if ((event->key.keysym.sym) == pGame->keybinds[0]) {
                *pRight = false;
                pGame->flip = SDL_FLIP_NONE;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
                *pLeft = false;
                pGame->flip = SDL_FLIP_NONE;
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
    bool buttonPressed = false;

    Mix_PauseMusic();

    renderGameMenu(pGame);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pResumeButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = ONGOING;
            buttonPressed = false;
        }
        handleButton(pGame->pMainMenuButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = MAIN_MENU;
            buttonPressed = false;
        }

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderGameMenu(Game* pGame){
    renderButton(pGame->pResumeButton, pGame->pRenderer);
    renderButton(pGame->pMainMenuButton, pGame->pRenderer);

    renderText(pGame->pMainMenuButtonText);
    renderText(pGame->pResumeButtonText);
}

void handleGameOverMenu(Game* pGame, SDL_Event event){
    bool buttonPressed = false;
    int i;

    Mix_PauseMusic();

    renderGameOverMenu(pGame);
    for(i = 0; i < pGame->nrOfPlayers; i++) {
        if (pGame->pPlayers[i]->alive) {
            break;
        }
    }
    if (i <= pGame->nrOfPlayers - 1) {
        renderText(pGame->pWhoWonText[i]);
    }
    else {
        renderText(pGame->pWhoWonText[MAX_PLAYERS]);
    }

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pMainMenuButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = MAIN_MENU;
            buttonPressed = false;
        }

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderGameOverMenu(Game* pGame){
    renderButton(pGame->pMainMenuButton, pGame->pRenderer);
    renderText(pGame->pMainMenuButtonText);
}

void resetGame(Game* pGame, int* pTime){
    if (pGame->state == ONGOING) {
        resetPlatforms(pGame->pPlatforms);
        resetStartPlatform(pGame->pStartPlatform, pGame->windowHeight, pTime);
        initPlayers(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pGame->windowWidth, pGame->pStartPlatform->yPos, pGame->pPlayerTextures, pGame->pWindow, pGame->pRenderer);
    }
}

void quitGame(Game* pGame){
    if (pGame->pSwedishButton) {
        destroyButton(pGame->pSwedishButton);
    }
    if (pGame->pEnglishButton) {
        destroyButton(pGame->pEnglishButton);
    }
    if (pGame->pReturnButton) {
        destroyButton(pGame->pReturnButton);
    }
    if (pGame->pMoveLeftButton) {
        destroyButton(pGame->pMoveLeftButton);
    }
    if (pGame->pMoveRightButton) {
        destroyButton(pGame->pMoveRightButton);
    }
    if (pGame->pLanguageButton) {
        destroyButton(pGame->pLanguageButton);
    }
    if (pGame->pMainMenuButton) {
        destroyButton(pGame->pMainMenuButton);
    }
    if (pGame->pResumeButton) {
        destroyButton(pGame->pResumeButton);
    }
    if (pGame->pQuitButton) {
        destroyButton(pGame->pQuitButton);
    }
    if (pGame->pStartButton) {
        destroyButton(pGame->pStartButton);
    }
    if (pGame->pPlayers[0]) {
        destroyPlayers(&pGame->pPlayers[0]);
    }
    if (pGame->pStartPlatform) {
        destroyPlatform(pGame->pStartPlatform);
    }
    if (pGame->pPlatforms[0]) {
        destroyPlatforms(pGame->pPlatforms);
    }
    if (pGame->pBackground) {
        destroyBackground(pGame->pBackground);
    }
    if (pGame->pWinSound) {
        destroyChunk(pGame->pWinSound);
    }
    if (pGame->pJumpSound) {
        destroyChunk(pGame->pJumpSound);
    }
    if (pGame->pMainSound) {
        destroyMusic(pGame->pMainSound);
    }
    if (pGame->pPlayerTextures[0]) {
        destroyPlayerTextures(pGame->pPlayerTextures);
    }
    if (pGame->pStartPlatformTexture) {
        SDL_DestroyTexture(pGame->pStartPlatformTexture);
    }
    if (pGame->pPlatformTexture) {
        SDL_DestroyTexture(pGame->pPlatformTexture);
    }
    if (pGame->pBackgroundTexture) {
        SDL_DestroyTexture(pGame->pBackgroundTexture);
    }
    if (pGame->pMenuTexture) {
        SDL_DestroyTexture(pGame->pMenuTexture);
    }
    if (pGame->pRenderer) {
        SDL_DestroyRenderer(pGame->pRenderer);
    }
    if (pGame->pWindow) {
        SDL_DestroyWindow(pGame->pWindow);
    }
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
