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
    float mouseState, mouseDistance;
} Button;

Button* createButton(float xPos, float yPos, float width, float height);
void getMousePos(Button* pButton);
void handleButton(Button* pButton, SDL_Renderer* pRenderer, State* pState, State desiredState);
void renderButton(Button * pButton, SDL_Renderer* pRenderer);
void destroyButton(Button* pButton);
SDL_Texture* createMainMenuImage(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMenuBackgroundRect, int windowWidth, int windowHeight);
void renderMainMenu(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect menuBackgroundRect);
