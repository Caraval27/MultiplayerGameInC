#pragma once

#include "../include/libraries.h"
#include "../include/files.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"
#include "../include/text.h"
#include "../include/network.h"

#define BACKGROUND_PICTURE "../assets/background.png"
#define BACKGROUND_WIDTH 1024
#define BACKGROUND_HEIGHT 1536
#define BACKGROUND_SPEED 1

typedef struct{
    float upperSrcYPos, upperSrcHeight;
    float lowerSrcYPos, lowerSrcHeight;
    float upperDstYPos, upperDstHeight;
    float lowerDstYPos, lowerDstHeight;
} Background;

typedef struct {
    State state;
    SDL_Window *pWindow;
    int windowWidth, windowHeight;
    SDL_Renderer *pRenderer;
    SDL_Texture *pMenuTexture, *pBackgroundTexture, *pButtonTexture, *pButtonExitTexture, *pPlatformTexture, *pStartPlatformTexture;
    TTF_Font *pMenuFont;
    Background* pBackground;
    Button *pStartButton,  *pSettingsButton, *pQuitButton, *pLanguageButton, *pMoveLeftButton, *pMoveRightButton, *pReturnButton, *pEnglishButton, *pSwedishButton, *pResumeButton,
    *pMainMenuButton, *pCreateLobbyButton, *pJoinLobbyButton, *pMuteButton;

    bool chosenLanguage;

    Text *pStartButtonText, *pSettingsButtonText, *pQuitButtonText, *pLanguageButtonText,  *pMoveLeftButton1Text, *pMoveRightButton1Text, *pReturnButtonText, *pEnglishButtonText,
    *pSwedishButtonText, *pResumeButtonText, *pMainMenuButtonText, *pMoveLeftButton2Text, *pMoveRightButton2Text,*pYouAreDeadText, *pCreateLobbyButtonText, *pJoinLobbyButtonText,
    *pMuteButton1Text, *pMuteButton2Text, *pWhoWonText, *pWaitingText, *pInputIPText, *pEnterIPText, *pNrClientsText;
    Player* pPlayers[MAX_PLAYERS];
    SDL_Texture* pPlayerTextures[MAX_PLAYERS];
    int nrOfPlayers, nrOfPlayersLeft;
    SDL_RendererFlip flip;
    Platform *pPlatforms[100], *pStartPlatform;

    char inputIP[INPUT_IP_LEN];
    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
    Mix_Chunk *pJumpSound, *pWinSound;
    Mix_Music *pMainSound;

	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand pClientCommands[COMMAND_BUFFER];
} Game;

Background* createBackground(int windowHeight);
SDL_Texture* createPicture(SDL_Window* pWindow, SDL_Renderer* pRenderer, char picture[]);
void destroyTexture(SDL_Texture* pTexture);
void handleBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth);
void destroyBackground(Background* pBackground);
void destroyMusic(Mix_Music* pMainSound);
void destroyChunk(Mix_Chunk* pMusicChunk);