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

    if (SDL_Init(SDL_INIT_EVERYTHING)){
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init()){
        printf("Error: %s\n", TTF_GetError());
        return 0;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("Error: %s\n", Mix_GetError());
        return 0;
    }
	if (SDLNet_Init() == -1) {
		printf("Error: %s\n", SDLNet_GetError());
		return 0;
	}

    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode)){
        printf("Error: %s\n", SDL_GetError());
        quitGame(pGame);
        return 0;
    }
    pGame->gameDisplay.windowWidth = displayMode.w;
    pGame->gameDisplay.windowHeight = displayMode.h;

	pGame->pNetworkData = malloc(sizeof(NetworkData));
	pGame->pGameplayData = malloc(sizeof(GameplayData));
	*pGame->pNetworkData = (NetworkData){0};
	*pGame->pGameplayData = (GameplayData){0};
	*pGame->pClientCommands = (ClientCommand){0};
	pGame->pNetworkData->pPlayers = pGame->pPlayers;
	initializeNetcode(pGame->pNetworkData);

    pGame->gameDisplay.pWindow = SDL_CreateWindow("Mental breakdown", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->gameDisplay.windowWidth, pGame->gameDisplay.windowHeight, 0);
    if (!handleError(pGame, pGame->gameDisplay.pWindow, SDL_GetError)) return 0;

	Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	#if __APPLE__ || __LINUX__
		flags = (flags & !SDL_RENDERER_ACCELERATED) | SDL_RENDERER_SOFTWARE;
	#endif
    pGame->gameDisplay.pRenderer = SDL_CreateRenderer(pGame->gameDisplay.pWindow, -1, flags);
    if (!handleError(pGame, pGame->gameDisplay.pRenderer, SDL_GetError)) return 0;

    pGame->pMenuFont = TTF_OpenFont("../assets/Ticketing.ttf", 25);
    if (!handleError(pGame, pGame->pMenuFont, TTF_GetError)) return 0;

    pGame->pMenuTexture = createPicture(&pGame->gameDisplay, MAIN_MENU_PICTURE);
    if (!handleError(pGame, pGame->pMenuTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pBackgroundTexture = createPicture(&pGame->gameDisplay, BACKGROUND_PICTURE);
    if (!handleError(pGame, pGame->pBackgroundTexture, SDL_GetError)) {
        return 0;
    }
    pGame->buttons.pButtonTexture = createPicture(&pGame->gameDisplay, BUTTON_PICTURE);
    if (!handleError(pGame, pGame->buttons.pButtonTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pPlatformTexture = createPicture(&pGame->gameDisplay, PLATFORM_PICTURE);
    if (!handleError(pGame, pGame->pPlatformTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pStartPlatformTexture = createPicture(&pGame->gameDisplay, START_PLATFORM_PICTURE);
    if (!handleError(pGame, pGame->pStartPlatformTexture, SDL_GetError)) {
        return 0;
    }

    pGame->pBackground = createBackground(pGame->gameDisplay.windowHeight);

    pGame->buttons.pStartButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pSettingsButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pQuitButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pCreateLobbyButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 0, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pJoinLobbyButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pLanguageButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 - 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pMoveRightButton = createButton(pGame->gameDisplay.windowWidth / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pMoveLeftButton = createButton(pGame->gameDisplay.windowWidth / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pMuteButton = createButton(pGame->gameDisplay.windowWidth / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 150, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pReturnButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 200, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pEnglishButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 - 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pSwedishButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pResumeButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->buttons.pMainMenuButton = createButton((pGame->gameDisplay.windowWidth - BUTTON_WIDTH) / 2, (pGame->gameDisplay.windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);

    pGame->pStartPlatform = createPlatform(0, pGame->gameDisplay.windowHeight - 100, pGame->gameDisplay.windowWidth, 100);

    pGame->pMainSound = Mix_LoadMUS("../assets/MainThemeSoundtrack.mp3");
    if (!handleError(pGame, pGame->pMainSound, Mix_GetError)) return 0;
    pGame->pJumpSound = Mix_LoadWAV("../assets/JumpEffect.wav");
    if (!handleError(pGame, pGame->pJumpSound, Mix_GetError)) return 0;
    pGame->pWinSound = Mix_LoadWAV("../assets/tempWinSound.wav");
    if (!handleError(pGame, pGame->pWinSound, Mix_GetError)) return 0;

    FILE *fp;
    readFromFileKey(fp, pGame->keybinds);
    //saveToFile(fp, pGame->keybinds);
    //pGame->chosenLanguage = true;
    initiateLanguage(fp, pGame, &pGame->buttons);

    pGame->state = MAIN_MENU;

    return 1;
}

void initiateLanguage(FILE* fp, Game* pGame, Buttons* pButtons) {
    #if __APPLE__
		readFromFileLangMAC(pGame->chosenLanguage, pGame->language);
    #else
        readFromFileLang(fp, pGame->language);
	#endif

    readKeybindString(pGame, 0, &pGame->gameDisplay, &pGame->buttons);

    pButtons->pStartButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[0], 255, 255, 255, 0, 0);
    pButtons->pSettingsButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[1], 255, 255, 255, 0, 50);
    pButtons->pQuitButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[2], 255, 255, 255, 0, 100);
    pButtons->pResumeButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[3], 255, 255, 255, 0, 50);
    pButtons->pMainMenuButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[4], 255, 255, 255, 0, 100);
    pButtons->pLanguageButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[5], 255, 255, 255, 0, -100);
    pButtons->pReturnButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[6], 255, 255, 255, 0, 200);
    pButtons->pMoveRightButton1Text = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[7], 255, 255, 255, -80, 50);
    pButtons->pMoveLeftButton1Text = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[8], 255, 255, 255, -80, 100);
    pGame->pYouAreDeadText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[9], 255, 255, 255, 0, -200);
    pButtons->pEnglishButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, "English", 255, 255, 255, 0, -50);
    pButtons->pSwedishButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, "Svenska", 255, 255, 255, 0, 0);
    pButtons->pCreateLobbyButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[10], 255, 255, 255, 0, 0);
    pButtons->pJoinLobbyButtonText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[11], 255, 255, 255, 0, 50);
    pButtons->pMuteButton1Text = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[13], 255, 255, 255, -80, 150);
    pGame->pWaitingText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[16], 255, 255, 255, 0, 50);
    pGame->pEnterIPText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->language[17], 255, 255, 255, -130, 50);
}

