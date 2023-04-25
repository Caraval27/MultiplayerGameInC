#include "../include/main.h"
#define LENGTH 100
#define MAXPLAYERS 6

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
    char characterPicture1[LENGTH] = "../assets/penguin.png";
    char characterPicture2[LENGTH] = "../assets/musse.png";
    char backgroundPicture[LENGTH] = "../assets/background.png";
    char startingPlatformPicture[LENGTH] = "../assets/iceBlock.png";
    //char plattformsPicture[LENGTH] = ;
    int nrOfPlayers = MAXPLAYERS;

    pGame->pWindow = SDL_CreateWindow("Totally not a doodle jump clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pGame->windowWidth, pGame->windowHeight, 0);
    if (!handleError(pGame, pGame->pWindow, SDL_GetError)) return 0;

	Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	#if __APPLE__ || __LINUX__
		flags = (flags & !SDL_RENDERER_ACCELERATED) | SDL_RENDERER_SOFTWARE;
	#endif

    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, flags);
    if (!handleError(pGame, pGame->pRenderer, SDL_GetError)) return 0;

    pGame->pMainMenuTexture = createMainMenuImage(pGame->pWindow, pGame->pRenderer, &pGame->mainMenuRect, pGame->windowWidth, pGame->windowHeight);
    if (!handleError(pGame, pGame->pMainMenuTexture, SDL_GetError)) return 0;

    pGame->pBackgroundTexture = createBackgroundImage(pGame->pWindow, pGame->pRenderer, backgroundPicture);
    if (!handleError(pGame, pGame->pBackgroundTexture, SDL_GetError)) return 0;

    pGame->pStartPlatformTexture = createBackgroundImage(pGame->pWindow, pGame->pRenderer, startingPlatformPicture);
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
    pGame->pStartButton = createButton(&pGame->startButtonRect, pGame->windowHeight, pGame->windowWidth, 0, 0);
    pGame->pSettingsButton = createButton(&pGame->settingsButtonRect, pGame->windowHeight, pGame->windowWidth, 50, 0);
    pGame->pQuitButton = createButton(&pGame->quitButtonRect, pGame->windowHeight, pGame->windowWidth, 100, 0);
    pGame->pResumeButton = createButton(&pGame->resumeButtonRect, pGame->windowHeight, pGame->windowWidth, 50, 0);
    pGame->pMainMenuButton = createButton(&pGame->mainMenuButtonRect, pGame->windowHeight, pGame->windowWidth, 100, 0);
    pGame->pLanguageButton = createButton(&pGame->languageButtonRect, pGame->windowHeight, pGame->windowWidth, -100, 0);
    pGame->pEnglishButton = createButton(&pGame->englishButtonRect, pGame->windowHeight, pGame->windowWidth, -50, 0);
    pGame->pSwedishButton = createButton(&pGame->swedishButtonRect, pGame->windowHeight, pGame->windowWidth, 0, 0);
    pGame->pReturnButton = createButton(&pGame->returnButtonRect, pGame->windowHeight, pGame->windowWidth, 200, 0);
    pGame->pMoveRight1Button = createButton(&pGame->moveRight1ButtonRect, pGame->windowHeight, pGame->windowWidth, 50, -80);
    pGame->pMoveLeft1Button = createButton(&pGame->moveLeft1ButtonRect, pGame->windowHeight, pGame->windowWidth, 100, -80);
    pGame->pMoveRight2Button = createButton(&pGame->moveRight2ButtonRect, pGame->windowHeight, pGame->windowWidth, 50, 80);
    pGame->pMoveLeft2Button = createButton(&pGame->moveLeft2ButtonRect, pGame->windowHeight, pGame->windowWidth, 100, 80);


    pGame->pGameOverText = createText(pGame->pRenderer, pGame->pMainMenuFont, 255, 255, 255, "Game Over", pGame->windowWidth, pGame->windowHeight, 0, 0);

    pGame->pStartingPlatform = createPlatform(0, pGame->windowHeight - 100, pGame->windowWidth, 100);

    
    for(int i=0; i<nrOfPlayers; i++){ //segmentation fault om man loopar mindre än 6 gånger??
        int startPosition = 5;
        pGame->pPlayers[i] = createPlayer(pGame->windowWidth / startPosition, pGame->windowHeight, 60, 60, SPEED, 400);
        pGame->pPlayerTexture[i] = createPlayerCharacter(pGame->pRenderer, pGame->pWindow, characterPicture1); //gör en sträng av detta ist
        startPosition += 100; //så att spelarna får olika startpositioner
    }

    // KRASCHAR Pï¿½ MAC initiateLanguage(fp, pGame);

    pGame->state = MAIN_MENU;

    return 1;
}

