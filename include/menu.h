#pragma once

#define MENU_WIDTH 2200
#define MENU_HEIGHT 1524
#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 40

typedef enum {
    MAIN_MENU,
	SETTINGS_MENU,
	ENTER_INPUT,
	ONGOING,
	GAME_MENU,
	GAME_OVER,
	QUIT
} State;

typedef struct {
	float xPos, yPos;
	float width, height;
	int mouseYDelta, mouseXDelta;
    float mouseState;
} Button;

SDL_Texture* createMainMenuImage(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMenuBackgroundRect, int windowWidth, int windowHeight);
void renderMenu(SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
Button* createButton(float xPos, float yPos, float width, float height);
void getMousePos(Button* pButton);
void handleButton(Button* pButton, bool* pPressed);
void renderButton(Button* pButton, SDL_Renderer* pRenderer);
void destroyButton(Button* pButton);
int playerIsDead(Player* pPlayer, int windowHeight);
void checkIfPlayerDead(Player* pPlayer, int windowHeight, State* pState, int* nrOfPlayersLeft);
