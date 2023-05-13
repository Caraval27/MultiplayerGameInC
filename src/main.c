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
    pGame->windowWidth = displayMode.w;
    pGame->windowHeight = displayMode.h;

	pGame->pNetworkData = malloc(sizeof(NetworkData));
	pGame->pGameplayData = malloc(sizeof(GameplayData));
	*pGame->pNetworkData = (NetworkData){0};
	*pGame->pGameplayData = (GameplayData){0};
	*pGame->pClientCommands = (ClientCommand){0};
	pGame->pNetworkData->pPlayers = pGame->pPlayers;
	initializeNetcode(pGame->pNetworkData);

    pGame->pWindow = SDL_CreateWindow("Mental breakdown", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->windowWidth, pGame->windowHeight, 0);
    if (!handleError(pGame, pGame->pWindow, SDL_GetError)) return 0;

	Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	#if __APPLE__ || __LINUX__
		flags = (flags & !SDL_RENDERER_ACCELERATED) | SDL_RENDERER_SOFTWARE;
	#endif
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, flags);
    if (!handleError(pGame, pGame->pRenderer, SDL_GetError)) return 0;

    pGame->pMenuFont = TTF_OpenFont("../assets/Ticketing.ttf", 25);
    if (!handleError(pGame, pGame->pMenuFont, TTF_GetError)) return 0;

    pGame->pMenuTexture = createPicture(pGame->pWindow, pGame->pRenderer, MAIN_MENU_PICTURE);
    if (!handleError(pGame, pGame->pMenuTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pBackgroundTexture = createPicture(pGame->pWindow, pGame->pRenderer, BACKGROUND_PICTURE);
    if (!handleError(pGame, pGame->pBackgroundTexture, SDL_GetError)) {
        return 0;
    }
    pGame->pButtonTexture = createPicture(pGame->pWindow, pGame->pRenderer, BUTTON_PICTURE);
    if (!handleError(pGame, pGame->pButtonTexture, SDL_GetError)) {
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

    pGame->pBackground = createBackground(pGame->windowHeight);

    pGame->pStartButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pSettingsButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pQuitButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pCreateLobbyButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 0, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pJoinLobbyButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pLanguageButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 - 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveRightButton = createButton(pGame->windowWidth / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMoveLeftButton = createButton(pGame->windowWidth / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMuteButton = createButton(pGame->windowWidth / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 150, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pReturnButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 200, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pEnglishButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 - 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pSwedishButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pResumeButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pGame->pMainMenuButton = createButton((pGame->windowWidth - BUTTON_WIDTH) / 2, (pGame->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);

    pGame->pStartPlatform = createPlatform(0, pGame->windowHeight - 100, pGame->windowWidth, 100);
    pGame->flip = SDL_FLIP_NONE;

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
    initiateLanguage(fp, pGame);

    pGame->state = MAIN_MENU;

    return 1;
}

void initiateLanguage(FILE *fp, Game *pGame){
    #if __APPLE__
		readFromFileLangMAC(pGame->chosenLanguage, pGame->language);
    #else
        readFromFileLang(fp, pGame->language);
	#endif

    readKeybindString(pGame, 0);

    pGame->pStartButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[0], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pSettingsButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[1], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 50);
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[2], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 100);
    pGame->pResumeButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[3], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 50);
    pGame->pMainMenuButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[4], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 100);
    pGame->pLanguageButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[5], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -100);
    pGame->pReturnButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[6], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 200);
    pGame->pMoveRightButton1Text = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[7], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, -80, 50);
    pGame->pMoveLeftButton1Text = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[8], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, -80, 100);
    pGame->pYouAreDeadText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[9], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -200);
    pGame->pEnglishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, "English", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -50);
    pGame->pSwedishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, "Svenska", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pCreateLobbyButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[10], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pJoinLobbyButtonText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[11], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 50);
    pGame->pMuteButton1Text = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[13], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, -80, 150);
    pGame->pWaitingText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[16], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 50);
    pGame->pEnterIPText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[17], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, -130, 50);
}

int handleError(Game* pGame, void* pMember, const char* (*GetError)(void)){
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
            case LOBBY_MENU: handleLobbyMenu(pGame, event, &left, &right, &time, &createLobbyPressed);
                break;
            case LOBBY: handleLobby(pGame, event, &joined, &inputIPIndex, &createLobbyPressed);
                break;
            case ONGOING: handleOngoing(pGame, event, &isRunning, &left, &right, &time, &mute);
                break;
            case GAME_MENU: handleGameMenu(pGame, event, &mute);
                break;
            case GAME_OVER: handleGameOver(pGame, event);
                break;
            case QUIT: isRunning = false;
                break;
            default:
                break;
        }

        SDL_RenderPresent(pGame->pRenderer);
    }
}

