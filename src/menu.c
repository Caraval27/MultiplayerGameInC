#include "../include/main.h"

void renderMenu(GameDisplay* pGameDisplay, SDL_Texture* pTexture) {
    SDL_Rect rect = {0, 0, pGameDisplay->windowWidth, pGameDisplay->windowHeight};

    SDL_RenderCopy(pGameDisplay->pRenderer, pTexture, NULL, &rect);
}

Button* createButton(float xPos, float yPos, float width, float height) {
    Button* pButton = malloc(sizeof(Button));

    pButton->xPos = xPos;
    pButton->yPos = yPos;
    pButton->width = width;
    pButton->height = height;

    return pButton;
}

void getMousePos(Button* pButton) {
    int mouseXPos, mouseYPos;

    pButton->mouseState = SDL_GetMouseState(&mouseXPos, &mouseYPos);
    pButton->mouseXDelta = fabsf(mouseXPos - (pButton->xPos + pButton->width / 2));
    pButton->mouseYDelta = fabsf(mouseYPos - (pButton->yPos + pButton->height / 2));
}

void handleButton(Button* pButton, bool* pPressed) {
    getMousePos(pButton);

    if (pButton->mouseXDelta <= BUTTON_WIDTH / 2 && pButton->mouseYDelta <= BUTTON_HEIGHT / 2 && pButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)) {
        *pPressed = true;
    }
}

void renderButton(Button* pButton, SDL_Renderer* pRenderer, SDL_Texture* pTexture) {
    SDL_Rect rect = {pButton->xPos, pButton->yPos, pButton->width, pButton->height};

    SDL_RenderCopy(pRenderer, pTexture, NULL, &rect);
}

void destroyButton(Button* pButton) {
    if (pButton) {
        free(pButton);
    }
}