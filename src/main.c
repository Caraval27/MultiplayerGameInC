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

    pGame->pLobbyConnect = malloc(sizeof(LobbyConnect));
	pGame->pNetworkData = malloc(sizeof(NetworkData));
	pGame->pGameplayData = malloc(sizeof(GameplayData));
    pGame->pPlayersData = malloc(sizeof(PlayersData));
    *pGame->pLobbyConnect = (LobbyConnect){0};
	*pGame->pNetworkData = (NetworkData){0};
	*pGame->pGameplayData = (GameplayData){0};
	*pGame->pClientCommands = (ClientCommand){0};
	pGame->pNetworkData->pPlayers = pGame->pPlayersData->pPlayers;
	initializeNetcode(pGame->pNetworkData);

    pGame->gameDisplay.pWindow = SDL_CreateWindow("Mental breakdown", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->gameDisplay.windowWidth, pGame->gameDisplay.windowHeight, 0);
    if (!handleError(pGame, pGame->gameDisplay.pWindow, SDL_GetError)) return 0;

	Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	#if __APPLE__ || __LINUX__
		flags = (flags & !SDL_RENDERER_ACCELERATED) | SDL_RENDERER_SOFTWARE;
	#endif
    pGame->gameDisplay.pRenderer = SDL_CreateRenderer(pGame->gameDisplay.pWindow, -1, flags);
    if (!handleError(pGame, pGame->gameDisplay.pRenderer, SDL_GetError)) return 0;

    pGame->gameDisplay.pMenuFont = TTF_OpenFont("../assets/Ticketing.ttf", 25);
    if (!handleError(pGame, pGame->gameDisplay.pMenuFont, TTF_GetError)) return 0;

    pGame->gameDisplay.pMenuTexture = createPicture(&pGame->gameDisplay, MAIN_MENU_PICTURE);
    if (!handleError(pGame, pGame->gameDisplay.pMenuTexture, SDL_GetError)) {
        return 0;
    }
    pGame->gameDisplay.pBackgroundTexture = createPicture(&pGame->gameDisplay, BACKGROUND_PICTURE);
    if (!handleError(pGame, pGame->gameDisplay.pBackgroundTexture, SDL_GetError)) {
        return 0;
    }
    pGame->buttons.pButtonTexture = createPicture(&pGame->gameDisplay, BUTTON_PICTURE);
    if (!handleError(pGame, pGame->buttons.pButtonTexture, SDL_GetError)) {
        return 0;
    }
    pGame->gameDisplay.pPlatformTexture = createPicture(&pGame->gameDisplay, PLATFORM_PICTURE);
    if (!handleError(pGame, pGame->gameDisplay.pPlatformTexture, SDL_GetError)) {
        return 0;
    }
    pGame->gameDisplay.pStartPlatformTexture = createPicture(&pGame->gameDisplay, START_PLATFORM_PICTURE);
    if (!handleError(pGame, pGame->gameDisplay.pStartPlatformTexture, SDL_GetError)) {
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

    FILE *fp = NULL;
    readFromFileKey(fp, pGame->language.keybinds);
    //saveToFile(fp, pGame->keybinds);
    //pGame->chosenLanguage = true;
    initiateLanguage(fp, &pGame->language, &pGame->gameDisplay, &pGame->buttons, &pGame->displayText);

    pGame->state = MAIN_MENU;

    return 1;
}

