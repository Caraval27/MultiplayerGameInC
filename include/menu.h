#pragma once

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

void renderMenu(SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
Button* createButton(float xPos, float yPos, float width, float height);
void getMousePos(Button* pButton);
void handleButton(Button* pButton, bool* pPressed);
void renderButton(Button* pButton, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void destroyButton(Button* pButton);