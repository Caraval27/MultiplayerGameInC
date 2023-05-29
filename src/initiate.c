#include "../include/main.h"
#include "../include/initiate.h"
#include "../include/run.h"

int initiateSDLLibraries(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Error: %s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init()){
        printf("Error: %s\n", TTF_GetError());
        return 0;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error: %s\n", Mix_GetError());
        return 0;
    }
	if (SDLNet_Init() == -1) {
		printf("Error: %s\n", SDLNet_GetError());
		return 0;
	}
    return 1;
}

int initiateDisplay(Game* pGame, GameDisplay* pGameDisplay) {
    pGameDisplay->pWindow = SDL_CreateWindow("Asylum Escape", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGameDisplay->windowWidth, pGameDisplay->windowHeight, 0);
    if (!handleError(pGame, pGameDisplay->pWindow, SDL_GetError)) return 0;

	Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	#if __APPLE__ || __LINUX__
		flags = (flags & !SDL_RENDERER_ACCELERATED) | SDL_RENDERER_SOFTWARE;
	#endif
    pGameDisplay->pRenderer = SDL_CreateRenderer(pGameDisplay->pWindow, -1, flags);
    if (!handleError(pGame, pGame->gameDisplay.pRenderer, SDL_GetError)) return 0;

    pGameDisplay->pMenuFont = TTF_OpenFont("../assets/font.ttf", 25);
    if (!handleError(pGame, pGameDisplay->pMenuFont, TTF_GetError)) return 0;

    return 1;
}

int initiateTexture(Game* pGame, GameDisplay* pGameDisplay, Buttons* pButtons) {
    pGameDisplay->pMenuTexture = createPicture(pGameDisplay, MAIN_MENU_PICTURE);
    if (!handleError(pGame, pGameDisplay->pMenuTexture, SDL_GetError)) {
        return 0;
    }
    pGameDisplay->pBackgroundTexture = createPicture(pGameDisplay, BACKGROUND_PICTURE);
    if (!handleError(pGame, pGameDisplay->pBackgroundTexture, SDL_GetError)) {
        return 0;
    }
    pButtons->pButtonTexture = createPicture(pGameDisplay, BUTTON_PICTURE);
    if (!handleError(pGame, pButtons->pButtonTexture, SDL_GetError)) {
        return 0;
    }
    pGameDisplay->pPlatformTexture = createPicture(pGameDisplay, PLATFORM_PICTURE);
    if (!handleError(pGame, pGameDisplay->pPlatformTexture, SDL_GetError)) {
        return 0;
    }
    pGameDisplay->pStartPlatformTexture = createPicture(pGameDisplay, START_PLATFORM_PICTURE);
    if (!handleError(pGame, pGameDisplay->pStartPlatformTexture, SDL_GetError)) {
        return 0;
    }
    return 1;
}

int initiateSound(Game* pGame, Sound* pSound) {
    pSound->pMainSound = Mix_LoadMUS("../assets/mainSound.mp3");
    if (!handleError(pGame, pSound->pMainSound, Mix_GetError)) {
        return 0;
    }
    Mix_VolumeMusic(75);
    pSound->pJumpSound = Mix_LoadWAV("../assets/jumpSound.mp3");
    if (!handleError(pGame, pSound->pJumpSound, Mix_GetError)) {
        return 0;
    }
    Mix_VolumeChunk(pSound->pJumpSound, 15);
    pSound->pWinSound = Mix_LoadWAV("../assets/winSound.wav");
    if (!handleError(pGame, pSound->pWinSound, Mix_GetError)) {
        return 0;
    }

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

void initiateButtons(Buttons* pButtons, GameDisplay* pGameDisplay) {
    pButtons->pStartButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pSettingsButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pQuitButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pCreateLobbyButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 0, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pJoinLobbyButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pLanguageButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 - 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pMoveRightButton = createButton(pGameDisplay->windowWidth / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pMoveLeftButton = createButton(pGameDisplay->windowWidth / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pMuteButton = createButton(pGameDisplay->windowWidth / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 150, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pReturnButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 200, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pEnglishButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 - 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pSwedishButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pResumeButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 50, BUTTON_WIDTH, BUTTON_HEIGHT);
    pButtons->pMainMenuButton = createButton((pGameDisplay->windowWidth - BUTTON_WIDTH) / 2, (pGameDisplay->windowHeight - BUTTON_HEIGHT) / 2 + 100, BUTTON_WIDTH, BUTTON_HEIGHT);
}

void initiateLanguage(FILE* fp, Language* pLanguage, GameDisplay* pGameDisplay, Buttons* pButtons, DisplayText* pDisplayText) {
    #if __APPLE__
		readFromFileLangMAC(pLanguage->chosenLanguage, pLanguage->language);
    #else
        readFromFileLang(fp, pLanguage->language);
	#endif

    readKeybindString(pLanguage, 0, pGameDisplay, pButtons);

    pButtons->pStartButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[0], 0, 0, 0, 0, 0);
    pButtons->pSettingsButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[1], 0, 0, 0, 0, 50);
    pButtons->pQuitButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[2], 0, 0, 0, 0, 100);
    pButtons->pResumeButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[3], 0, 0, 0, 0, 50);
    pButtons->pMainMenuButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[4], 0, 0, 0, 0, 100);
    pButtons->pLanguageButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[5], 0, 0, 0, 0, -100);
    pButtons->pReturnButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[6], 0, 0, 0, 0, 200);
    pButtons->pMoveRightButton1Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[7], 0, 0, 0, -80, 50);
    pButtons->pMoveLeftButton1Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[8], 0, 0, 0, -80, 100);
    pDisplayText->pYouAreDeadText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[9], 0, 0, 0, 0, -200);
    pButtons->pEnglishButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, "English", 0, 0, 0, 0, -50);
    pButtons->pSwedishButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, "Svenska", 0, 0, 0, 0, 0);
    pButtons->pCreateLobbyButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[10], 0, 0, 0, 0, 0);
    pButtons->pJoinLobbyButtonText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[11], 0, 0, 0, 0, 50);
    pButtons->pMuteButton1Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[13], 0, 0, 0, -80, 150);
    pDisplayText->pWaitingText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[16], 0, 0, 0, 0, 50);
    pDisplayText->pEnterIPText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[17], 0, 0, 0, -130, 50);
}