int handleError(Game* pGame, void* pMember, const char* (*GetError)(void)) {
    if (!pMember) {
        printf("Error: %s\n", (*GetError)());
        quitGame(pGame);
        return 0;
    }
    else return 1;
}

void runGame(Game* pGame){
    SDL_Event event;
    bool isRunning = true, left = false, right = false, mute = false, showLang = false, joined = false, createLobbyPressed = false;
    int num, time = 0, inputIPIndex = 0;

    Mix_VolumeMusic(75);
    Mix_PlayMusic(pGame->pMainSound, -1);

    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU: handleMainMenu(pGame, event, &mute);
                break;
            case SETTINGS_MENU: handleSettingsMenu(pGame, event, &num, &showLang);
                break;
            case ENTER_INPUT: handleEnterInput(pGame, event, &num);
                break;
            case LOBBY_MENU: handleLobbyMenu(pGame, event, &left, &right, &time, &inputIPIndex, &joined);
                break;
            case LOBBY: handleLobby(pGame, event, &inputIPIndex, &joined);
                break;
            case ONGOING: handleOngoing(pGame, event, &isRunning, &left, &right, &time, &mute);
                break;
            case GAME_MENU: handleGameMenu(&pGame->gameDisplay, &pGame->buttons, event, &pGame->state, &mute);
                break;
            case GAME_OVER: handleGameOver(pGame, event);
                break;
            case QUIT: isRunning = false;
                break;
            default:
                break;
        }

        SDL_RenderPresent(pGame->gameDisplay.pRenderer);
    }
}

