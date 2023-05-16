#include "../include/main.h"

int initiateSDLLibraries(void) {
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
    return 1;
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