#include "../include/main.h"

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

    renderButton(pButton, pRenderer);
}

void renderButton(Button* pButton, SDL_Renderer* pRenderer){
    SDL_Rect rect = {pButton->xPos, pButton->yPos, pButton->width, pButton->height};

    SDL_SetRenderDrawColor(pRenderer, 138, 43, 226, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(pRenderer, &rect);
}

void destroyButton(Button* pButton){
    free(pButton);
}

SDL_Texture* createMainMenuImage(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMainMenuRect, int windowWidth, int windowHeight){
    SDL_Surface* pSurface = IMG_Load("../assets/menuBackground.jpeg");
    if (!pSurface) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if (!pTexture) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_FreeSurface(pSurface);

    pMainMenuRect->w = windowWidth; // size
    pMainMenuRect->h = windowHeight;
    pMainMenuRect->x = 0; //left
    pMainMenuRect->y = 0; //right
    //SDL_QueryTexture(pTexture, NULL, NULL, &pMenuBackgroundRect->w, &pMenuBackgroundRect->h);

    return pTexture;
}

void renderMainMenu(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect mainMenuRect){
    SDL_RenderCopy(pRenderer, pTexture, NULL, &mainMenuRect);
}