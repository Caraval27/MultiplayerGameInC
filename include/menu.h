#pragma once

#include "../include/text.h"

#define MAIN_MENU_PICTURE "../assets/mainMenuBackground.png"
#define BUTTON_PICTURE "../assets/button.png"
#define BUTTON_WIDTH 220
#define BUTTON_HEIGHT 40

typedef struct {
	float xPos, yPos;
	float width, height;
	int mouseYDelta, mouseXDelta;
    float mouseState;
} Button;

typedef struct {
    Button *pStartButton,  *pSettingsButton, *pQuitButton, *pLanguageButton, *pMoveLeftButton, *pMoveRightButton, *pReturnButton, *pEnglishButton, *pSwedishButton, *pResumeButton,
    *pMainMenuButton, *pCreateLobbyButton, *pJoinLobbyButton, *pMuteButton;
    Text *pStartButtonText, *pSettingsButtonText, *pQuitButtonText, *pLanguageButtonText,  *pMoveLeftButton1Text, *pMoveRightButton1Text, *pReturnButtonText, *pEnglishButtonText,
    *pSwedishButtonText, *pResumeButtonText, *pMainMenuButtonText, *pMoveLeftButton2Text, *pMoveRightButton2Text, *pCreateLobbyButtonText, *pJoinLobbyButtonText,
    *pMuteButton1Text, *pMuteButton2Text;
    SDL_Texture *pButtonTexture, *pButtonExitTexture;
} Buttons;

void renderMenu(GameDisplay* pGameDisplay, SDL_Texture* pTexture);
Button* createButton(float xPos, float yPos, float width, float height);
void getMousePos(Button* pButton);
void handleButton(Button* pButton, bool* pPressed);
void renderButton(Button* pButton, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void destroyButton(Button* pButton);