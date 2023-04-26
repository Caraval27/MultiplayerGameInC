#include "../include/main.h"

void renderMenu(SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight){
    SDL_Rect rect = {0, 0, windowWidth, windowHeight};

    SDL_RenderCopy(pRenderer, pTexture, NULL, &rect);
}

Button* createButton(float xPos, float yPos, float width, float height){
    Button* pButton = malloc(sizeof(Button));

    pButton->xPos = xPos;
    pButton->yPos = yPos;
    pButton->width = width;
    pButton->height = height;

    return pButton;
}

void getMousePos(Button* pButton){
    int mouseXPos, mouseYPos;
    int mouseXDelta, mouseYDelta;

    pButton->mouseState = SDL_GetMouseState(&mouseXPos, &mouseYPos);
    mouseXDelta = mouseXPos - (pButton->xPos + pButton->width / 2);
    mouseYDelta = mouseYPos - (pButton->yPos + pButton->height / 2);
    pButton->mouseDistance = sqrt(mouseXDelta * mouseXDelta + mouseYDelta * mouseYDelta);
}

void handleButton(Button* pButton, SDL_Renderer* pRenderer, State* pState, State desiredState){
    getMousePos(pButton);

    if (pButton->mouseDistance < BUTTON_HEIGHT && pButton->mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)) {
        *pState = desiredState;
    }
}

void renderButton(Button* pButton, SDL_Renderer* pRenderer){
    SDL_Rect rect = {pButton->xPos, pButton->yPos, pButton->width, pButton->height};

    SDL_SetRenderDrawColor(pRenderer, 138, 43, 226, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(pRenderer, &rect);
}

void destroyButton(Button* pButton){
    free(pButton);
}

int playerIsDead(Player* pPlayer, int windowHeight){
    if(pPlayer->yPos >= windowHeight) {
        return 1;
    }  
    return 0;
}

void checkIfPlayerDead(Player* pPlayer, int windowHeight, State* gameState){
    if(playerIsDead(pPlayer, windowHeight)) {
        *gameState = MAIN_MENU;
    }
}
