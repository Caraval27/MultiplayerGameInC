#ifndef MENU_H
#define MENU_H

#define MENU_IMAGE_WIDTH 2200
#define MENU_IMAGE_HEIGHT 1524
#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 40

typedef enum {
    MAIN_MENU,
	SETTINGS_MENU,
	ONGOING,
	GAME_MENU,
	GAME_OVER,
	QUIT
} State;

typedef struct {
    float buttonDistance;
    int deltaX, deltaY;
} Button;

void getMousePos(SDL_Rect* pButtonRect, int* pMousePos, int windowWidth, int windowHeight, int addY, Button* pButton);
void handleButtonInput(Button* pQuitButton, bool* pIsRunning, int mousePos, SDL_Event event, State* pState, State desiredState);
void renderButton(SDL_Renderer* pRenderer, SDL_Rect buttonRect, int r, int g, int b);
SDL_Texture* createMainMenuImage(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMenuBackgroundRect, int windowWidth, int windowHeight);
void renderMainMenu(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect menuBackgroundRect);
Button* createButton(SDL_Rect* pButtonRect, int windowHeight, int windowWidth, int yOffset);

#endif