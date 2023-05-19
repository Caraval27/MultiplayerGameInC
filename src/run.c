#include "../include/main.h"
#include "../include/run.h"
#include "../include/initiate.h"

void handleMainMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute) {
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

void renderMainMenu(GameDisplay* pGameDisplay, Buttons* pButtons) {
    renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);

    renderButton(pButtons->pStartButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pSettingsButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pQuitButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pStartButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pSettingsButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pQuitButtonText, pGameDisplay->pRenderer);
}

void handleSettingsMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState) {
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

void renderSettingsMenu(GameDisplay* pGameDisplay, Buttons* pButtons) {
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

void handleLanguageMenu(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event) {
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

void renderLanguageMenu(GameDisplay* pGameDisplay, Buttons* pButtons) {
    renderButton(pButtons->pEnglishButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pSwedishButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pEnglishButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pSwedishButtonText, pGameDisplay->pRenderer);
}

void readKeybindString(Language* pLanguage, int index, GameDisplay* pGameDisplay, Buttons* pButtons) {
    SDL_Keycode moveRight = pLanguage->keybinds[0], moveLeft = pLanguage->keybinds[1], mute = pLanguage->keybinds[2];
    char keybindsText[3][30];

    if (index == 0) {
        strcpy(keybindsText[0], (SDL_GetKeyName(moveRight)));
        strcpy(keybindsText[1], (SDL_GetKeyName(moveLeft)));
        strcpy(keybindsText[2], (SDL_GetKeyName(mute)));

        pButtons->pMoveRightButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, keybindsText[0], 0, 0, 0, BUTTON_WIDTH / 2, 50);
        pButtons->pMoveLeftButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, keybindsText[1], 0, 0, 0, BUTTON_WIDTH / 2, 100);
        pButtons->pMuteButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, keybindsText[2], 0, 0, 0, BUTTON_WIDTH / 2, 150);
    }
    else if (index == 1) {
        pButtons->pMoveRightButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[12], 0, 0, 0, BUTTON_WIDTH / 2, 50);
    }
    else if (index == 2) {
        pButtons->pMoveLeftButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[12], 0, 0, 0, BUTTON_WIDTH / 2, 100);
    }
    else if (index == 3) {
        pButtons->pMuteButton2Text = createText(pGameDisplay, pGameDisplay->pMenuFont, pLanguage->language[12], 0, 0, 0, BUTTON_WIDTH / 2, 150);
    }
}

void handleEnterInput(GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButtons, SDL_Event event, State* pState) {
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

void handleLobbyMenu(GameDisplay* pGameDisplay, PlayersData* pPlayersData, NetworkData* pNetworkData, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect, Platform** pPlatforms, Platform* pStartPlatform, int* pTime) {
    bool buttonPressed = false;

    renderLobbyMenu(pGameDisplay, pButtons);

    if (pLobbyConnect->joined) {
        renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);
        renderText(pDisplayText->pEnterIPText, pGameDisplay->pRenderer);
        renderButton(pButtons->pReturnButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
        renderText(pButtons->pReturnButtonText, pGameDisplay->pRenderer);
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
            handleButton(pButtons->pReturnButton, &buttonPressed);
            if (buttonPressed) {
                buttonPressed = false;
                pLobbyConnect->joined = false;
                //fillZero(pGame, INPUT_IP_LEN);
                //(*pIndex) = 0;
            }
            if (event.type == SDL_QUIT) {
                *pState = QUIT;
            }
        }
        if ((pLobbyConnect->inputIPIndex) > 0) {
            pDisplayText->pInputIPText = createText(pGameDisplay, pGameDisplay->pMenuFont, pLobbyConnect->inputIP, 0, 0, 0, 70, 50);
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
        resetGame(pPlayersData, pGameDisplay, pPlatforms, pStartPlatform, &pTime);
    }
}

void renderLobbyMenu(GameDisplay* pGameDisplay, Buttons* pButtons) {
    renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);

    renderButton(pButtons->pCreateLobbyButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pJoinLobbyButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pReturnButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pCreateLobbyButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pJoinLobbyButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pReturnButtonText, pGameDisplay->pRenderer);
}

