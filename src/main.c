#include "../include/main.h"
#include "../include/initiate.h"
#include "../include/run.h"
#include "../include/quit.h"

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

    if (!initializeNetcode(pGame->pNetworkData)) {
		printf("netcode initialization failed\n");
		return 0;
	}

	printf("gameplaydata size: %d\n", sizeof(GameplayData));
	if (sizeof(GameplayData) > PACKET_SIZE) {
		printf("gameplaydata size exceeds packet size\n");
		return 0;
	}

    if (!initiateDisplay(pGame, &pGame->gameDisplay)) return 0;
    if (!initiateTexture(pGame, &pGame->gameDisplay, &pGame->buttons)) return 0;
    if (!initiateSound(pGame, &pGame->sound)) return 0;
    initiateButtons(&pGame->buttons, &pGame->gameDisplay);

    pGame->pBackground = createBackground(pGame->gameDisplay.windowHeight);
    pGame->pStartPlatform = createPlatform(0, pGame->gameDisplay.windowHeight - 100, pGame->gameDisplay.windowWidth, 100);

    FILE *fp = NULL;
    readFromFileKey(fp, pGame->language.keybinds);
    initiateLanguage(fp, &pGame->language, &pGame->gameDisplay, &pGame->buttons, &pGame->displayText);

    pGame->state = MAIN_MENU;

    return 1;
}

void runGame(Game* pGame) {
    SDL_Event event;
    bool isRunning = true, mute = false;
    int time = 0;

    Mix_PlayMusic(pGame->sound.pMainSound, -1);

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
            case LOBBY: handleLobby(&pGame->gameDisplay, pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands, &pGame->buttons, &pGame->displayText, event, &pGame->state, pGame->pLobbyConnect, pGame->pPlatforms);
                break;
            case ONGOING: handleOngoing(&pGame->gameDisplay, pGame->pPlayersData, pGame->pNetworkData, pGame->pGameplayData, pGame->pClientCommands, &pGame->displayText, &pGame->language, &pGame->sound, event, &pGame->state, pGame->pBackground, pGame->pPlatforms, pGame->pStartPlatform, &isRunning, &time, &mute);
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

    quitSound(&pGame->sound);
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