void handleMainMenu(Game* pGame, SDL_Event event, bool* pMute){
    bool buttonPressed = false;

    if (!(*pMute)) Mix_ResumeMusic();

    renderMainMenu(&pGame->gameDisplay, &pGame->buttons, pGame->pMenuTexture);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->buttons.pStartButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = LOBBY_MENU;
            buttonPressed = false;
        }
        handleButton(pGame->buttons.pSettingsButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = SETTINGS_MENU;
            buttonPressed = false;
        }
        handleButton(pGame->buttons.pQuitButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = QUIT;
            buttonPressed = false;
        }

        switch (event.type) {
            case SDL_QUIT: pGame->state = QUIT;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == pGame->keybinds[2] && !(*pMute)) {
                    *pMute = true;
                    Mix_PauseMusic();
                }
                else if (event.key.keysym.sym == pGame->keybinds[2] && (*pMute)) {
                    *pMute = false;
                    Mix_ResumeMusic();
                }
                break;
        }
    }
}

void renderMainMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Texture* pMenuTexture) { //kan flyttas
    renderMenu(pGameDisplay, pMenuTexture);

    renderButton(pButtons->pStartButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pSettingsButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pQuitButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pStartButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pSettingsButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pQuitButtonText, pGameDisplay->pRenderer);
}

void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum, bool *pShowLang){
    bool buttonPressed = false;

    renderSettingsMenu(&pGame->gameDisplay, &pGame->buttons, pGame->pMenuTexture);
    if (*pShowLang){
        renderLanguageMenu(&pGame->gameDisplay, &pGame->buttons);
    }

    while (SDL_PollEvent(&event)){
        handleButton(pGame->buttons.pLanguageButton, &buttonPressed);
        if (buttonPressed) {
            (*pShowLang) = true;
            buttonPressed = false;
        }
        handleButton(pGame->buttons.pMoveRightButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 0;
            readKeybindString(pGame, 1, &pGame->gameDisplay, &pGame->buttons);
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->buttons.pMoveLeftButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 1;
            readKeybindString(pGame, 2, &pGame->gameDisplay, &pGame->buttons);
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->buttons.pMuteButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 2;
            readKeybindString(pGame, 3, &pGame->gameDisplay, &pGame->buttons);
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->buttons.pReturnButton, &buttonPressed);
        if (buttonPressed){
            pGame->state = MAIN_MENU;
            *pShowLang = false;
            buttonPressed = false;
        }

        if ((*pShowLang)){
            handleLanguageMenu(pGame, event, pShowLang);
        }

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderSettingsMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Texture* pMenuTexture) { //kan flyttas
    renderMenu(pGameDisplay, pMenuTexture);

    renderButton(pButtons->pLanguageButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pMoveLeftButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pMoveRightButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pReturnButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pMuteButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pLanguageButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pMoveLeftButton1Text, pGameDisplay->pRenderer);
    renderText(pButtons->pMoveLeftButton2Text, pGameDisplay->pRenderer);
    renderText(pButtons->pMoveRightButton1Text, pGameDisplay->pRenderer);
    renderText(pButtons->pMoveRightButton2Text, pGameDisplay->pRenderer);
    renderText(pButtons->pMuteButton1Text, pGameDisplay->pRenderer);
    renderText(pButtons->pMuteButton2Text, pGameDisplay->pRenderer);
    renderText(pButtons->pReturnButtonText, pGameDisplay->pRenderer);
}

void handleLanguageMenu(Game* pGame, SDL_Event event, bool* pShowLang){
    bool buttonPressed = false;
    char chosenLang[LANG_LENGTH];
    FILE *fp;

    handleButton(pGame->buttons.pEnglishButton, &buttonPressed);
    if (buttonPressed){
        strcpy(chosenLang, "english.txt");
        *pShowLang = false;
        buttonPressed = false;
    }
    handleButton(pGame->buttons.pSwedishButton, &buttonPressed);
    if (buttonPressed){
        strcpy(chosenLang, "svenska.txt");
        *pShowLang = false;
        buttonPressed = false;
    }

    if (!(*pShowLang)){
        changeLanguageInFile(fp, chosenLang);
        /*if (strcmp(chosenLang, "svenska.txt")) {
            pGame->chosenLanguage = true;
        }
        else {
            pGame->chosenLanguage = false;
        }*/
        initiateLanguage(fp, pGame, &pGame->buttons);
    }
}

void renderLanguageMenu(GameDisplay* pGameDisplay, Buttons* pButtons) { //kan flyttas
    renderButton(pButtons->pEnglishButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pSwedishButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pEnglishButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pSwedishButtonText, pGameDisplay->pRenderer);
}

void readKeybindString(Game *pGame, int index, GameDisplay* pGameDisplay, Buttons* pButtons) {
    SDL_Keycode moveRight = pGame->keybinds[0], moveLeft = pGame->keybinds[1], mute = pGame->keybinds[2];
    char keybindsText[3][30];

    if (index == 0) {
        strcpy(keybindsText[0], (SDL_GetKeyName(moveRight)));
        strcpy(keybindsText[1], (SDL_GetKeyName(moveLeft)));
        strcpy(keybindsText[2], (SDL_GetKeyName(mute)));

        pButtons->pMoveRightButton2Text = createText(pGameDisplay, pGame->pMenuFont, keybindsText[0], 255, 255, 255, BUTTON_WIDTH / 2, 50);
        pButtons->pMoveLeftButton2Text = createText(pGameDisplay, pGame->pMenuFont, keybindsText[1], 255, 255, 255, BUTTON_WIDTH / 2, 100);
        pButtons->pMuteButton2Text = createText(pGameDisplay, pGame->pMenuFont, keybindsText[2], 255, 255, 255, BUTTON_WIDTH / 2, 150);
    }
    else if (index == 1) {
        pButtons->pMoveRightButton2Text = createText(pGameDisplay, pGame->pMenuFont, pGame->language[12], 255, 255, 255, BUTTON_WIDTH / 2, 50);
    }
    else if (index == 2) {
        pButtons->pMoveLeftButton2Text = createText(pGameDisplay, pGame->pMenuFont, pGame->language[12], 255, 255, 255, BUTTON_WIDTH / 2, 100);
    }
    else if (index == 3) {
        pButtons->pMuteButton2Text = createText(pGameDisplay, pGame->pMenuFont, pGame->language[12], 255, 255, 255, BUTTON_WIDTH / 2, 150);
    }
}

void handleEnterInput(Game* pGame, SDL_Event event, int* pNum) {
    FILE *fp;

    renderSettingsMenu(&pGame->gameDisplay, &pGame->buttons, pGame->pMenuTexture);

    while (SDL_PollEvent(&event)) {
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
                readKeybindString(pGame, 0, &pGame->gameDisplay, &pGame->buttons);
                break;
        }
    }
}

