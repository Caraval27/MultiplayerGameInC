#include "../include/main.h"

Button* createButton(SDL_Rect* pButtonRect, int windowHeight, int windowWidth, int yOffset){
    Button* pButton = malloc(sizeof(Button));
    
    pButtonRect->x = (windowWidth - BUTTON_WIDTH)/2;
    pButtonRect->y = (windowHeight - BUTTON_HEIGHT)/2 + yOffset;
    pButtonRect->w = BUTTON_WIDTH;
    pButtonRect->h = BUTTON_HEIGHT;

    return pButton;
    
}

void getMousePos(SDL_Rect* pButtonRect, int* pMousePos, int windowWidth, int windowHeight, int addY, Button* pButton){
    int mouseX, mouseY; 

    *pMousePos = SDL_GetMouseState(&mouseX, &mouseY);

    pButton->deltaX = mouseX - (pButtonRect->x + pButtonRect->w/2);
    pButton->deltaY = mouseY - (pButtonRect->y + pButtonRect->h/2);
    pButton->buttonDistance = sqrt(pButton->deltaX * pButton->deltaX + pButton->deltaY * pButton->deltaY);
}

void handleButtonInput(Button* pQuitButton, bool* pIsRunning, int mousePos, SDL_Event event, State* pState, State desiredState){
    if (pQuitButton->buttonDistance < BUTTON_HEIGHT && mousePos && SDL_BUTTON(SDL_BUTTON_LEFT)){
        *pState = desiredState;
    }
    if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) *pIsRunning = false;
    if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE) *pState = ONGOING;
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