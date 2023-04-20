#include "../include/main.h"

Button* createButton(SDL_Rect* pButtonRect, int windowHeight, int windowWidth, int yOffset, int xOffset){
    Button* pButton = malloc(sizeof(Button));
    
    pButtonRect->x = (windowWidth - BUTTON_WIDTH)/2 + xOffset;
    pButtonRect->y = (windowHeight - BUTTON_HEIGHT)/2 + yOffset;
    pButtonRect->w = BUTTON_WIDTH;
    pButtonRect->h = BUTTON_HEIGHT;

    return pButton;
}

int getMousePos(SDL_Rect buttonRect, int mousePos, Button* pButton){
    int mouseX, mouseY;    

    mousePos = SDL_GetMouseState(&mouseX, &mouseY);
    pButton->deltaX = mouseX - (buttonRect.x + buttonRect.w/2);
    pButton->deltaY = mouseY - (buttonRect.y + buttonRect.h/2);
    pButton->buttonDistance = sqrt(pButton->deltaX * pButton->deltaX + pButton->deltaY * pButton->deltaY);

    return mousePos;
}

void handleButtonInput(Button* pButton, int mousePos, SDL_Event event, State* pState, State desiredState){
    if (pButton->buttonDistance < BUTTON_HEIGHT && mousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
        *pState = desiredState;
    }
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT) *pState = QUIT;
}

void renderButton(SDL_Renderer* pRenderer, SDL_Rect buttonRect, int r, int g, int b){
    SDL_SetRenderDrawColor(pRenderer, r, g, b, 0);
    SDL_RenderFillRect(pRenderer, &buttonRect);
}

SDL_Texture* createMainMenuImage(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMainMenuRect, int windowWidth, int windowHeight){
    SDL_Surface* pSurface = IMG_Load("../assets/menuBackground.jpeg");
    if(!pSurface){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit(1);
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if(!pTexture){
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