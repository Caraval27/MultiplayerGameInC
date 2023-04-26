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
    //pGame->pPlayerTexture[0] = createPlayerCharacter(pGame->pRenderer, pGame->pWindow, characterPicture1);
    //if (!handleError(pGame, pGame->pPlayer1Texture, SDL_GetError)) return 0;
    //pGame->pPlayerTexture[1] = createPlayerCharacter(pGame->pRenderer, pGame->pWindow, characterPicture2);
    //if (!handleError(pGame, pGame->pPlayer2Texture, SDL_GetError)) return 0;

    pGame->pMenuFont = TTF_OpenFont("../assets/Ticketing.ttf", 25);
    if (!handleError(pGame, pGame->pWindow, TTF_GetError)) return 0;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("Error: %s\n", Mix_GetError());
        return 0;
    }
    pGame->pMainSound = Mix_LoadMUS("../assets/MainThemeSoundtrack.mp3");
    if (!handleError(pGame, pGame->pWindow, Mix_GetError)) return 0;
    pGame->pJumpSound = Mix_LoadWAV("../assets/JumpEffect.wav"); //for short sounds
    if (!handleError(pGame, pGame->pWindow, Mix_GetError)) return 0;

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

    pGame->pStartPlatform = createPlatform(0, pGame->windowHeight - 100, pGame->windowWidth, 100);

    pGame->pNrOfPlayers = MAX_PLAYERS;
    pGame->nrOfPlayersLeft = MAX_PLAYERS;
    
    initiatePlayers(pGame);
    /*int startPosition = 2;
    for(int i = 0; i < pGame->pNrOfPlayers; i++) { //måste vara -1 annars blir det malloc fel
        printf("%d\n", i);

        pGame->pPlayers[i] = createPlayer(pGame->windowWidth / startPosition, pGame->windowHeight, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED);
        pGame->pPlayerTextures[i] = createPicture(pGame->pWindow, pGame->pRenderer, CHARACTER_PICTURE); //gör en sträng av detta ist
        startPosition += 1;
    }*/

    FILE *fp;
    readFromFileKey(fp, pGame->keybinds);
    saveToFile(fp, pGame->keybinds);
    // KRASCHAR Pï¿½ MAC initiateLanguage(fp, pGame);

    pGame->state = MAIN_MENU;

    return 1;
}

void initiateLanguage(FILE *fp, Game *pGame){
    readFromFileLang(fp, pGame->language);
    pGame->pStartButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[0], pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pSettingsButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[1], pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pQuitButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[2], pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pResumeButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[3], pGame->windowWidth, pGame->windowHeight, 50, 0);
    pGame->pMainMenuButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[4], pGame->windowWidth, pGame->windowHeight, 100, 0);
    pGame->pLanguageButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[5], pGame->windowWidth, pGame->windowHeight, -100, 0);
    pGame->pEnglishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "English", pGame->windowWidth, pGame->windowHeight, -50, 0);
    pGame->pSwedishButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, "Svenska", pGame->windowWidth, pGame->windowHeight, 0, 0);
    pGame->pReturnButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[6], pGame->windowWidth, pGame->windowHeight, 200, 0);
    pGame->pMoveRight1ButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[7], pGame->windowWidth, pGame->windowHeight, 50, -80);
    pGame->pMoveLeft1ButtonText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[8], pGame->windowWidth, pGame->windowHeight, 100, -80);
    pGame->pGameOverText = createText(pGame->pRenderer, pGame->pMenuFont, 255, 255, 255, pGame->language[9], pGame->windowWidth, pGame->windowHeight, 100, -80);
}

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
            case MAIN_MENU: handleMainMenu(pGame, event, &time);
                break;
            case SETTINGS_MENU: handleSettingsMenu(pGame, event, &num);
                break;
            case ENTER_INPUT: handleEnterInput(pGame, event, &num);
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

