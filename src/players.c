#include "../include/main.h"

#define SPEED 100

struct player{
    float posX, posY;
    float velocityY, velocityX;
};

Player* createPlayer(int x, int y){
    Player* pPlayer = malloc(sizeof(Player));

    pPlayer->posX = x;
    pPlayer->posY = y;
    pPlayer->velocityX = 0;
    pPlayer->velocityY = 5;

    return pPlayer;
}

SDL_Texture* createPlayerCharacter(SDL_Rect* pPlayerRect, SDL_Renderer* pPlayerRenderer, SDL_Window* pWindow, int windowWidth, int windowHeight){
    
    SDL_Surface* pPlayerSurface = IMG_Load("../assets/penguin.png"); //Ändra så att man kan skicka in en textsträng sen
    if (!pPlayerSurface){
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pPlayerRenderer); // destroys the render
        SDL_DestroyWindow(pWindow); //destroys the window that has been made
        SDL_Quit(); //quit SDL because it has been initialized
        exit (1);
    }

    SDL_Texture* pPlayerTexture = SDL_CreateTextureFromSurface(pPlayerRenderer, pPlayerSurface);
    SDL_FreeSurface(pPlayerSurface);
    if(!pPlayerTexture){
        printf("Error: %s\n",SDL_GetError());
        SDL_DestroyRenderer(pPlayerRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit (1);    
    }

    pPlayerRect->w = 50;
    pPlayerRect->h = 50;
    pPlayerRect->x = windowWidth - pPlayerRect->w;
    pPlayerRect->y = windowHeight - pPlayerRect->h;

    //SDL_QueryTexture(pTexture, NULL, NULL, &pPlayerRect->w, &pPlayerRect->h); behövs inte?
    return pPlayerTexture;
}

/*void renderPlayer(SDL_Renderer *pRenderer, Player* pPlayer) {
	SDL_Rect playerRect;
	playerRect.x = pPlayer->posX;
	playerRect.y = pPlayer->posY;
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(pRenderer, &playerRect);
}*/

void jumpPlayer(Player* pPlayer, SDL_Rect playerRect, int windowHeight, float platformY, float maxJumpHeight){
    pPlayer->posY += pPlayer->velocityY;

    if (pPlayer->posY + playerRect.h >= windowHeight - platformY){
        pPlayer->posY = windowHeight - playerRect.h - platformY;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
    if (playerRect.y <= 0){
        pPlayer->posY = 0;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
    if (pPlayer->posY + playerRect.h < maxJumpHeight){
        pPlayer->posY = maxJumpHeight - playerRect.h;
        pPlayer->velocityY = -(pPlayer->velocityY);
    }
}

void movePlayer(Player* pPlayer, SDL_Rect playerRect, bool left, bool right, int windowWidth){
    if (pPlayer->velocityX > SPEED) pPlayer->velocityX = 0;
    if (left && !right){
        pPlayer->velocityX += SPEED;
        pPlayer->posX -= (pPlayer->velocityX) / 60;
        
    }
    else if(right && !left){
        pPlayer->velocityX += SPEED;
        pPlayer->posX += (pPlayer->velocityX) / 60;
    }

    if(pPlayer->posX<0) pPlayer->posX = 0;
    if (pPlayer->posX > windowWidth-playerRect.w) pPlayer->posX = windowWidth - playerRect.w;
}

void platformCollidePlayer(Player* pPlayer, SDL_Rect playerRect, Plank** platforms, float* pPlatformY, float* pMaxJumpHeight){
    for (int i = 0; platforms[i] != 0; i++){
        if (pPlayer->posY + playerRect.h >= platforms[i]->yPos &&
            pPlayer->posY < platforms[i]->yPos + platforms[i]->height &&
            pPlayer->posX + playerRect.w >= platforms[i]->xPos &&
            pPlayer->posX < platforms[i]->xPos + platforms[i]->width &&
            pPlayer->velocityY >= 0){
            pPlayer->posY = platforms[i]->yPos - playerRect.h; // vet ej om behövs
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

void updatePlayer(Player* pPlayer, SDL_Rect* pPlayerRect){
    pPlayerRect->x = pPlayer->posX;
    pPlayerRect->y = pPlayer->posY;
}

void destroyPlayer(Player* pPlayer){
    free(pPlayer);
}