void initiateLanguage(FILE* fp, Language* pLanguage, GameDisplay* pGameDisplay, Buttons* pButtons, DisplayText* pDisplayText) { // kan flyttas
    #if __APPLE__
		readFromFileLangMAC(pLanguage->chosenLanguage, pLanguage->language);
    #else
        readFromFileLang(fp, pLanguage->language);
	#endif

    readKeybindString(pLanguage, 0, pGameDisplay, pButtons);

    pButtons->pStartButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[0], 255, 255, 255, 0, 0);
    pButtons->pSettingsButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[1], 255, 255, 255, 0, 50);
    pButtons->pQuitButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[2], 255, 255, 255, 0, 100);
    pButtons->pResumeButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[3], 255, 255, 255, 0, 50);
    pButtons->pMainMenuButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[4], 255, 255, 255, 0, 100);
    pButtons->pLanguageButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[5], 255, 255, 255, 0, -100);
    pButtons->pReturnButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[6], 255, 255, 255, 0, 200);
    pButtons->pMoveRightButton1Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[7], 255, 255, 255, -80, 50);
    pButtons->pMoveLeftButton1Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[8], 255, 255, 255, -80, 100);
    pDisplayText->pYouAreDeadText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[9], 255, 255, 255, 0, -200);
    pButtons->pEnglishButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, "English", 255, 255, 255, 0, -50);
    pButtons->pSwedishButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, "Svenska", 255, 255, 255, 0, 0);
    pButtons->pCreateLobbyButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[10], 255, 255, 255, 0, 0);
    pButtons->pJoinLobbyButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[11], 255, 255, 255, 0, 50);
    pButtons->pMuteButton1Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[13], 255, 255, 255, -80, 150);
    pDisplayText->pWaitingText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[16], 255, 255, 255, 0, 50);
    pDisplayText->pEnterIPText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[17], 255, 255, 255, -130, 50);
}

int handleError(Game* pGame, void* pMember, const char* (*GetError)(void)) {
    if (!pMember) {
        printf("Error: %s\n", (*GetError)());
        quitGame(pGame);
        return 0;
    }
    else return 1;
}

void runGame(Game* pGame) {
    SDL_Event event;
    bool isRunning = true, mute = false;
    int time = 0;

    Mix_VolumeMusic(75);
    Mix_PlayMusic(pGame->pMainSound, -1);

    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU: handleMainMenu(&pGame->gameDisplay, &pGame->language, &pGame->buttons, event, &pGame->state, &mute);
                break;
            case SETTINGS_MENU: handleSettingsMenu(&pGame->gameDisplay, &pGame->language, &pGame->buttons, &pGame->displayText, event, &pGame->state);
                break;
            case ENTER_INPUT: handleEnterInput(&pGame->gameDisplay, &pGame->language, &pGame->buttons, event, &pGame->state);
                break;
            case LOBBY_MENU: handleLobbyMenu(pGame, &pGame->gameDisplay, pGame->pNetworkData, &pGame->buttons, &pGame->displayText, event, &pGame->state, pGame->pLobbyConnect, &time);
                break;
            case LOBBY: handleLobby(&pGame->gameDisplay, pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands, &pGame->buttons, &pGame->displayText, event, &pGame->state, pGame->pLobbyConnect);
                break;
            case ONGOING: handleOngoing(pGame, pGame->pPlayersData, pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands, &pGame->language, event, &pGame->state, &isRunning, &time, &mute);
                break;
            case GAME_MENU: handleGameMenu(&pGame->gameDisplay, &pGame->buttons, event, &pGame->state, &mute);
                break;
            case GAME_OVER: handleGameOver(pGame->pPlayersData, &pGame->gameDisplay, &pGame->language, &pGame->buttons, &pGame->displayText, event, &pGame->state);
                break;
            case QUIT: isRunning = false;
                break;
            default:
                break;
        }

        SDL_RenderPresent(pGame->gameDisplay.pRenderer);
    }
}

void handleMainMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute) { // kan flyttas
    bool buttonPressed = false;

    if (!(*pMute)) Mix_ResumeMusic();

    renderMainMenu(pGameDisplay, pButtons);

    while (SDL_PollEvent(&event)) {
        handleButton(pButtons->pStartButton, &buttonPressed);
        if (buttonPressed) {
            *pState = LOBBY_MENU;
            buttonPressed = false;
        }
        handleButton(pButtons->pSettingsButton, &buttonPressed);
        if (buttonPressed) {
            *pState = SETTINGS_MENU;
            buttonPressed = false;
        }
        handleButton(pButtons->pQuitButton, &buttonPressed);
        if (buttonPressed) {
            *pState = QUIT;
            buttonPressed = false;
        }

        switch (event.type) {
            case SDL_QUIT: *pState = QUIT;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == pLanguage->keybinds[2] && !(*pMute)) {
                    *pMute = true;
                    Mix_PauseMusic();
                }
                else if (event.key.keysym.sym == pLanguage->keybinds[2] && (*pMute)) {
                    *pMute = false;
                    Mix_ResumeMusic();
                }
                break;
        }
    }
}

