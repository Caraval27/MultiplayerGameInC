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

int initiateGame(Game* pGame) {
    srand(time(NULL));

    if (!initiateSDLLibraries()) return 0;

    pGame->gameDisplay.windowWidth = 1440;
    pGame->gameDisplay.windowHeight = 900;

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

    initiateButtons(&pGame->buttons, &pGame->gameDisplay);

    pGame->pStartPlatform = createPlatform(0, pGame->gameDisplay.windowHeight - 100, pGame->gameDisplay.windowWidth, 100);

    pGame->music.pMainSound = Mix_LoadMUS("../assets/MainThemeSoundtrack.mp3");
    if (!handleError(pGame, pGame->music.pMainSound, Mix_GetError)) return 0;
    pGame->music.pJumpSound = Mix_LoadWAV("../assets/JumpEffect.wav");
    if (!handleError(pGame, pGame->music.pJumpSound, Mix_GetError)) return 0;
    pGame->music.pWinSound = Mix_LoadWAV("../assets/tempWinSound.wav");
    if (!handleError(pGame, pGame->music.pWinSound, Mix_GetError)) return 0;

    FILE *fp = NULL;
    readFromFileKey(fp, pGame->language.keybinds);
    //saveToFile(fp, pGame->keybinds);
    //pGame->chosenLanguage = true;
    initiateLanguage(fp, &pGame->language, &pGame->gameDisplay, &pGame->buttons, &pGame->displayText);

    pGame->state = MAIN_MENU;

    return 1;
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
    Mix_PlayMusic(pGame->music.pMainSound, -1);

    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU: handleMainMenu(&pGame->gameDisplay, &pGame->language, &pGame->buttons, event, &pGame->state, &mute);
                break;
            case SETTINGS_MENU: handleSettingsMenu(&pGame->gameDisplay, &pGame->language, &pGame->buttons, &pGame->displayText, event, &pGame->state);
                break;
            case ENTER_INPUT: handleEnterInput(&pGame->gameDisplay, &pGame->language, &pGame->buttons, event, &pGame->state);
                break;
            case LOBBY_MENU: handleLobbyMenu(&pGame->gameDisplay, pGame->pPlayersData, pGame->pNetworkData, &pGame->buttons, &pGame->displayText, event, &pGame->state, pGame->pLobbyConnect, pGame->pPlatforms, pGame->pStartPlatform, &time);
                break;
            case LOBBY: handleLobby(&pGame->gameDisplay, pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands, &pGame->buttons, &pGame->displayText, event, &pGame->state, pGame->pLobbyConnect);
                break;
            case ONGOING: handleOngoing(&pGame->gameDisplay, pGame->pPlayersData, pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands, &pGame->displayText, &pGame->language, &pGame->music, event, &pGame->state, pGame->pBackground, pGame->pPlatforms, pGame->pStartPlatform, &isRunning, &time, &mute);
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

void quitGame(Game* pGame) {

    destroyPlayers(pGame->pPlayersData->pPlayers);
    destroyPlatform(pGame->pStartPlatform);
    destroyPlatforms(pGame->pPlatforms);
    destroyBackground(pGame->pBackground);

    quitMusic(&pGame->music);
    quitText(&pGame->buttons, &pGame->displayText);
    quitButton(&pGame->buttons);
    quitTexture(&pGame->gameDisplay, pGame->pPlayersData);

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