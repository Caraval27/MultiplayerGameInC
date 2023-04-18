#include "../include/main.h"

Button* createButton(SDL_Rect buttonRect, int mouseX, int mouseY){
    Button* pButton = malloc(sizeof(Button));

    pButton->deltaX = mouseX - (buttonRect.x + buttonRect.w/2);
    pButton->deltaY = mouseY - (buttonRect.y + buttonRect.h/2);
    pButton->buttonDistance = sqrt(pButton->deltaX*pButton->deltaX+pButton->deltaY*pButton->deltaY);
    
    return pButton;
}

void handleButtonInput(Button* pButton, SDL_Rect* pButtonRect, bool* pIsRunning, int windowWidth, int windowHeight){
    int mouseX, mouseY, mousePos;

    mousePos = SDL_GetMouseState(&mouseX, &mouseY);
    pButtonRect->x = (windowWidth - BUTTON_WIDTH)/2;
    pButtonRect->y = (windowHeight - BUTTON_HEIGHT)/2;
    pButtonRect->w = BUTTON_WIDTH;
    pButtonRect->h = BUTTON_HEIGHT;
    pButton = createButton(*pButtonRect, mouseX, mouseY);
    if (pButton->buttonDistance < BUTTON_HEIGHT && mousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
        *pIsRunning = false;
    }
}

void renderButton(SDL_Renderer* pRenderer, SDL_Rect buttonRect, int r, int g, int b){
    SDL_SetRenderDrawColor(pRenderer, r, g, b, 0);
    SDL_RenderFillRect(pRenderer, &buttonRect);
}

SDL_Texture* initMenuBackground(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMenuBackgroundRect, int windowWidth, int windowHeight){
    SDL_Surface* pSurface = IMG_Load("../assets/menuBackground.jpeg");
    if(!pSurface){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if(!pTexture){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }

    //SDL_QueryTexture(pTexture, NULL, NULL, &pMenuBackgroundRect->w, &pMenuBackgroundRect->h);
    pMenuBackgroundRect->w = windowWidth; // size
    pMenuBackgroundRect->h = windowHeight;
    pMenuBackgroundRect->x = 0; //left
    pMenuBackgroundRect->y = 0; //right

    return pTexture;
}

void renderMenuBackground(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect menuBackgroundRect){
    SDL_RenderCopy(pRenderer, pTexture, NULL, &menuBackgroundRect);
}