void renderMainMenu(GameDisplay* pGameDisplay, Buttons* pButtons) { //kan flyttas
    renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);

    renderButton(pButtons->pStartButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pSettingsButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pQuitButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pStartButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pSettingsButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pQuitButtonText, pGameDisplay->pRenderer);
}

void handleSettingsMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState) { // kan flyttas
    bool buttonPressed = false;

    renderSettingsMenu(pGameDisplay, pButtons);
    if (pLanguage->showLang){
        renderLanguageMenu(pGameDisplay, pButtons);
    }

    while (SDL_PollEvent(&event)){
        handleButton(pButtons->pLanguageButton, &buttonPressed);
        if (buttonPressed) {
            (pLanguage->showLang) = true;
            buttonPressed = false;
        }
        handleButton(pButtons->pMoveRightButton, &buttonPressed);
        if (buttonPressed) {
            pLanguage->num = 0;
            readKeybindString(pLanguage, 1, pGameDisplay, pButtons);
            *pState = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pButtons->pMoveLeftButton, &buttonPressed);
        if (buttonPressed) {
            pLanguage->num = 1;
            readKeybindString(pLanguage, 2, pGameDisplay, pButtons);
            *pState = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pButtons->pMuteButton, &buttonPressed);
        if (buttonPressed) {
            pLanguage->num = 2;
            readKeybindString(pLanguage, 3, pGameDisplay, pButtons);
            *pState = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pButtons->pReturnButton, &buttonPressed);
        if (buttonPressed){
            *pState = MAIN_MENU;
            pLanguage->showLang = false;
            buttonPressed = false;
        }

        if ((pLanguage->showLang)){
            handleLanguageMenu(pGameDisplay, pLanguage, pButtons, pDisplayText, event);
        }

        if (event.type == SDL_QUIT) {
            *pState = QUIT;
        }
    }
}

void renderSettingsMenu(GameDisplay* pGameDisplay, Buttons* pButtons) { //kan flyttas
    renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);

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

void handleLanguageMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event) { // kan flyttas
    bool buttonPressed = false;
    char chosenLang[LANG_LENGTH];
    FILE *fp = NULL;

    handleButton(pButtons->pEnglishButton, &buttonPressed);
    if (buttonPressed) {
        strcpy(chosenLang, "english.txt");
        pLanguage->showLang = false;
        buttonPressed = false;
    }
    handleButton(pButtons->pSwedishButton, &buttonPressed);
    if (buttonPressed) {
        strcpy(chosenLang, "svenska.txt");
        pLanguage->showLang = false;
        buttonPressed = false;
    }

    if (!(pLanguage->showLang)) {
        changeLanguageInFile(fp, chosenLang);
        /*if (strcmp(chosenLang, "svenska.txt")) {
            pLanguage->chosenLanguage = true;
        }
        else {
            pLanguage->chosenLanguage = false;
        }*/
        initiateLanguage(fp, pLanguage, pGameDisplay, pButtons, pDisplayText);
    }
}

void renderLanguageMenu(GameDisplay* pGameDisplay, Buttons* pButtons) { //kan flyttas
    renderButton(pButtons->pEnglishButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pSwedishButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pEnglishButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pSwedishButtonText, pGameDisplay->pRenderer);
}