void handleMainMenu(Game* pGame, SDL_Event event, bool* pMute){
    bool buttonPressed = false;

    if (!(*pMute)) Mix_ResumeMusic();

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

void renderMainMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pStartButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pSettingsButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pQuitButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pStartButtonText, pGame->pRenderer);
    renderText(pGame->pSettingsButtonText, pGame->pRenderer);
    renderText(pGame->pQuitButtonText, pGame->pRenderer);
}

void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum, bool *pShowLang){
    bool buttonPressed = false;

    renderSettingsMenu(pGame);
    if (*pShowLang){
        renderLanguageMenu(pGame);
    }

    while (SDL_PollEvent(&event)){
        handleButton(pGame->pLanguageButton, &buttonPressed);
        if (buttonPressed) {
            (*pShowLang) = true;
            buttonPressed = false;
        }
        handleButton(pGame->pMoveRightButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 0;
            readKeybindString(pGame, 1);
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->pMoveLeftButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 1;
            readKeybindString(pGame, 2);
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->pMuteButton, &buttonPressed);
        if (buttonPressed) {
            *pNum = 2;
            readKeybindString(pGame, 3);
            pGame->state = ENTER_INPUT;
            buttonPressed = false;
        }
        handleButton(pGame->pReturnButton, &buttonPressed);
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

void renderSettingsMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pLanguageButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pMoveLeftButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pMoveRightButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pReturnButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pMuteButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pLanguageButtonText, pGame->pRenderer);
    renderText(pGame->pMoveLeftButton1Text, pGame->pRenderer);
    renderText(pGame->pMoveLeftButton2Text, pGame->pRenderer);
    renderText(pGame->pMoveRightButton1Text, pGame->pRenderer);
    renderText(pGame->pMoveRightButton2Text, pGame->pRenderer);
    renderText(pGame->pMuteButton1Text, pGame->pRenderer);
    renderText(pGame->pMuteButton2Text, pGame->pRenderer);
    renderText(pGame->pReturnButtonText, pGame->pRenderer);
}

void handleLanguageMenu(Game* pGame, SDL_Event event, bool* pShowLang){
    bool buttonPressed = false;
    char chosenLang[LANG_LENGTH];
    FILE *fp;

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

    if (!(*pShowLang)){
        changeLanguageInFile(fp, chosenLang);
        /*if (strcmp(chosenLang, "svenska.txt")) {
            pGame->chosenLanguage = true;
        }
        else {
            pGame->chosenLanguage = false;
        }*/
        initiateLanguage(fp, pGame);
    }
}

void renderLanguageMenu(Game* pGame){
    renderButton(pGame->pEnglishButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pSwedishButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pEnglishButtonText, pGame->pRenderer);
    renderText(pGame->pSwedishButtonText, pGame->pRenderer);
}

void readKeybindString(Game *pGame, int index){
    SDL_Keycode moveRight = pGame->keybinds[0], moveLeft = pGame->keybinds[1], mute = pGame->keybinds[2];
    char keybindsText[3][30];

    if (index == 0) {
        strcpy(keybindsText[0], (SDL_GetKeyName(moveRight)));
        strcpy(keybindsText[1], (SDL_GetKeyName(moveLeft)));
        strcpy(keybindsText[2], (SDL_GetKeyName(mute)));

        pGame->pMoveRightButton2Text = createText(pGame->pRenderer, pGame->pMenuFont, keybindsText[0], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, BUTTON_WIDTH / 2, 50);
        pGame->pMoveLeftButton2Text = createText(pGame->pRenderer, pGame->pMenuFont, keybindsText[1], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, BUTTON_WIDTH / 2, 100);
        pGame->pMuteButton2Text = createText(pGame->pRenderer, pGame->pMenuFont, keybindsText[2], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, BUTTON_WIDTH / 2, 150);
    }
    else if (index == 1) {
        pGame->pMoveRightButton2Text = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[12], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, BUTTON_WIDTH / 2, 50);
    }
    else if (index == 2) {
        pGame->pMoveLeftButton2Text = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[12], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, BUTTON_WIDTH / 2, 100);
    }
    else if (index == 3) {
        pGame->pMuteButton2Text = createText(pGame->pRenderer, pGame->pMenuFont, pGame->language[12], 255, 255, 255, pGame->windowWidth, pGame->windowHeight, BUTTON_WIDTH / 2, 150);
    }
}

void handleEnterInput(Game* pGame, SDL_Event event, int* pNum){
    FILE *fp;

    renderSettingsMenu(pGame);

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
                readKeybindString(pGame, 0);
                break;
        }
    }
}