void runGame(Game* pGame){
    bool isRunning = true, left = false, right = false;
    SDL_Event event;
    int mousePos, num, sec;
    float jumpHeight = pGame->windowHeight - JUMP_HEIGHT;

    Mix_VolumeMusic(32);
    Mix_PlayMusic(pGame->pMainSound, -1);
    while (isRunning){
        switch (pGame->state) {
            case MAIN_MENU: handleMainMenu(pGame, event, &mousePos);
            break;
            case SETTINGS_MENU: handleSettingsMenu(pGame, event, &mousePos, &num);
            break;
            case ENTER_INPUT: handleEnterInput(pGame, event, &num);
            break;
            case ONGOING: handleOngoing(pGame, event, &isRunning, &right, &left, &jumpHeight, &sec);
            break;
            case GAME_MENU: handleGameMenu(pGame, &mousePos, event);
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
        destroyChuck(pGame->pJumpSound);
    }
    if (pGame->pMainSound){
        destroyMusic(pGame->pMainSound);
    }
    destroyPlatform(pGame->platforms);
    if (pGame->pPlayers[0]){
        destroyPlayer(pGame->pPlayers[0]);
    }
    if (pGame->pPlayers[1]){
        destroyPlayer(pGame->pPlayers[1]);
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
    if (pGame->pPlayer1Texture){
        SDL_DestroyTexture(pGame->pPlayer1Texture);
    }
    if (pGame->pPlayer2Texture){
        SDL_DestroyTexture(pGame->pPlayer2Texture);
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

void handleMainMenu(Game* pGame, SDL_Event event, int* pMousePos){
    Mix_ResumeMusic();
    while (SDL_PollEvent(&event)){
        *pMousePos = getMousePos(pGame->startButtonRect, *pMousePos, pGame->pStartButton);
        handleButtonInput(pGame->pStartButton, *pMousePos, event, &pGame->state, ONGOING);
        *pMousePos = getMousePos(pGame->settingsButtonRect, *pMousePos, pGame->pSettingsButton);
        handleButtonInput(pGame->pSettingsButton, *pMousePos, event, &pGame->state, SETTINGS_MENU);
        *pMousePos = getMousePos(pGame->quitButtonRect, *pMousePos, pGame->pQuitButton);
        handleButtonInput(pGame->pQuitButton, *pMousePos, event, &pGame->state, QUIT);

        renderMainMenu(pGame->pRenderer, pGame->pMainMenuTexture, pGame->mainMenuRect);
        renderButton(pGame->pRenderer, pGame->startButtonRect, 250, 43, 226);
        renderButton(pGame->pRenderer, pGame->settingsButtonRect, 170, 43, 226);
        renderButton(pGame->pRenderer, pGame->quitButtonRect, 138, 43, 226);
        // KRASHCAR MAC renderText(pGame->pStartButtonText);
        // KRASCHAR MAC renderText(pGame->pSettingsButtonText);
        // KRASCHAR MAC renderText(pGame->pQuitButtonText);
    }
}

void handleSettingsMenu(Game* pGame, SDL_Event event, int* pMousePos, int* pNum){
    char chosenLang[LANG_LENGTH];
    bool showLang = false;
    while (SDL_PollEvent(&event)){
        *pMousePos = getMousePos(pGame->languageButtonRect, *pMousePos, pGame->pLanguageButton);
        if (pGame->pLanguageButton->buttonDistance < BUTTON_HEIGHT && *pMousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
            showLang = !showLang;
        }
        *pMousePos = getMousePos(pGame->moveRight2ButtonRect, *pMousePos, pGame->pMoveRight2Button);
        if (pGame->pMoveRight2Button->buttonDistance < BUTTON_HEIGHT && *pMousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
            *pNum = 0;
            pGame->state = ENTER_INPUT;
        }
        *pMousePos = getMousePos(pGame->moveLeft2ButtonRect, *pMousePos, pGame->pMoveLeft2Button);
        if (pGame->pMoveLeft2Button->buttonDistance < BUTTON_HEIGHT && *pMousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
            *pNum = 1;
            pGame->state = ENTER_INPUT;
        }

        *pMousePos = getMousePos(pGame->returnButtonRect, *pMousePos, pGame->pReturnButton);
        if (pGame->pReturnButton->buttonDistance < BUTTON_HEIGHT && *pMousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
            pGame->state = MAIN_MENU;
            showLang = false;
        }

        renderMainMenu(pGame->pRenderer, pGame->pMainMenuTexture, pGame->mainMenuRect);
        if (showLang){
            *pMousePos = getMousePos(pGame->englishButtonRect, *pMousePos, pGame->pEnglishButton);
            if (pGame->pEnglishButton->buttonDistance < BUTTON_HEIGHT && *pMousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
                showLang = false;
                strcpy(chosenLang, "english.txt");
            }
            *pMousePos = getMousePos(pGame->swedishButtonRect, *pMousePos, pGame->pSwedishButton);
            if (pGame->pSwedishButton->buttonDistance < BUTTON_HEIGHT && *pMousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
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

            renderButton(pGame->pRenderer, pGame->englishButtonRect, 200, 43, 226);
            renderButton(pGame->pRenderer, pGame->swedishButtonRect, 200, 43, 226);
            renderText(pGame->pEnglishButtonText);
            renderText(pGame->pSwedishButtonText);
        }

        renderSettingsMenu(pGame);
    }
}

void renderSettingsMenu(Game *pGame){
    renderButton(pGame->pRenderer, pGame->languageButtonRect, 250, 43, 226);
    renderText(pGame->pLanguageButtonText);
    renderButton(pGame->pRenderer, pGame->returnButtonRect, 250, 43, 226);
    renderText(pGame->pReturnButtonText);
    renderButton(pGame->pRenderer, pGame->moveRight1ButtonRect, 250, 43, 226);
    renderText(pGame->pMoveRight1ButtonText);
    renderButton(pGame->pRenderer, pGame->moveLeft1ButtonRect, 250, 43, 226);
    renderText(pGame->pMoveLeft1ButtonText);
    renderButton(pGame->pRenderer, pGame->moveRight2ButtonRect, 250, 43, 226);
    //renderText(pGame->pMoveRight2ButtonText);
    renderButton(pGame->pRenderer, pGame->moveLeft2ButtonRect, 250, 43, 226);
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

void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pRight, bool* pLeft, float* pJumpHeight, int* pSec){
    Mix_ResumeMusic();
    while (SDL_PollEvent(&event)){
        handleInputOngoing(&pGame->state, &event, pIsRunning, pRight, pLeft, pGame->keybinds);
    }

    handleBackground(pGame->pBackground, pGame->pRenderer, pGame->pBackgroundTexture, pGame->windowWidth, pGame->windowHeight); //denna måste ligga före allt med player

    for (int i=0; i != 1; i++) //av någon anledning dyker inte player 2 upp, förmodligen pga samma bild och position, samt båda rör sig med tangenttrycken
    {
        if (i==0)
        {
            movePlayer(pGame->pPlayers[i], *pLeft, *pRight, pGame->windowWidth);
            jumpPlayer(pGame->pPlayers[i], *pJumpHeight, pGame->pStartingPlatform->yPos, pGame->pJumpSound);
            playerCollidePlatform(pGame->pPlayers[i], pGame->platforms, pJumpHeight, pGame->windowHeight, pGame->pJumpSound);
            renderPlayer(pGame->pPlayers[i], pGame->pRenderer, pGame->pPlayerTexture[i]);
        }
        else
        {
            jumpPlayer(pGame->pPlayers[i], *pJumpHeight, pGame->pStartingPlatform->yPos, pGame->pJumpSound);
            playerCollidePlatform(pGame->pPlayers[i], pGame->platforms, pJumpHeight, pGame->windowHeight, pGame->pJumpSound);
            renderPlayer(pGame->pPlayers[i], pGame->pRenderer, pGame->pPlayerTexture[i]);
        }

    }

        jumpPlayer(pGame->pPlayers[1], *pJumpHeight, pGame->pStartingPlatform->yPos, pGame->pJumpSound);
        playerCollidePlatform(pGame->pPlayers[1], pGame->platforms, pJumpHeight, pGame->windowHeight, pGame->pJumpSound);
        renderPlayer(pGame->pPlayers[1], pGame->pRenderer, pGame->pPlayerTexture[1]);

    /*movePlayer(pGame->pPlayers[0], *pLeft, *pRight, pGame->windowWidth);
    jumpPlayer(pGame->pPlayers[0], *pJumpHeight, pGame->pStartingPlatform->yPos, pGame->pJumpSound);
    playerCollidePlatform(pGame->pPlayers[0], pGame->platforms, pJumpHeight, pGame->windowHeight, pGame->pJumpSound);
    renderPlayer(pGame->pPlayers[0], pGame->pRenderer, pGame->pPlayerTexture[0]); //player 1

    jumpPlayer(pGame->pPlayers[1], *pJumpHeight, pGame->pStartingPlatform->yPos, pGame->pJumpSound);
    playerCollidePlatform(pGame->pPlayers[1], pGame->platforms, pJumpHeight, pGame->windowHeight, pGame->pJumpSound);
    renderPlayer(pGame->pPlayers[1], pGame->pRenderer, pGame->pPlayerTexture[1]); //player 2*/

    handlePlatform(pGame->platforms, pGame->pRenderer, pGame->windowWidth);
    handleStartingPlatform(pGame->pStartingPlatform, pGame->pRenderer, pGame->pStartPlatformTexture, pGame->windowHeight, pSec);
    //checkIfPlayerDead(pGame->players[0], pGame->windowHeight, &pGame->state);  
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

void handleGameMenu(Game* pGame, int* pMousePos, SDL_Event event){
    Mix_PauseMusic();
    *pMousePos = getMousePos(pGame->resumeButtonRect, *pMousePos, pGame->pResumeButton);
    handleButtonInput(pGame->pResumeButton, *pMousePos, event, &pGame->state, ONGOING);
    *pMousePos = getMousePos(pGame->mainMenuButtonRect, *pMousePos, pGame->pMainMenuButton);
    handleButtonInput(pGame->pMainMenuButton, *pMousePos, event, &pGame->state, MAIN_MENU);
    renderButton(pGame->pRenderer, pGame->mainMenuButtonRect, 138, 43, 226);
    renderButton(pGame->pRenderer, pGame->resumeButtonRect, 250, 43, 226);
    // Gï¿½R Sï¿½ ATT MAN INTE KAN KOMMA TILL RESUMEMENU renderText(pGame->pMainMenuButtonText);
    // Gï¿½R Sï¿½ ATT MAN INTE KAN KOMMA TILL RESUMEMENU renderText(pGame->pResumeButtonText);
}