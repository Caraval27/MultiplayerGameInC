#include "../include/main.h"

struct player{
    float xPos, yPos;
    float velocityY, velocityX;
};

Player* createPlayer(int x, int y, SDL_Rect* playerRect, int windowWidth, int windowHeight){
    Player* pPlayer = malloc(sizeof(Player));

    pPlayer->xPos = x;
    pPlayer->yPos = y;
    pPlayer->velocityX = 0;
    pPlayer->velocityY = 5;

    playerRect->w = 50;
    playerRect->h = 50;
    playerRect->x = windowWidth - playerRect->w;
    playerRect->y = windowHeight - playerRect->h;

    return pPlayer;
}

SDL_Texture* createPlayerCharacter(SDL_Renderer* pPlayerRenderer, SDL_Window* pWindow){
    SDL_Surface* pPlayerSurface = IMG_Load("../assets/penguin.png"); //Ändra så att man kan skicka in en textsträng sen
    if (!pPlayerSurface){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pPlayerRenderer); 
        SDL_DestroyWindow(pWindow); 
        SDL_Quit(); 
        exit (1);
    }
    SDL_Texture* pPlayerTexture = SDL_CreateTextureFromSurface(pPlayerRenderer, pPlayerSurface);
    if(!pPlayerTexture){
        printf("Error: %s\n",SDL_GetError());
        SDL_DestroyRenderer(pPlayerRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit (1);    
    }
    SDL_FreeSurface(pPlayerSurface);

    return pPlayerTexture;
}

void jumpPlayer(Player* pPlayer, SDL_Rect playerRect, int windowHeight, float platformY, float maxJumpHeight){
    pPlayer->yPos += pPlayer->velocityY;

    if (pPlayer->yPos + playerRect.h >= windowHeight - platformY){
        pPlayer->yPos = windowHeight - playerRect.h - platformY;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
    if (playerRect.y <= 0){
        pPlayer->yPos = 0;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
    if (pPlayer->yPos + playerRect.h < maxJumpHeight){
        pPlayer->yPos = maxJumpHeight - playerRect.h;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
}

void movePlayer(Player* pPlayer, SDL_Rect playerRect, bool left, bool right, int windowWidth){
    //if (pPlayer->velocityX > SPEED) pPlayer->velocityX = 0;
    pPlayer->velocityX = SPEED;
    if (left && !right){
        //pPlayer->velocityX += SPEED;
        pPlayer->xPos -= (pPlayer->velocityX) / 60;
        
    }
    else if (right && !left){
        //pPlayer->velocityX += SPEED;
        pPlayer->xPos += (pPlayer->velocityX) / 60;
    }

    if (pPlayer->xPos < 0) pPlayer->xPos = 0;
    if (pPlayer->xPos > windowWidth - playerRect.w) pPlayer->xPos = windowWidth - playerRect.w;
}

void platformCollidePlayer(Player* pPlayer, SDL_Rect playerRect, Platform** platforms, float* pPlatformY, float* pMaxJumpHeight){
    for (int i = 0; platforms[i] != 0; i++){
        if (pPlayer->yPos + playerRect.h >= platforms[i]->yPos &&
            pPlayer->yPos < platforms[i]->yPos + platforms[i]->height &&
            pPlayer->xPos + playerRect.w >= platforms[i]->xPos &&
            pPlayer->xPos < platforms[i]->xPos + platforms[i]->width &&
            pPlayer->velocityY >= 0){
            pPlayer->yPos = platforms[i]->yPos - playerRect.h; // vet ej om behövs
            *pPlatformY = platforms[i]->yPos + platforms[i]->height;
            *pMaxJumpHeight = *pPlatformY - (MAX_JUMP_HEIGHT - 100);
            if (*pMaxJumpHeight < 1) *pMaxJumpHeight = 10 + playerRect.h;
            pPlayer->velocityY = -(pPlayer->velocityY);
        }
        else {
            *pPlatformY = 0;
            //*pMaxJumpHeight = MAX_JUMP_HEIGHT;
        }
    }
} 

void renderPlayer(SDL_Renderer* pRenderer, SDL_Texture* pTexture, Player* pPlayer, SDL_Rect* pPlayerRect){
    pPlayerRect->x = pPlayer->xPos;
    pPlayerRect->y = pPlayer->yPos;
    SDL_RenderCopy(pRenderer, pTexture, NULL, pPlayerRect);
}

void destroyPlayer(Player* pPlayer){
    free(pPlayer);
}