void handleLobbyMenu(Game* pGame, SDL_Event event, bool* pLeft, bool* pRight, int* pTime, bool* pCreateLobbyPressed){
    bool buttonPressed = false;

    renderLobbyMenu(pGame);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pCreateLobbyButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = LOBBY;
            *pCreateLobbyPressed = true;
            buttonPressed = false;
        }
        handleButton(pGame->pJoinLobbyButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = LOBBY;
            buttonPressed = false;
        }
        handleButton(pGame->pReturnButton, &buttonPressed);
        if (buttonPressed){
            pGame->state = MAIN_MENU;
            buttonPressed = false;
        }

        if (pGame->state == LOBBY) {
            resetGame(pGame, pLeft, pRight, pTime);
        }

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}
//test

void renderLobbyMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pCreateLobbyButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pJoinLobbyButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pReturnButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pCreateLobbyButtonText, pGame->pRenderer);
    renderText(pGame->pJoinLobbyButtonText, pGame->pRenderer);
    renderText(pGame->pReturnButtonText, pGame->pRenderer);
}

void handleLobby(Game* pGame, SDL_Event event, bool* pJoined, int* pIndex, bool* pCreateLobbyPressed) {
    bool buttonPressed = false;
    bool isHost = pGame->pNetworkData->isHost;
    char nrOfClients[2];

    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    if (*pJoined) {
        runNetcode(pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands);
    }
    if (*pCreateLobbyPressed) {
        setConnection(pGame->pNetworkData, NULL);
    }

    if (isHost) { // SERVER
        renderButton(pGame->pStartButton, pGame->pRenderer, pGame->pButtonTexture);
        renderText(pGame->pStartButtonText, pGame->pRenderer);
        while (SDL_PollEvent(&event)) {
            handleButton(pGame->pStartButton, &buttonPressed);
            if (buttonPressed) {
                pGame->state = ONGOING;
                buttonPressed = false;
                pGame->pGameplayData->start = true;
            }
            if (event.type == SDL_QUIT) {
                pGame->state = QUIT;
            }
        }
        sprintf(nrOfClients, "%d", pGame->pNetworkData->nClients);
        pGame->pNrClientsText = createText(pGame->pRenderer, pGame->pMenuFont, nrOfClients, 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, 70);
        renderText(pGame->pNrClientsText, pGame->pRenderer);
    } else { // CLIENT
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if ((event.key.keysym.sym) == (SDLK_RETURN) ) {
                        setConnection(pGame->pNetworkData, pGame->inputIP);
                        *pJoined = true;
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
            handleButton(pGame->pMainMenuButton, &buttonPressed);
            if (buttonPressed) {
                pGame->state = MAIN_MENU;
                buttonPressed = false;
                (*pJoined) = false;
                fillZero(pGame, INPUT_IP_LEN);
                (*pIndex) = 0;
            }
            if (event.type == SDL_QUIT) {
                pGame->state = QUIT;
            }
        }
        if ((*pIndex) > 0) {
            pGame->pInputIPText = createText(pGame->pRenderer, pGame->pMenuFont, pGame->inputIP, 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 70, 50);
            renderText(pGame->pInputIPText, pGame->pRenderer);
        }
        if (*pJoined) {
            renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);
            renderText(pGame->pWaitingText, pGame->pRenderer);
            renderButton(pGame->pMainMenuButton, pGame->pRenderer, pGame->pButtonTexture);
            renderText(pGame->pMainMenuButtonText, pGame->pRenderer);
        } else {
            renderText(pGame->pEnterIPText, pGame->pRenderer);
        }
        if (pGame->pGameplayData->start) {
            pGame->state = ONGOING;
        }
    }
}

void fillZero(Game* pGame, int max) {
    for (int i = 0; i < max; i++)
    {
        pGame->inputIP[i] = '\0';
    }
}

void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pLeft, bool *pRight, int *pTime, bool* pMute){
    while (SDL_PollEvent(&event)){
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
		// SERVER: HÄR SKA PUNKT (1) UTFÖRAS
		// Det är bara att lägga in datan direkt i "temp".

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

    handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight); //denna mï¿½ste ligga fï¿½re allt med player

	// KEEP THIS COMMENTED FOR NOW
    //handlePlatforms(pGame->pPlatforms, pGame->pRenderer, pGame->pPlatformTexture, pGame->windowWidth, pGame->windowHeight, isHost);

    handleStartPlatform(pGame->pStartPlatform, pGame->pPlatforms[0], pGame->pRenderer, pGame->pStartPlatformTexture, pGame->windowHeight, pTime);
    handlePlayers(pGame->pPlayers, pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pLeft, pRight, pMute, pGame->windowWidth, pGame->windowHeight, pGame->pStartPlatform, pGame->pJumpSound, pGame->pWinSound, &pGame->state, pGame->pRenderer, pGame->pPlayerTextures, pGame->flip, pGame->pPlatforms, pGame->pYouAreDeadText, &pGame->pNetworkData->isHost);

    SDL_Delay(3);
}

