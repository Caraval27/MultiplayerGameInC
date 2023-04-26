#pragma once

#include "../include/main.h"
#include "../include/libraries.h"
#include "../include/files.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"
#include "../include/text.h"
#include "../include/network.h"

#define BACKGROUND_WIDTH 1024
#define BACKGROUND_HEIGHT 1536
#define BACKGROUND_SPEED 1
#define MAX_PLAYERS 10
#define BACKGROUND_PICTURE "../assets/background.png"
#define PLATFORM_PICTURE "../assets/iceBlock.png"
#define START_PLATFORM_PICTURE "../assets/iceBlock.png"
#define CHARACTER_PICTURE "../assets/musse.png"
#define MAIN_MENU_PICTURE "../assets/mainMenuBackground.png"
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
    SDL_RendererFlip *flip;
    SDL_Texture *pBackgroundTexture, *pMenuTexture, *pStartPlatformTexture, *pPlatformTexture;
    TTF_Font *pMenuFont;
    Background* pBackground;
    Button *pStartButton, *pQuitButton, *pResumeButton, *pMainMenuButton, *pSettingsButton, *pLanguageButton, *pEnglishButton, *pSwedishButton, *pReturnButton,
    *pMoveLeft1Button, *pMoveRight1Button, *pMoveLeft2Button, *pMoveRight2Button;

    Text *pStartButtonText, *pQuitButtonText, *pResumeButtonText, *pMainMenuButtonText, *pSettingsButtonText, *pLanguageButtonText, *pEnglishButtonText,
    *pSwedishButtonText, *pReturnButtonText, *pMoveLeft1ButtonText, *pMoveRight1ButtonText, *pMoveLeft2ButtonText, *pMoveRight2ButtonText, *pGameOverText;

    Player* pPlayers[MAX_PLAYERS];
    SDL_Texture* pPlayerTextures[MAX_PLAYERS];
    int pNrOfPlayers, nrOfPlayersLeft;

    Platform *pPlatforms[1000], *pStartPlatform;

    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
    Mix_Chunk *pJumpSound;
    Mix_Music *pMainSound;

	NetworkData *pNetworkData;
	GameplayData *pGameplayData;
	ClientCommand *pClientCommand;
} Game;

Background* createBackground(int windowHeight);
SDL_Texture* createPicture(SDL_Window* pWindow, SDL_Renderer* pRenderer, char picture[]);
void handleBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth);
void destroyBackground(Background* pBackground);
void destroyMusic(Mix_Music* pMainSound);
void destroyChunk(Mix_Chunk* pMusicChunk);
void handleStartPlatform(Platform* pStartPlatform, Platform* pFirstPlatform, Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowHeight, int* pTime);
void resetStartPlatform(Platform* pStartPlatform, int windowHeight, int* pTime);