void handleMainMenu(Game* pGame, SDL_Event event, int* pTime){
    Mix_ResumeMusic();

    renderMainMenu(pGame);

    while (SDL_PollEvent(&event)) {

        handleButton(pGame->pStartButton, &pGame->state, ONGOING);
        handleButton(pGame->pSettingsButton, &pGame->state, SETTINGS_MENU);
        handleButton(pGame->pQuitButton, &pGame->state, QUIT);

        resetGame(pGame, pTime);

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
    // KRASHCAR MAC renderText(pGame->pStartButtonText);
    // KRASCHAR MAC renderText(pGame->pSettingsButtonText);
    // KRASCHAR MAC renderText(pGame->pQuitButtonText);
}

void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum){
    char chosenLang[LANG_LENGTH];
    bool showLang = false;

    while (SDL_PollEvent(&event)){
        getMousePos(pGame->pLanguageButton);
        if (pGame->pLanguageButton->mouseXDelta <= BUTTON_WIDTH / 2 && pGame->pLanguageButton->mouseYDelta <= BUTTON_HEIGHT / 2 && pGame->pLanguageButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            showLang = !showLang;
        }
        getMousePos(pGame->pMoveRight2Button);
        if (pGame->pMoveRight2Button->mouseXDelta <= BUTTON_WIDTH / 2 && pGame->pMoveRight2Button->mouseYDelta <= BUTTON_HEIGHT / 2 && pGame->pMoveRight2Button->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            *pNum = 0;
            pGame->state = ENTER_INPUT;
        }
        getMousePos(pGame->pMoveLeft2Button);
        if (pGame->pMoveLeft2Button->mouseXDelta <= BUTTON_WIDTH / 2 && pGame->pMoveLeft2Button->mouseYDelta <= BUTTON_HEIGHT / 2 && pGame->pMoveLeft2Button->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            *pNum = 1;
            pGame->state = ENTER_INPUT;
        }
        getMousePos(pGame->pReturnButton);
        if (pGame->pReturnButton->mouseXDelta <= BUTTON_WIDTH / 2 && pGame->pReturnButton->mouseYDelta <= BUTTON_HEIGHT / 2 && pGame->pReturnButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
            pGame->state = MAIN_MENU;
            showLang = false;
        }

        renderMenu(pGame->pRenderer, pGame->pMenuTexture, pGame->windowWidth, pGame->windowHeight);
        if (showLang){
            getMousePos(pGame->pEnglishButton);
            if (pGame->pEnglishButton->mouseXDelta <= BUTTON_WIDTH / 2 && pGame->pEnglishButton->mouseYDelta <= BUTTON_HEIGHT / 2 && pGame->pEnglishButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
                showLang = false;
                strcpy(chosenLang, "english.txt");
            }
            getMousePos(pGame->pSwedishButton);
            if (pGame->pSwedishButton->mouseXDelta <= BUTTON_WIDTH / 2 && pGame->pSwedishButton->mouseYDelta <= BUTTON_HEIGHT / 2 && pGame->pSwedishButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)){
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

void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pRight, bool* pLeft, int *pTime){
    Mix_ResumeMusic();

    while (SDL_PollEvent(&event)){
        handleInputOngoing(pGame, &event, pIsRunning, pRight, pLeft);
    }

    handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight); //denna måste ligga före allt med player
    handlePlatforms(pGame->pPlatforms, pGame->pRenderer, pGame->pPlatformTexture, pGame->windowWidth);
    handleStartPlatform(pGame->pStartPlatform, pGame->pPlatforms[0], pGame->pPlayers[0], pGame->pRenderer, pGame->pStartPlatformTexture, pGame->windowHeight, pTime);
    handlePlayers(pGame, pLeft, pRight);

    SDL_Delay(1000/60);
}

void handleInputOngoing(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft){
    switch (event->type){
        case SDL_QUIT: *pIsRunning = false;
            break;
        case SDL_KEYDOWN:
            if ((event->key.keysym.sym) == (SDLK_ESCAPE)){
                pGame->state = GAME_MENU;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[0]) {
                *pRight = true;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
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
            if ((event->key.keysym.sym) == pGame->keybinds[0]) {
                *pRight = false;
            }
            else if ((event->key.keysym.sym) == pGame->keybinds[1]) {
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

    renderGameMenu(pGame);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pResumeButton, &pGame->state, ONGOING);
        handleButton(pGame->pMainMenuButton, &pGame->state, MAIN_MENU);

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }
}

void renderGameMenu(Game* pGame){
    renderButton(pGame->pResumeButton, pGame->pRenderer);
    renderButton(pGame->pMainMenuButton, pGame->pRenderer);
    // Gï¿½R Sï¿½ ATT MAN INTE KAN KOMMA TILL RESUMEMENU renderText(pGame->pMainMenuButtonText);
    // Gï¿½R Sï¿½ ATT MAN INTE KAN KOMMA TILL RESUMEMENU renderText(pGame->pResumeButtonText);
}

void handleGameOverMenu(Game* pGame, SDL_Event event){
    //Mix_PauseMusic();
    //renderText();
    renderButton(pGame->pMainMenuButton, pGame->pRenderer);

    while (SDL_PollEvent(&event)) {
        handleButton(pGame->pMainMenuButton, &pGame->state, MAIN_MENU);

        if (event.type == SDL_QUIT) {
            pGame->state = QUIT;
        }
    }

    // Gï¿½R Sï¿½ ATT MAN INTE KAN KOMMA TILL RESUMEMENU renderText(pGame->pMainMenuButtonText);
}

void initiatePlayers(Game* pGame){
    int startPosition = 2;
    for(int i = 0; i < pGame->pNrOfPlayers; i++) { //måste vara -1 annars blir det malloc fel
        printf("%d\n", i);

        pGame->pPlayers[i] = createPlayer(pGame->windowWidth / startPosition, pGame->windowHeight, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED);
        pGame->pPlayerTextures[i] = createPicture(pGame->pWindow, pGame->pRenderer, CHARACTER_PICTURE); //gör en sträng av detta ist
        startPosition += 1;
    }
}

void handlePlayers(Game* pGame, bool *pLeft, bool *pRight){

    for (int i = 0; i < pGame->pNrOfPlayers ; i++) //av någon anledning dyker inte player 2 upp, förmodligen pga samma bild och position, samt båda rör sig med tangenttrycken
    {
        if (i == 0) //bara för att prova om spelare 2 dyker upp i loopen
        {
            movePlayer(pGame->pPlayers[i], *pLeft, *pRight, pGame->windowWidth);
            jumpPlayer(pGame->pPlayers[i], pGame->pStartPlatform->yPos, pGame->pJumpSound);
            playerCollidePlatform(pGame->pPlayers[i], pGame->pPlatforms, pGame->pJumpSound);
            checkIfPlayerDead(pGame->pPlayers[i], pGame->windowHeight, &pGame->state, &pGame->nrOfPlayersLeft);
            renderPlayer(pGame->pPlayers[i], pGame->pRenderer, pGame->pPlayerTextures[i]);
            //if(!pGame->pPlayers[i]->alive) renderText(pGame->pGameOverText);

        }
        else
        {
            jumpPlayer(pGame->pPlayers[i], pGame->pStartPlatform->yPos, pGame->pJumpSound);
            playerCollidePlatform(pGame->pPlayers[i], pGame->pPlatforms, pGame->pJumpSound);
            checkIfPlayerDead(pGame->pPlayers[i], pGame->windowHeight, &pGame->state, &pGame->nrOfPlayersLeft);
            renderPlayer(pGame->pPlayers[i], pGame->pRenderer, pGame->pPlayerTextures[i]);
            //if(!pGame->pPlayers[i]->alive) renderText(pGame->pGameOverText);
        }

    }
}

void resetGame(Game* pGame, int* pTime){
    if (pGame->state == ONGOING) {
        int startPosition = 2;

        resetStartPlatform(pGame->pStartPlatform, pGame->windowHeight, pTime);
        resetPlatforms(pGame->pPlatforms);
        for(int i = 0; i < pGame->pNrOfPlayers -1 ; i++) { //måste vara -1 annars blir det malloc fel
            pGame->pPlayers[i] = createPlayer(pGame->windowWidth / startPosition, pGame->windowHeight, 60, 60, MOVE_SPEED, 400);
            pGame->pPlayerTextures[i] = createPicture(pGame->pWindow, pGame->pRenderer, CHARACTER_PICTURE); //gör en sträng av detta ist
            startPosition += 1;
        }
    }
}

void quitGame(Game* pGame){
    if (pGame->pJumpSound) {
        destroyChunk(pGame->pJumpSound);
    }
    if (pGame->pMainSound) {
        destroyMusic(pGame->pMainSound);
    }
    if (pGame->pMainMenuButtonText) {
        destroyText(pGame->pMainMenuButtonText);
    }
    if (pGame->pResumeButtonText) {
        destroyText(pGame->pResumeButtonText);
    }
    if (pGame->pQuitButtonText) {
        destroyText(pGame->pQuitButtonText);
    }
    if (pGame->pStartButtonText) {
        destroyText(pGame->pStartButtonText);
    }
    if (pGame->pStartPlatform) {
        destroyPlatform(pGame->pStartPlatform);
    }
    if (pGame->pPlayers[0]) {
        destroyPlayers(&pGame->pPlayers[0]);
    }
    if (pGame->pPlatforms[0]) {
        destroyPlatforms(pGame->pPlatforms);
    }
    if (pGame->pBackground) {
        destroyBackground(pGame->pBackground);
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
    //destro playerRenderer?
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}