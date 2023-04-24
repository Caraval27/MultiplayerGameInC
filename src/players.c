#include "../include/main.h"

Player* createPlayer(int xPos, int yPos, int width, int height, int xVelocity, int yVelocity){
    Player* pPlayer = malloc(sizeof(Player));

    pPlayer->xPos = xPos;
    pPlayer->yPos = yPos;
    pPlayer->width = width;
    pPlayer->height = height;
    pPlayer->xVelocity = xVelocity;
    pPlayer->yVelocity = yVelocity;

    return pPlayer;
}

SDL_Texture* createPlayerCharacter(SDL_Renderer* pPlayerRenderer, SDL_Window* pWindow, char characterPicture[]){
    SDL_Surface* pPlayerSurface = IMG_Load(characterPicture);
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

void jumpPlayer(Player* pPlayer, int windowHeight, float platformYPos, float jumpHeight){
    pPlayer->yPos += pPlayer->yVelocity / 60;

    if (pPlayer->yPos + pPlayer->height >= windowHeight - platformYPos){ //<=platformY
        pPlayer->yPos = windowHeight - pPlayer->height - platformYPos;
        pPlayer->yVelocity = -(pPlayer->yVelocity);
    }
    if (pPlayer->yPos <= 0){
        pPlayer->yPos = 0;
        pPlayer->yVelocity = -(pPlayer->yVelocity);
    }
    if (pPlayer->yPos + pPlayer->height < jumpHeight){
        pPlayer->yPos = jumpHeight - pPlayer->height;
        pPlayer->yVelocity = -(pPlayer->yVelocity);
    }
}

void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth){
    if (left && !right){
        pPlayer->xPos -= (pPlayer->xVelocity) / 60;
    }
    else if (right && !left){
        pPlayer->xPos += (pPlayer->xVelocity) / 60;
    }

    if (pPlayer->xPos < 0) pPlayer->xPos = 0;
    if (pPlayer->xPos > windowWidth - pPlayer->width) pPlayer->xPos = windowWidth - pPlayer->width;
}

void playerCollidePlatform(Player* pPlayer, Platform** platforms, float* pPlatformYPos, float* pJumpHeight){
    int i; 

    for (i = 0; platforms[i] != 0; i++){
        if (pPlayer->yPos + pPlayer->height >= platforms[i]->yPos &&
            pPlayer->yPos < platforms[i]->yPos + platforms[i]->height &&
            pPlayer->xPos + pPlayer->width >= platforms[i]->xPos &&
            pPlayer->xPos < platforms[i]->xPos + platforms[i]->width &&
            pPlayer->yVelocity >= 0){
            pPlayer->yPos = platforms[i]->yPos - pPlayer->height; // vet ej om behövs
            *pPlatformYPos = platforms[i]->yPos + platforms[i]->height;
            *pJumpHeight = *pPlatformYPos - (JUMP_HEIGHT - 100);
            if (*pJumpHeight < 1) *pJumpHeight = 10 + pPlayer->height;
            pPlayer->yVelocity = -(pPlayer->yVelocity);
        }
        else {
            *pPlatformYPos = 0;
        }
    }
} 

void playerCollidePlayer(Player* pPlayer1, Player* pPlayer2, SDL_Rect player1Rect, SDL_Rect player2Rect)
{

}

void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture){
    SDL_Rect player = {pPlayer->xPos, pPlayer->yPos, pPlayer->width, pPlayer->height};

    SDL_RenderCopy(pRenderer, pTexture, NULL, &player);
}

void destroyPlayer(Player* pPlayer){
    free(pPlayer);
}