void handleLobbyMenu(Game* pGame, SDL_Event event, bool* pLeft, bool* pRight, int* pTime, int* pIndex, bool* pJoined) {
    bool buttonPressed = false;

    renderLobbyMenu(&pGame->gameDisplay, &pGame->buttons, pGame->pMenuTexture);

    if (*pJoined) {
        renderMenu(&pGame->gameDisplay, pGame->pMenuTexture);
        renderText(pGame->pEnterIPText, pGame->gameDisplay.pRenderer);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if ((event.key.keysym.sym) == (SDLK_RETURN) ) {
                        setConnection(pGame->pNetworkData, pGame->inputIP);
                        pGame->state = LOBBY;
                    }
                    else if ((event.key.keysym.sym) == (SDLK_BACKSPACE) && (*pIndex) > 0) {
                        (*pIndex)--;
                        pGame->inputIP[*pIndex] = '\0';
                    }
                    else if ((*pIndex) < INPUT_IP_LEN) {
                        pGame->inputIP[*pIndex] = (event.key.keysym.sym);
                        (*pIndex)++;
                    }
                break;
            }
            if (event.type == SDL_QUIT) {
                pGame->state = QUIT;
            }
        }
        if ((*pIndex) > 0) {
            pGame->pInputIPText = createText(&pGame->gameDisplay, pGame->pMenuFont, pGame->inputIP, 255, 255, 255, 70, 50);
            renderText(pGame->pInputIPText, pGame->gameDisplay.pRenderer);
        }
    } else {
        while (SDL_PollEvent(&event)) {
            handleButton(pGame->buttons.pCreateLobbyButton, &buttonPressed);
            if (buttonPressed) {
                setConnection(pGame->pNetworkData, NULL);
                pGame->state = LOBBY;
                buttonPressed = false;
            }
            handleButton(pGame->buttons.pJoinLobbyButton, &buttonPressed);
            if (buttonPressed) {
                *pJoined = true;
                buttonPressed = false;
            }
            handleButton(pGame->buttons.pReturnButton, &buttonPressed);
            if (buttonPressed){
                pGame->state = MAIN_MENU;
                buttonPressed = false;
            }
            if (event.type == SDL_QUIT) {
                pGame->state = QUIT;
            }
        }
    }
    if (pGame->state == LOBBY) {
        resetGame(pGame, pLeft, pRight, pTime);
    }
}

void renderLobbyMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Texture* pMenuTexture) { //kan flyttas
    renderMenu(pGameDisplay, pMenuTexture);

    renderButton(pButtons->pCreateLobbyButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pJoinLobbyButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pReturnButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pCreateLobbyButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pJoinLobbyButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pReturnButtonText, pGameDisplay->pRenderer);
}

void handleLobby(Game* pGame, SDL_Event event, int* pIndex, bool* pJoined) {
    bool buttonPressed = false;
    bool isHost = pGame->pNetworkData->isHost;
    char nrOfClients[2];

    renderMenu(&pGame->gameDisplay, pGame->pMenuTexture);

    runNetcode(pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands);

    if (isHost) { // SERVER
        renderButton(pGame->buttons.pStartButton, pGame->gameDisplay.pRenderer, pGame->buttons.pButtonTexture);
        renderText(pGame->buttons.pStartButtonText, pGame->gameDisplay.pRenderer);

        sprintf(nrOfClients, "%d", pGame->pNetworkData->nClients);
        pGame->pNrClientsText = createText(&pGame->gameDisplay, pGame->pMenuFont, nrOfClients, 255, 255, 255, 0, 70);
        renderText(pGame->pNrClientsText, pGame->gameDisplay.pRenderer);
        while (SDL_PollEvent(&event)) {
            handleButton(pGame->buttons.pStartButton, &buttonPressed);
            if (buttonPressed) {
                pGame->state = ONGOING;
                buttonPressed = false;
                pGame->pGameplayData->gameState = ONGOING;
            }
            if (event.type == SDL_QUIT) {
                pGame->state = QUIT;
            }
        }
    } else { // CLIENT
        renderMenu(&pGame->gameDisplay, pGame->pMenuTexture);
        renderText(pGame->pWaitingText, pGame->gameDisplay.pRenderer);
        renderButton(pGame->buttons.pMainMenuButton, pGame->gameDisplay.pRenderer, pGame->buttons.pButtonTexture);
        renderText(pGame->buttons.pMainMenuButtonText, pGame->gameDisplay.pRenderer);
        while (SDL_PollEvent(&event)) {
            handleButton(pGame->buttons.pMainMenuButton, &buttonPressed);
            if (buttonPressed) {
                pGame->state = MAIN_MENU;
                buttonPressed = false;
                (*pJoined) = false;
                //fillZero(pGame, INPUT_IP_LEN);
                //(*pIndex) = 0;
            }
            if (event.type == SDL_QUIT) {
                pGame->state = QUIT;
            }
        }
        if (pGame->pGameplayData->gameState == ONGOING) {
            pGame->state = ONGOING;
        }
    }
}

void fillZero(char inputIP[], int max) { // kan flyttas
    for (int i = 0; i < max; i++)
    {
        inputIP[i] = '\0';
    }
}