void readKeybindString(Language* pLanguage, int index, GameDisplay* pGameDisplay, Buttons* pButtons) { //kan flyttas
    SDL_Keycode moveRight = pLanguage->keybinds[0], moveLeft = pLanguage->keybinds[1], mute = pLanguage->keybinds[2];
    char keybindsText[3][30];

    if (index == 0) {
        strcpy(keybindsText[0], (SDL_GetKeyName(moveRight)));
        strcpy(keybindsText[1], (SDL_GetKeyName(moveLeft)));
        strcpy(keybindsText[2], (SDL_GetKeyName(mute)));

        pButtons->pMoveRightButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, keybindsText[0], 255, 255, 255, BUTTON_WIDTH / 2, 50);
        pButtons->pMoveLeftButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, keybindsText[1], 255, 255, 255, BUTTON_WIDTH / 2, 100);
        pButtons->pMuteButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, keybindsText[2], 255, 255, 255, BUTTON_WIDTH / 2, 150);
    }
    else if (index == 1) {
        pButtons->pMoveRightButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[12], 255, 255, 255, BUTTON_WIDTH / 2, 50);
    }
    else if (index == 2) {
        pButtons->pMoveLeftButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[12], 255, 255, 255, BUTTON_WIDTH / 2, 100);
    }
    else if (index == 3) {
        pButtons->pMuteButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[12], 255, 255, 255, BUTTON_WIDTH / 2, 150);
    }
}

void handleEnterInput(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState) { // kan flyttas
    FILE *fp = NULL;

    renderSettingsMenu(pGameDisplay, pButtons);

    while (SDL_PollEvent(&event)) {
        switch (event.type){
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    *pState = SETTINGS_MENU;
                }
                else {
                    pLanguage->keybinds[pLanguage->num] = (event.key.keysym.sym);
                    saveToFile(fp, pLanguage->keybinds);
                    *pState = SETTINGS_MENU;
                }
                readKeybindString(pLanguage, 0, pGameDisplay, pButtons);
                break;
        }
    }
}

void handleLobbyMenu(Game* pGame, GameDisplay* pGameDisplay, NetworkData* pNetworkData, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect, int* pTime) {
    bool buttonPressed = false;

    renderLobbyMenu(pGameDisplay, pButtons);

    if (pLobbyConnect->joined) {
        renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);
        renderText(pDisplayText->pEnterIPText, pGameDisplay->pRenderer);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if ((event.key.keysym.sym) == (SDLK_RETURN) ) {
                        setConnection(pNetworkData, pLobbyConnect->inputIP);
                        *pState = LOBBY;
                    }
                    else if ((event.key.keysym.sym) == (SDLK_BACKSPACE) && (pLobbyConnect->inputIPIndex) > 0) {
                        (pLobbyConnect->inputIPIndex)--;
                        pLobbyConnect->inputIP[pLobbyConnect->inputIPIndex] = '\0';
                    }
                    else if ((pLobbyConnect->inputIPIndex) < INPUT_IP_LEN) {
                        pLobbyConnect->inputIP[pLobbyConnect->inputIPIndex] = (event.key.keysym.sym);
                        (pLobbyConnect->inputIPIndex)++;
                    }
                break;
            }
            if (event.type == SDL_QUIT) {
                *pState = QUIT;
            }
        }
        if ((pLobbyConnect->inputIPIndex) > 0) {
            pDisplayText->pInputIPText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLobbyConnect->inputIP, 255, 255, 255, 70, 50);
            renderText(pDisplayText->pInputIPText, pGameDisplay->pRenderer);
        }
    } else {
        while (SDL_PollEvent(&event)) {
            handleButton(pButtons->pCreateLobbyButton, &buttonPressed);
            if (buttonPressed) {
                setConnection(pNetworkData, NULL);
                *pState = LOBBY;
                buttonPressed = false;
            }
            handleButton(pButtons->pJoinLobbyButton, &buttonPressed);
            if (buttonPressed) {
                pLobbyConnect->joined = true;
                buttonPressed = false;
            }
            handleButton(pButtons->pReturnButton, &buttonPressed);
            if (buttonPressed){
                *pState = MAIN_MENU;
                buttonPressed = false;
            }
            if (event.type == SDL_QUIT) {
                *pState = QUIT;
            }
        }
    }
    if (*pState == LOBBY) {
        resetGame(pGame, pGame->pPlayersData, pTime);
    }
}

void renderLobbyMenu(GameDisplay* pGameDisplay, Buttons* pButtons) { //kan flyttas
    renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);

    renderButton(pButtons->pCreateLobbyButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pJoinLobbyButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pReturnButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pCreateLobbyButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pJoinLobbyButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pReturnButtonText, pGameDisplay->pRenderer);
}

