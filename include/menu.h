#ifndef MENU_H
#define MENU_H

#define MENU_IMAGE_WIDTH 2200
#define MENU_IMAGE_HEIGHT 1524

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 30

typedef struct {
    float buttonDistance;
    int deltaX, deltaY;
} Button;

Button* createButton(SDL_Rect buttonRect, int mouseX, int mouseY);
void handleButtonInput(Button* pButton, SDL_Rect* pButtonRect, bool* pIsRunning, int windowWidth, int windowHeight);
void renderButton(SDL_Renderer* pRenderer, SDL_Rect buttonRect, int r, int g, int b);
SDL_Texture* initMenuBackground(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMenuBackgroundRect, int windowWidth, int windowHeight);
void renderMenuBackground(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect menuBackgroundRect);

#endif