void handleLobby(GameDisplay* pGameDisplay, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, Buttons* pButtons, DisplayText* pDisplayText, SDL_Event event, State* pState, LobbyConnect* pLobbyConnect, Platform** pPlatforms) {
    bool buttonPressed = false;
    bool isHost = pNetworkData->isHost;
    char nrOfClients[2];

    renderMenu(pGameDisplay, pGameDisplay->pMenuTexture);

    runNetcode(pNetworkData, pGameplayData, pClientCommands);

    if (isHost) { // SERVER
        renderButton(pButtons->pStartButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
        renderText(pButtons->pStartButtonText, pGameDisplay->pRenderer);
        renderButton(pButtons->pReturnButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
        renderText(pButtons->pReturnButtonText, pGameDisplay->pRenderer);

        sprintf(nrOfClients, "%d", pNetworkData->nClients);
        pDisplayText->pNrClientsText = createText(pGameDisplay, pGameDisplay->pMenuFont, nrOfClients, 0, 0, 0, 0, 70);
        renderText(pDisplayText->pNrClientsText, pGameDisplay->pRenderer);
        while (SDL_PollEvent(&event)) {
            handleButton(pButtons->pStartButton, &buttonPressed);
            if (buttonPressed) {
                *pState = ONGOING;
                buttonPressed = false;
                pGameplayData->gameState = ONGOING;
                initPlatforms(pPlatforms, pGameDisplay);
            }
            handleButton(pButtons->pReturnButton, &buttonPressed);
            if (buttonPressed) {
                *pState = LOBBY_MENU;
                buttonPressed = false;
            }
            if (event.type == SDL_QUIT) {
                *pState = QUIT;
            }
        }

		for (int i = 0; i < 6; i++) {
			// evaluate command ...
			pClientCommands[i] = (ClientCommand){0};
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
            for(int i = 0; i < NR_OF_PLATFORMS; i++){
                pPlatforms[i] = createPlatform(0, 0, PLATFORM_WIDTH, PLATFORM_HEIGHT);
            }
            *pState = ONGOING;
        }
    }
}

void fillZero(char inputIP[], int max) {
    for (int i = 0; i < max; i++)
    {
        inputIP[i] = '\0';
    }
}

void handleOngoing(GameDisplay* pGameDisplay, PlayersData* pPlayersData, NetworkData* pNetworkData, GameplayData* pGameplayData, ClientCommand* pClientCommands, DisplayText* pDisplayText, Language* pLanguage, Music* pMusic, SDL_Event event, State* pState, Background* pBackground, Platform** pPlatforms, Platform* pStartPlatform, bool* pIsRunning, int *pTime, bool* pMute) {
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

        for(int i = 0; i < NR_OF_PLATFORMS; i++){
            temp.platformXPos[i] = (short)pPlatforms[i]->xPos;
            temp.platformYPos[i] = (short)pPlatforms[i]->yPos;
        }
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

        for(int i = 0; i < NR_OF_PLATFORMS; i++){
            pPlatforms[i]->xPos = pGameplayData->platformXPos[i];
            pPlatforms[i]->yPos = pGameplayData->platformYPos[i];
        }
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

    handleBackground(pBackground, pGameDisplay, pGameDisplay->pBackgroundTexture); //denna mï¿½ste ligga fï¿½re allt med player

	// KEEP THIS COMMENTED FOR NOW
    handlePlatforms(pPlatforms, pGameDisplay->pRenderer, pGameDisplay->pPlatformTexture, pGameDisplay->windowWidth, pGameDisplay->windowHeight, isHost);
    //pGame->pPlatforms[0] = createPlatform(100, 100, PLATFORM_WIDTH, PLATFORM_HEIGHT);
    //renderPlatform(pGame->pPlatforms[0], pGame->gameDisplax   y.pRenderer, pGame->gameDisplay.pPlatformTexture);

    handleStartPlatform(pStartPlatform, pPlatforms[0], pGameDisplay->pRenderer, pGameDisplay->pStartPlatformTexture, pGameDisplay->windowHeight, pTime);
    handlePlayers(pPlayersData->pPlayers, pPlayersData->nrOfPlayers, &pPlayersData->nrOfPlayersLeft, pMute, pGameDisplay->windowWidth, pGameDisplay->windowHeight, pStartPlatform, pMusic->pJumpSound, pMusic->pWinSound, pState, pGameDisplay->pRenderer, pPlayersData->pPlayerTextures, pPlatforms, pDisplayText->pYouAreDeadText, &pNetworkData->isHost);

    SDL_Delay(3);
}

void handleOngoingInput(PlayersData* pPlayersData, NetworkData* pNetworkData, ClientCommand* pClientCommands, Language* pLanguage, SDL_Event* event, State* pState, bool* pIsRunning, bool* pMute) {
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
            break;
    }
}

void handleGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons, SDL_Event event, State* pState, bool* pMute) {
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

void renderGameMenu(GameDisplay* pGameDisplay, Buttons* pButtons) {
    renderButton(pButtons->pResumeButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);
    renderButton(pButtons->pMainMenuButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pMainMenuButtonText, pGameDisplay->pRenderer);
    renderText(pButtons->pResumeButtonText, pGameDisplay->pRenderer);
}

void handleGameOver(PlayersData* pPlayersData, GameDisplay* pGameDisplay, Language* pLanguage, Buttons* pButton, DisplayText* pDisplayText, SDL_Event event, State* pState) {
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
        pDisplayText->pWhoWonText = createText(pGameDisplay, pGameDisplay->pMenuFont, whoWonString, 0, 0, 0, 0, -300);
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

void renderGameOver(GameDisplay* pGameDisplay, Buttons* pButtons, Text* pWhoWonText) {
    renderButton(pButtons->pMainMenuButton, pGameDisplay->pRenderer, pButtons->pButtonTexture);

    renderText(pButtons->pMainMenuButtonText, pGameDisplay->pRenderer);
    renderText(pWhoWonText, pGameDisplay->pRenderer);
}

void resetGame(PlayersData* pPlayersData, GameDisplay* pGameDisplay, Platform** pPlatforms, Platform* pStartPlatform, int *pTime) {
    char avatar[6][25] = {"../assets/player1.png", "../assets/player2.png", "../assets/player3.png", "../assets/player4.png", "../assets/player5.png", "../assets/player6.png"};
    int subtractXPos = -100;
    int increaseXPos = 0;

    resetPlatforms(pPlatforms);
    resetStartPlatform(pStartPlatform, pGameDisplay->windowHeight, pTime);
    resetPlayers(pPlayersData->pPlayers, &pPlayersData->nrOfPlayers, &pPlayersData->nrOfPlayersLeft);

	for (int i = 0; i < MAX_PLAYERS; i++) {
		initPlayer(pPlayersData->pPlayers, &pPlayersData->nrOfPlayers, &pPlayersData->nrOfPlayersLeft, pGameDisplay, pStartPlatform->yPos, pPlayersData->pPlayerTextures, &subtractXPos, &increaseXPos, avatar[i]);
	}
}