void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pLeft, bool *pRight, int *pTime, bool* pMute) {
    while (SDL_PollEvent(&event)) {
        handleOngoingInput(pGame, &event, pIsRunning, pLeft, pRight, pMute);
    }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool isHost = pGame->pNetworkData->isHost;

	if (isHost) {
		GameplayData temp;

        for(int i = 0; i < 6; i++){
            temp.players[i] = *pGame->pPlayers[i];
        }

        // for(int i = 0; i < 30; i++){
        //     if(pGame->pPlatforms[i]){
        //         temp.platforms[i] = *pGame->pPlatforms[i];
        //         temp.platforms[i].created = true;
        //     }
        //     else temp.platforms[i].created = false;
        // }
		temp.nrOfPlayers = pGame->nrOfPlayers;
		temp.nrOfPlayersLeft = pGame->nrOfPlayersLeft;

        temp.gameState = pGame->state;
		// SERVER: H�R SKA PUNKT (1) UTF�RAS
		// Det �r bara att l�gga in datan direkt i "temp".

		*pGame->pGameplayData = temp;
	}

	runNetcode(pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands);

	if (isHost) {
		// SERVER (4)
		for (int i = 0; pGame->pClientCommands[i].ip.host && i < COMMAND_BUFFER; i++) {
			// printf("looking at command at index %d\n", i);
			// printf("looking for host %d\n", pGame->pClientCommands[i].ip.host);
			// printf("looking for port %d\n", pGame->pClientCommands[i].ip.port);
			int iPlayer = 1;
			while ((pGame->pClientCommands[i].ip.host != pGame->pPlayers[iPlayer]->ip.host
					|| pGame->pClientCommands[i].ip.port != pGame->pPlayers[iPlayer]->ip.port)
					&& iPlayer < COMMAND_BUFFER) {
				// printf("comparing to index %d...\n", iPlayer);
				// printf("comparing to host %d\n", pGame->pPlayers[iPlayer]->ip.host);
				// printf("comparing to port %d\n", pGame->pPlayers[iPlayer]->ip.port);
				iPlayer++;
			}
			// printf("found player object associated with client command at index %d\n", iPlayer);
			ClientCommand tempCC = pGame->pClientCommands[i];
			Player *tempP = pGame->pPlayers[iPlayer];
			switch (tempCC.commandType) {
				case MOVEMENT: {
					// printf("movement direction: %d\n", tempCC.direction);
					tempP->moveLeft = (tempCC.direction == -1);
					tempP->moveRight = (tempCC.direction == 1);
					break;
                default:
                    break;
				}
			}
            tempP->flip = tempCC.flip;
			pGame->pClientCommands[i] = (ClientCommand){0};
		}
	} else {
		for (int i = 0; i < 6; i++) {
			*pGame->pPlayers[i] = pGame->pGameplayData->players[i];
		}
		pGame->nrOfPlayers = pGame->pGameplayData->nrOfPlayers;
		pGame->nrOfPlayersLeft = pGame->pGameplayData->nrOfPlayersLeft;

        // for(int i = 0; i < 30; i++){
        //     if(pGame->pGameplayData->platforms[i].created){
        //         //printf("Platform[%d]\n", i);
        //         if(pGame->pPlatforms[i] == 0){
        //             pGame->pPlatforms[i] = createPlatform(pGame->pGameplayData->platforms[i].xPos,
        //             pGame->pGameplayData->platforms[i].yPos, pGame->pGameplayData->platforms[i].width,
        //             pGame->pGameplayData->platforms[i].height);
        //         }else{
        //             *pGame->pPlatforms[i] = pGame->pGameplayData->platforms[i];
        //         }
        //     }
        // }
        /*for(int i = 0; i < pGame->nrOfPlayers; i++){
            *pGame->pPlayers[i] = pGame->pGameplayData->players[i];
        }
        if(pGame->pGameplayData->gameState == GAME_OVER){
            pGame->state = GAME_OVER;
        }*/
		// KLIENT: H�R SKA PUNKT (B) UTF�RAS
		// Datan �r tillg�nglig via pGame->GameplayData.

        /*
        L�ggs in n�r matning av GameplayData (Punkt 1) �r klar !!!

        *pGame->pPlayers = pGame->pGameplayData->players;
        pGame->nrOfPlayers = pGame->pGameplayData->nrOfPlayers;
        pGame->nrOfPlayersLeft = pGame->pGameplayData->nrOfPlayersLeft;
        pGame->windowWidth = pGame->pGameplayData->players->width;
        pGame->windowHeight = pGame->pGameplayData->players->height;
        */

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

    handleBackground(pGame->pBackground, &pGame->gameDisplay, pGame->pBackgroundTexture); //denna m�ste ligga f�re allt med player

	// KEEP THIS COMMENTED FOR NOW
    //handlePlatforms(pGame->pPlatforms, pGame->pRenderer, pGame->pPlatformTexture, pGame->windowWidth, pGame->windowHeight, isHost);

    handleStartPlatform(pGame->pStartPlatform, pGame->pPlatforms[0], pGame->gameDisplay.pRenderer, pGame->pStartPlatformTexture, pGame->gameDisplay.windowHeight, pTime);
    handlePlayers(pGame->pPlayers, pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pLeft, pRight, pMute, pGame->gameDisplay.windowWidth, pGame->gameDisplay.windowHeight, pGame->pStartPlatform, pGame->pJumpSound, pGame->pWinSound, &pGame->state, pGame->gameDisplay.pRenderer, pGame->pPlayerTextures, pGame->pPlatforms, pGame->pYouAreDeadText, &pGame->pNetworkData->isHost);

    SDL_Delay(3);
}

void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pLeft, bool* pRight, bool* pMute) {
	// Det visade sig att de events vi anv�nder f�r att l�sa input var lite mer komplicerat �n vad jag
	// f�rst trodde, s� jag var tvungen att modifiera pClientCommands[0] direkt via dess pointer ist�llet.
	if (!pGame->pNetworkData->isHost) {
        pGame->pClientCommands[0].commandType = MOVEMENT;
    }

    switch (event->type){
        case SDL_QUIT:
            *pIsRunning = false;
            pGame->pClientCommands[0].commandType = LEAVE;
            break;
        case SDL_KEYDOWN:
            if ((event->key.keysym.sym) == (SDLK_ESCAPE)){
                pGame->state = GAME_MENU;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[0]) {
                if (pGame->pNetworkData->isHost) {
					pGame->pPlayers[0]->moveRight = true;
					pGame->pPlayers[0]->flip = SDL_FLIP_NONE;
				} else {
					pGame->pClientCommands[0].direction = 1;
                    pGame->pClientCommands[0].flip = SDL_FLIP_NONE;
				}
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
                if (pGame->pNetworkData->isHost) {
					pGame->pPlayers[0]->moveLeft = true;
					pGame->pPlayers[0]->flip = SDL_FLIP_HORIZONTAL;
				} else {
					pGame->pClientCommands[0].direction = -1;
                    pGame->pClientCommands[0].flip = SDL_FLIP_HORIZONTAL;
				}
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[2] && !(*pMute)) {
                *pMute = true;
                Mix_PauseMusic();
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[2] && (*pMute)) {
                *pMute = false;
                Mix_ResumeMusic();
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
                if (pGame->pNetworkData->isHost) {
					pGame->pPlayers[0]->moveRight = false;
				} else {
					pGame->pClientCommands[0].direction = 0;
				}
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
                if (pGame->pNetworkData->isHost) {
					pGame->pPlayers[0]->moveLeft = false;
				} else {
					pGame->pClientCommands[0].direction = 0;
				}
            }
            // switch(event->key.keysym.sym){
            //     case SDLK_LEFT: *pLeft = false;
            //         break;m
            //     case SDLK_RIGHT: *pRight = false;
            //         break;
            // }
            break;
    }
}

void handleGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute) { //kan flyttas
    bool buttonPressed = false;

    Mix_PauseMusic();

    renderGameMenu(pGameDisplay, pButtons);

    while (SDL_PollEvent(&event)) {
        handleButton(pButtons->pResumeButton, &buttonPressed);
        if (buttonPressed) {
            *pState = ONGOING;
            buttonPressed = false;
            if (!(*pMute)) Mix_ResumeMusic();
        }
        handleButton(pButtons->pMainMenuButton, &buttonPressed);
        if (buttonPressed) {
            *pState = MAIN_MENU;
            buttonPressed = false;
        }

        if (event.type == SDL_QUIT) {
            *pState = QUIT;
        }
    }
}

void renderGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons) { // kan flyttas
    renderButton(pButtons->pResumeButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pMainMenuButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pMainMenuButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pResumeButtonText, pGameDisplay->pRenderer);
}

void handleGameOver(Game* pGame, SDL_Event event) {
    int i;
    static bool whoWonTextCreated = false;
    char whoWonString[50];
    bool buttonPressed = false;

    Mix_PauseMusic();

    for(i = 0; i < pGame->nrOfPlayers; i++) {
        if (pGame->pPlayers[i]->alive) {
            break;
        }
    }
    if(!whoWonTextCreated) {
        sprintf(whoWonString, "%s %d %s", pGame->language[14], i + 1, pGame->language[15]);
        pGame->pWhoWonText = createText(&pGame->gameDisplay, pGame->pMenuFont, whoWonString, 255, 255, 255, 0, -300);
        whoWonTextCreated = true;
    }

    renderGameOver(&pGame->gameDisplay, &pGame->buttons, pGame->pWhoWonText);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->buttons.pMainMenuButton, &buttonPressed);
        if (buttonPressed) {
            destroyText(pGame->pWhoWonText);
            whoWonTextCreated = false;
            pGame->state = MAIN_MENU;
            buttonPressed = false;
        }

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderGameOver(GameDisplay* pGameDisplay, Buttons* pButtons, Text* pWhoWonText) { //kan flyttas
    renderButton(pButtons->pMainMenuButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pMainMenuButtonText, pGameDisplay->pRenderer);
    renderText(pWhoWonText, pGameDisplay->pRenderer);
}