void handleLobby(GameDisplay* pGameDisplay, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect) { //kan flyttas
    bool buttonPressed = false;
    bool isHost = pNetworkData->isHost;
    char nrOfClients[2];

    renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);

    runNetcode(pNetworkData, pGameplayData, pClientCommands);

    if (isHost) { // SERVER
        renderButton(pButtons->pStartButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
        renderText(pButtons->pStartButtonText, pGameDisplay->pRenderer);

        sprintf(nrOfClients, "%d", pNetworkData->nClients);
        pDisplayText->pNrClientsText = createText(pGameDisplay, pGameDisplay->pMenuFont, nrOfClients, 255, 255, 255, 0, 70);
        renderText(pDisplayText->pNrClientsText, pGameDisplay->pRenderer);
        while (SDL_PollEvent(&event)) {
            handleButton(pButtons->pStartButton, &buttonPressed);
            if (buttonPressed) {
                *pState = ONGOING;
                buttonPressed = false;
                pGameplayData->gameState = ONGOING;
            }
            if (event.type == SDL_QUIT) {
                *pState = QUIT;
            }
        }
    } else { // CLIENT
        renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);
        renderText(pDisplayText->pWaitingText, pGameDisplay->pRenderer);
        renderButton(pButtons->pMainMenuButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
        renderText(pButtons->pMainMenuButtonText, pGameDisplay->pRenderer);
        while (SDL_PollEvent(&event)) {
            handleButton(pButtons->pMainMenuButton, &buttonPressed);
            if (buttonPressed) {
                *pState = MAIN_MENU;
                buttonPressed = false;
                pLobbyConnect->joined = false;
                //fillZero(pGame, INPUT_IP_LEN);
                //(*pIndex) = 0;
            }
            if (event.type == SDL_QUIT) {
                *pState = QUIT;
            }
        }
        if (pGameplayData->gameState == ONGOING) {
            *pState = ONGOING;
        }
    }
}

void fillZero(char inputIP[], int max) { // kan flyttas
    for (int i = 0; i < max; i++)
    {
        inputIP[i] = '\0';
    }
}