void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pLeft, bool* pRight, bool* pMute) {
	// Det visade sig att de events vi använder för att läsa input var lite mer komplicerat än vad jag
	// först trodde, så jag var tvungen att modifiera pClientCommands[0] direkt via dess pointer istället.
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
				}
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
                if (pGame->pNetworkData->isHost) {
					pGame->pPlayers[0]->moveLeft = true;
					pGame->pPlayers[0]->flip = SDL_FLIP_HORIZONTAL;
				} else {
					pGame->pClientCommands[0].direction = -1;
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

void handleGameMenu(Game* pGame, SDL_Event event, bool* pMute){
    bool buttonPressed = false;

    Mix_PauseMusic();

    renderGameMenu(pGame);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pResumeButton, &buttonPressed);
        if (buttonPressed) {
            pGame->state = ONGOING;
            buttonPressed = false;
            if (!(*pMute)) Mix_ResumeMusic();
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
    renderButton(pGame->pResumeButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pMainMenuButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pMainMenuButtonText, pGame->pRenderer);
    renderText(pGame->pResumeButtonText, pGame->pRenderer);
}

void handleGameOver(Game* pGame, SDL_Event event){
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
        pGame->pWhoWonText = createText(pGame->pRenderer, pGame->pMenuFont, whoWonString, 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
        whoWonTextCreated = true;
    }

    renderGameOver(pGame);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pMainMenuButton, &buttonPressed);
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

void renderGameOver(Game* pGame){
    renderButton(pGame->pMainMenuButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pMainMenuButtonText, pGame->pRenderer);
    renderText(pGame->pWhoWonText, pGame->pRenderer);
}

void resetGame(Game* pGame, bool* pLeft, bool* pRight, int* pTime){
    char avatar[6][25] = {"../assets/player1.png", "../assets/player2.png", "../assets/player3.png", "../assets/player4.png", "../assets/player5.png", "../assets/player6.png"};
    int subtractXPos = -100;
    int increaseXPos = 0;

    resetPlatforms(pGame->pPlatforms);
    resetStartPlatform(pGame->pStartPlatform, pGame->windowHeight, pTime);
    resetPlayers(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		initPlayer(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pGame->windowWidth, pGame->pStartPlatform->yPos, pGame->pPlayerTextures, pGame->pWindow, pGame->pRenderer, &subtractXPos, &increaseXPos, avatar[i]);
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
    destroyText(pGame->pMainMenuButtonText);
    destroyText(pGame->pResumeButtonText);
    destroyText(pGame->pJoinLobbyButtonText);
    destroyText(pGame->pCreateLobbyButtonText);
    destroyText(pGame->pSwedishButtonText);
    destroyText(pGame->pEnglishButtonText);
    destroyText(pGame->pReturnButtonText);
    destroyText(pGame->pMuteButton2Text);
    destroyText(pGame->pMuteButton1Text);
    destroyText(pGame->pMoveLeftButton2Text);
    destroyText(pGame->pMoveLeftButton1Text);
    destroyText(pGame->pMoveRightButton2Text);
    destroyText(pGame->pMoveRightButton1Text);
    destroyText(pGame->pLanguageButtonText);
    destroyText(pGame->pQuitButtonText);
    destroyText(pGame->pSettingsButtonText);
    destroyText(pGame->pStartButtonText);

    destroyButton(pGame->pMainMenuButton);
    destroyButton(pGame->pResumeButton);
    destroyButton(pGame->pJoinLobbyButton);
    destroyButton(pGame->pCreateLobbyButton);
    destroyButton(pGame->pSwedishButton);
    destroyButton(pGame->pEnglishButton);
    destroyButton(pGame->pReturnButton);
    destroyButton(pGame->pMoveLeftButton);
    destroyButton(pGame->pMoveRightButton);
    destroyButton(pGame->pLanguageButton);
    destroyButton(pGame->pQuitButton);
    destroyButton(pGame->pSettingsButton);
    destroyButton(pGame->pStartButton);

    destroyPlayerTextures(pGame->pPlayerTextures);
    destroyTexture(pGame->pStartPlatformTexture);
    destroyTexture(pGame->pPlatformTexture);
    destroyTexture(pGame->pBackgroundTexture);
    destroyTexture(pGame->pMenuTexture);

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