void resetGame(Game* pGame, bool* pLeft, bool* pRight, int* pTime){
    char avatar[6][25] = {"../assets/player1.png", "../assets/player2.png", "../assets/player3.png", "../assets/player4.png", "../assets/player5.png", "../assets/player6.png"};
    int subtractXPos = -100;
    int increaseXPos = 0;

    resetPlatforms(pGame->pPlatforms);
    resetStartPlatform(pGame->pStartPlatform, pGame->gameDisplay.windowHeight, pTime);
    resetPlayers(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		initPlayer(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, &pGame->gameDisplay, pGame->pStartPlatform->yPos, pGame->pPlayerTextures, &subtractXPos, &increaseXPos, avatar[i]);
	}
}

void quitGame(Game* pGame){
    int i;

    destroyPlayers(pGame->pPlayers);
    destroyPlatform(pGame->pStartPlatform);
    destroyPlatforms(pGame->pPlatforms);
    destroyBackground(pGame->pBackground);

    destroyChunk(pGame->pWinSound);
    destroyChunk(pGame->pJumpSound);
    destroyMusic(pGame->pMainSound);

    destroyText(pGame->pYouAreDeadText);
    destroyText(pGame->buttons.pMainMenuButtonText);
    destroyText(pGame->buttons.pResumeButtonText);
    destroyText(pGame->buttons.pJoinLobbyButtonText);
    destroyText(pGame->buttons.pCreateLobbyButtonText);
    destroyText(pGame->buttons.pSwedishButtonText);
    destroyText(pGame->buttons.pEnglishButtonText);
    destroyText(pGame->buttons.pReturnButtonText);
    destroyText(pGame->buttons.pMuteButton2Text);
    destroyText(pGame->buttons.pMuteButton1Text);
    destroyText(pGame->buttons.pMoveLeftButton2Text);
    destroyText(pGame->buttons.pMoveLeftButton1Text);
    destroyText(pGame->buttons.pMoveRightButton2Text);
    destroyText(pGame->buttons.pMoveRightButton1Text);
    destroyText(pGame->buttons.pLanguageButtonText);
    destroyText(pGame->buttons.pQuitButtonText);
    destroyText(pGame->buttons.pSettingsButtonText);
    destroyText(pGame->buttons.pStartButtonText);

    destroyButton(pGame->buttons.pMainMenuButton);
    destroyButton(pGame->buttons.pResumeButton);
    destroyButton(pGame->buttons.pJoinLobbyButton);
    destroyButton(pGame->buttons.pCreateLobbyButton);
    destroyButton(pGame->buttons.pSwedishButton);
    destroyButton(pGame->buttons.pEnglishButton);
    destroyButton(pGame->buttons.pReturnButton);
    destroyButton(pGame->buttons.pMoveLeftButton);
    destroyButton(pGame->buttons.pMoveRightButton);
    destroyButton(pGame->buttons.pLanguageButton);
    destroyButton(pGame->buttons.pQuitButton);
    destroyButton(pGame->buttons.pSettingsButton);
    destroyButton(pGame->buttons.pStartButton);

    destroyPlayerTextures(pGame->pPlayerTextures);
    destroyTexture(pGame->pStartPlatformTexture);
    destroyTexture(pGame->pPlatformTexture);
    destroyTexture(pGame->pBackgroundTexture);
    destroyTexture(pGame->pMenuTexture);

    if (pGame->gameDisplay.pRenderer) {
        SDL_DestroyRenderer(pGame->gameDisplay.pRenderer);
    }
    if (pGame->gameDisplay.pWindow) {
        SDL_DestroyWindow(pGame->gameDisplay.pWindow);
    }

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}