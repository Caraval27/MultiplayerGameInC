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

    pGame->pYouAreDeadText = createText(pGame->pRenderer, pGame->pMenuFont, "You are dead", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -200);
    pGame->pWhoWonTexts[0] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 0 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[1] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 1 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[2] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 2 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[3] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 3 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[4] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 4 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[6] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 6 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[7] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 7 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[8] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 8 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[9] = createText(pGame->pRenderer, pGame->pMenuFont, "Player 9 won", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);
    pGame->pWhoWonTexts[10] = createText(pGame->pRenderer, pGame->pMenuFont, "You won!", 255, 255, 255, pGame->windowWidth, pGame->windowHeight, 0, -300);

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
    bool isRunning = true, left = false, right = false, mute = false, showLang = false;
    int num, time = 0;

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
            case LOBBY_MENU: handleLobbyMenu(pGame, event, &left, &right, &time);
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

void handleLobbyMenu(Game* pGame, SDL_Event event, bool* pLeft, bool* pRight, int* pTime){
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

        if (pGame->state == ONGOING) {
            resetGame(pGame, pLeft, pRight, pTime);
        }

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderLobbyMenu(Game* pGame){
    renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);

    renderButton(pGame->pCreateLobbyButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pJoinLobbyButton, pGame->pRenderer, pGame->pButtonTexture);
    renderButton(pGame->pReturnButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pCreateLobbyButtonText, pGame->pRenderer);
    renderText(pGame->pJoinLobbyButtonText, pGame->pRenderer);
    renderText(pGame->pReturnButtonText, pGame->pRenderer);
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

        for(int i = 0; i < pGame->nrOfPlayers; i++){
            temp.players[i] = *pGame->pPlayers[i];
        }

        temp.gameState = pGame->state;
		// SERVER: HÄR SKA PUNKT (1) UTFÖRAS
		// Det är bara att lägga in datan direkt i "temp".

		*pGame->pGameplayData = temp;
	}

	runNetcode(pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands);

	if (isHost) {

		// SERVER: HÄR SKA PUNKT (4) UTFÖRAS
		// Alla kommandon finns tillgängliga i en array vid pGame->pClientCommands.
		// Varje kommando innehåller en IP-adress. Ni kommer behöva leta efter
		// spelarobjektet som har denna adress genom pGame->pPlayers[i]->ip.

		*pGame->pClientCommands = (ClientCommand){0};
	} else {

		// KLIENT: HÄR SKA PUNKT (B) UTFÖRAS
		// Datan är tillgänglig via pGame->GameplayData.

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

    handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight); //denna mï¿½ste ligga fï¿½re allt med player
    handlePlatforms(pGame->pPlatforms, pGame->pRenderer, pGame->pPlatformTexture, pGame->windowWidth, pGame->windowHeight);
    handleStartPlatform(pGame->pStartPlatform, pGame->pPlatforms[0], pGame->pRenderer, pGame->pStartPlatformTexture, pGame->windowHeight, pTime);
    handlePlayers(pGame->pPlayers, pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pLeft, pRight, pMute, pGame->windowWidth, pGame->windowHeight, pGame->pStartPlatform, pGame->pJumpSound, pGame->pWinSound, &pGame->state, pGame->pRenderer, pGame->pPlayerTextures, pGame->flip, pGame->pPlatforms, pGame->pYouAreDeadText);

    SDL_Delay(3);
}

void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pLeft, bool* pRight, bool* pMute) {
    ClientCommand tempClient = {0, MOVEMENT, 0};

    switch (event->type){
        case SDL_QUIT:
            *pIsRunning = false;
            tempClient.commandType = LEAVE;
            break;
        case SDL_KEYDOWN:
            if ((event->key.keysym.sym) == (SDLK_ESCAPE)){
                pGame->state = GAME_MENU;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[0]) {
                *pRight = true;
                pGame->flip = SDL_FLIP_HORIZONTAL;
                tempClient.direction = 1;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
                *pLeft = true;
                pGame->flip = SDL_FLIP_HORIZONTAL;
                tempClient.direction = -1;
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
                *pRight = false;
                pGame->flip = SDL_FLIP_NONE;
                tempClient.direction = 0;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
                *pLeft = false;
                pGame->flip = SDL_FLIP_NONE;
                tempClient.direction = 0;
            }
            // switch(event->key.keysym.sym){
            //     case SDLK_LEFT: *pLeft = false;
            //         break;m
            //     case SDLK_RIGHT: *pRight = false;
            //         break;
            // }
            break;
    }
    pGame->pClientCommands[0].commandType = tempClient.commandType;
    pGame->pClientCommands[0].direction = tempClient.direction;
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
    bool buttonPressed = false;

    Mix_PauseMusic();

    renderGameOver(pGame);

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

void renderGameOver(Game* pGame){
    int i;

    renderButton(pGame->pMainMenuButton, pGame->pRenderer, pGame->pButtonTexture);

    renderText(pGame->pMainMenuButtonText, pGame->pRenderer);
    for(i = 0; i < pGame->nrOfPlayers; i++) {
        if (pGame->pPlayers[i]->alive) {
            break;
        }
    }
    if (i <= pGame->nrOfPlayers - 1) {
        renderText(pGame->pWhoWonTexts[i], pGame->pRenderer);
    }
    else {
        renderText(pGame->pWhoWonTexts[MAX_PLAYERS], pGame->pRenderer);
    }
}

void resetGame(Game* pGame, bool* pLeft, bool* pRight, int* pTime){
    int subtractXPos = -100;
    int increaseXPos = 0;

    resetPlatforms(pGame->pPlatforms);
    resetStartPlatform(pGame->pStartPlatform, pGame->windowHeight, pTime);
    resetPlayers(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pLeft, pRight);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		initPlayers(pGame->pPlayers, &pGame->nrOfPlayers, &pGame->nrOfPlayersLeft, pGame->windowWidth, pGame->pStartPlatform->yPos, pGame->pPlayerTextures, pGame->pWindow, pGame->pRenderer, &subtractXPos, &increaseXPos);
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

    destroyTexts(pGame->pWhoWonTexts);
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