void handleOngoing(Game* pGame, PlayersData* pPlayersData, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, Language* pLanguage, SDL_Event event, State* pState, bool* pIsRunning, int *pTime, bool* pMute) { // vi kanske har kvar den i main.c?
    while (SDL_PollEvent(&event)) {
        handleOngoingInput(pPlayersData, pNetworkData, pClientCommands, pLanguage, &event, pState, pIsRunning, pMute);
    }

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool isHost = pNetworkData->isHost;

	if (isHost) {
		GameplayData temp;

        for(int i = 0; i < 6; i++){
            temp.players[i] = *pPlayersData->pPlayers[i];
        }

        // for(int i = 0; i < 30; i++){
        //     if(pGame->pPlatforms[i]){
        //         temp.platforms[i] = *pGame->pPlatforms[i];
        //         temp.platforms[i].created = true;
        //     }
        //     else temp.platforms[i].created = false;
        // }
		temp.nrOfPlayers = pPlayersData->nrOfPlayers;
		temp.nrOfPlayersLeft = pPlayersData->nrOfPlayersLeft;

        temp.gameState = *pState;
		// SERVER: HÄR SKA PUNKT (1) UTFÖRAS
		// Det är bara att lägga in datan direkt i "temp".

		*pGameplayData = temp;
	}

	runNetcode(pNetworkData, pGameplayData, pClientCommands);

	if (isHost) {
		// SERVER (4)
		for (int i = 0; pClientCommands[i].ip.host && i < COMMAND_BUFFER; i++) {
			// printf("looking at command at index %d\n", i);
			// printf("looking for host %d\n", pGame->pClientCommands[i].ip.host);
			// printf("looking for port %d\n", pGame->pClientCommands[i].ip.port);
			int iPlayer = 1;
			while ((pClientCommands[i].ip.host != pPlayersData->pPlayers[iPlayer]->ip.host
					|| pClientCommands[i].ip.port != pPlayersData->pPlayers[iPlayer]->ip.port)
					&& iPlayer < COMMAND_BUFFER) {
				// printf("comparing to index %d...\n", iPlayer);
				// printf("comparing to host %d\n", pGame->pPlayers[iPlayer]->ip.host);
				// printf("comparing to port %d\n", pGame->pPlayers[iPlayer]->ip.port);
				iPlayer++;
			}
			// printf("found player object associated with client command at index %d\n", iPlayer);
			ClientCommand tempCC = pClientCommands[i];
			Player *tempP = pPlayersData->pPlayers[iPlayer];
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
			pClientCommands[i] = (ClientCommand){0};
		}
	} else {
		for (int i = 0; i < 6; i++) {
			*pPlayersData->pPlayers[i] = pGameplayData->players[i];
		}
		pPlayersData->nrOfPlayers = pGameplayData->nrOfPlayers;
		pPlayersData->nrOfPlayersLeft = pGameplayData->nrOfPlayersLeft;

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
		// KLIENT: HÄR SKA PUNKT (B) UTFÖRAS
		// Datan är tillgänglig via pGame->GameplayData.

        /*
        Läggs in när matning av GameplayData (Punkt 1) är klar !!!

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

    handleBackground(pGame->pBackground, &pGame->gameDisplay, pGame->gameDisplay.pBackgroundTexture); //denna mï¿½ste ligga fï¿½re allt med player

	// KEEP THIS COMMENTED FOR NOW
    //handlePlatforms(pGame->pPlatforms, pGame->pRenderer, pGame->pPlatformTexture, pGame->windowWidth, pGame->windowHeight, isHost);

    handleStartPlatform(pGame->pStartPlatform, pGame->pPlatforms[0], pGame->gameDisplay.pRenderer, pGame->gameDisplay.pStartPlatformTexture, pGame->gameDisplay.windowHeight, pTime);
    handlePlayers(pPlayersData->pPlayers, pPlayersData->nrOfPlayers, &pPlayersData->nrOfPlayersLeft, pMute, pGame->gameDisplay.windowWidth, pGame->gameDisplay.windowHeight, pGame->pStartPlatform, pGame->pJumpSound, pGame->pWinSound, &pGame->state, pGame->gameDisplay.pRenderer, pPlayersData->pPlayerTextures, pGame->pPlatforms, pGame->displayText.pYouAreDeadText, &pGame->pNetworkData->isHost);

    SDL_Delay(3);
}

void handleOngoingInput(PlayersData* pPlayersData, NetworkData* pNetworkData, ClientCommand* pClientCommands, Language* pLanguage, SDL_Event* event, State* pState, bool* pIsRunning, bool* pMute) { // kan flyttas
	// Det visade sig att de events vi använder för att läsa input var lite mer komplicerat än vad jag
	// först trodde, så jag var tvungen att modifiera pClientCommands[0] direkt via dess pointer istället.
	if (!pNetworkData->isHost) {
        pClientCommands[0].commandType = MOVEMENT;
    }

    switch (event->type){
        case SDL_QUIT:
            *pIsRunning = false;
            pClientCommands[0].commandType = LEAVE;
            break;
        case SDL_KEYDOWN:
            if ((event->key.keysym.sym) == (SDLK_ESCAPE)){
                *pState = GAME_MENU;
            }
            else if ((event->key.keysym.sym) == pLanguage->keybinds[0]) {
                if (pNetworkData->isHost) {
					pPlayersData->pPlayers[0]->moveRight = true;
					pPlayersData->pPlayers[0]->flip = SDL_FLIP_NONE;
				} else {
					pClientCommands[0].direction = 1;
                    pClientCommands[0].flip = SDL_FLIP_NONE;
				}
            }
            else if ((event->key.keysym.sym) == pLanguage->keybinds[1]) {
                if (pNetworkData->isHost) {
					pPlayersData->pPlayers[0]->moveLeft = true;
					pPlayersData->pPlayers[0]->flip = SDL_FLIP_HORIZONTAL;
				} else {
					pClientCommands[0].direction = -1;
                    pClientCommands[0].flip = SDL_FLIP_HORIZONTAL;
				}
            }
            else if ((event->key.keysym.sym) == pLanguage->keybinds[2] && !(*pMute)) {
                *pMute = true;
                Mix_PauseMusic();
            }
            else if ((event->key.keysym.sym) == pLanguage->keybinds[2] && (*pMute)) {
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
            if ((event->key.keysym.sym) == pLanguage->keybinds[0]) {
                if (pNetworkData->isHost) {
					pPlayersData->pPlayers[0]->moveRight = false;
				} else {
					pClientCommands[0].direction = 0;
				}
            }
            else if ((event->key.keysym.sym) == pLanguage->keybinds[1]) {
                if (pNetworkData->isHost) {
					pPlayersData->pPlayers[0]->moveLeft = false;
				} else {
					pClientCommands[0].direction = 0;
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

void handleGameOver(PlayersData* pPlayersData, GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButton, DisplayText* pDisplayText, SDL_Event event, State* pState) { // klar att flyttas
    int i;
    static bool whoWonTextCreated = false;
    char whoWonString[50];
    bool buttonPressed = false;

    Mix_PauseMusic();

    for(i = 0; i < pPlayersData->nrOfPlayers; i++) {
        if (pPlayersData->pPlayers[i]->alive) {
            break;
        }
    }
    if(!whoWonTextCreated) {
        sprintf(whoWonString, "%s %d %s", pLanguage->language[14], i + 1, pLanguage->language[15]);
        pDisplayText->pWhoWonText = createText(pGameDisplay, pGameDisplay->pMenuFont, whoWonString, 255, 255, 255, 0, -300);
        whoWonTextCreated = true;
    }

    renderGameOver(pGameDisplay, pButton, pDisplayText->pWhoWonText);

    while (SDL_PollEvent(&event)) {
        handleButton(pButton->pMainMenuButton, &buttonPressed);
        if (buttonPressed) {
            destroyText(pDisplayText->pWhoWonText);
            whoWonTextCreated = false;
            *pState = MAIN_MENU;
            buttonPressed = false;
        }

        if (event.type == SDL_QUIT) {
            *pState = QUIT;
        }
    }
}

void renderGameOver(GameDisplay* pGameDisplay, Buttons* pButtons, Text* pWhoWonText) { //kan flyttas
    renderButton(pButtons->pMainMenuButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pMainMenuButtonText, pGameDisplay->pRenderer);
    renderText(pWhoWonText, pGameDisplay->pRenderer);
}

void resetGame(Game* pGame, PlayersData* pPlayersData, int* pTime) {
    char avatar[6][25] = {"../assets/player1.png", "../assets/player2.png", "../assets/player3.png", "../assets/player4.png", "../assets/player5.png", "../assets/player6.png"};
    int subtractXPos = -100;
    int increaseXPos = 0;

    resetPlatforms(pGame->pPlatforms);
    resetStartPlatform(pGame->pStartPlatform, pGame->gameDisplay.windowHeight, pTime);
    resetPlayers(pPlayersData->pPlayers, &pPlayersData->nrOfPlayers, &pPlayersData->nrOfPlayersLeft);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		initPlayer(pPlayersData->pPlayers, &pPlayersData->nrOfPlayers, &pPlayersData->nrOfPlayersLeft, &pGame->gameDisplay, pGame->pStartPlatform->yPos, pPlayersData->pPlayerTextures, &subtractXPos, &increaseXPos, avatar[i]);
	}
}

void quitGame(Game* pGame) {

    destroyPlayers(pGame->pPlayersData->pPlayers);
    destroyPlatform(pGame->pStartPlatform);
    destroyPlatforms(pGame->pPlatforms);
    destroyBackground(pGame->pBackground);

    destroyChunk(pGame->pWinSound);
    destroyChunk(pGame->pJumpSound);
    destroyMusic(pGame->pMainSound);

    destroyText(pGame->displayText.pYouAreDeadText);
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

    destroyPlayerTextures(pGame->pPlayersData->pPlayerTextures);
    destroyTexture(pGame->gameDisplay.pStartPlatformTexture);
    destroyTexture(pGame->gameDisplay.pPlatformTexture);
    destroyTexture(pGame->gameDisplay.pBackgroundTexture);
    destroyTexture(pGame->gameDisplay.pMenuTexture);

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