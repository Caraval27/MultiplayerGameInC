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

/*void handlePlayer (Player* pPlayer, SDL_Renderer* pRenderer, SDL_Window* pWindow, char characterPicture[], bool left, bool right, int windowWidth, float jumpHeight,int height, Mix_Chunk* pJumpSound, Platform** platforms, int windowHeight){

    createPlayerCharacter(&pRenderer, &pWindow, characterPicture);
    movePlayer(&pPlayer, left, right, windowWidth);
    jumpPlayer(&pPlayer, jumpHeight, height, &pJumpSound);
    playerCollidePlatforms(&pPlayer, &platforms, &jumpHeight, windowHeight, &pJumpSound);
    renderPlayer(&pPlayer, &pRenderer, &pTexture);
}*/

SDL_Texture* createPlayerCharacter(SDL_Renderer* pRenderer, SDL_Window* pWindow, char characterPicture[]){
    SDL_Surface* pSurface = IMG_Load(characterPicture);
    if (!pSurface) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer); 
        SDL_DestroyWindow(pWindow); 
        SDL_Quit(); 
        exit (1);
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if (!pTexture) {
        printf("Error: %s\n",SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        exit (1);    
    }
    SDL_FreeSurface(pSurface);

    return pTexture;
}

void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth){
    if (left && !right) {
        pPlayer->xPos -= (pPlayer->xVelocity) / 20;
    }
    else if (right && !left) {
        pPlayer->xPos += (pPlayer->xVelocity) / 20;
    }

    if (pPlayer->xPos < 0) {
        pPlayer->xPos = 0;
    }
    else if (pPlayer->xPos > windowWidth - pPlayer->width) {
        pPlayer->xPos = windowWidth - pPlayer->width;
    }
}

void jumpPlayer(Player* pPlayer, float jumpHeight, int height, Mix_Chunk* pJumpSound){
    pPlayer->yPos += pPlayer->yVelocity / 60;

    if (pPlayer->yPos <= 0) {
        pPlayer->yPos = 0;
        pPlayer->yVelocity = -(pPlayer->yVelocity);
    }
    else if (pPlayer->yPos >= height - pPlayer->height) { 
        pPlayer->yPos = height - pPlayer->height;
        pPlayer->yVelocity = -(pPlayer->yVelocity);
        Mix_PlayChannel(-1, pJumpSound, 0);
    }
    if (pPlayer->yPos <= jumpHeight - pPlayer->height) {
        pPlayer->yPos = jumpHeight - pPlayer->height;
        pPlayer->yVelocity = -(pPlayer->yVelocity);
    }
}

void playerCollidePlatform(Player* pPlayer, Platform** platforms, float* pJumpHeight, int windowHeight, Mix_Chunk* pJumpSound){
    int i; 

    for (i = 0; platforms[i] != 0; i++) {
        if (pPlayer->yVelocity > 0 &&
        pPlayer->xPos + pPlayer->width >= platforms[i]->xPos &&
        pPlayer->xPos <= platforms[i]->xPos + platforms[i]->width &&
        pPlayer->yPos + pPlayer->height >= platforms[i]->yPos &&
        pPlayer->yPos + pPlayer->height < platforms[i]->yPos + pPlayer->yVelocity / 20) {
            pPlayer->yVelocity = -(pPlayer->yVelocity);
            *pJumpHeight = platforms[i]->yPos - JUMP_HEIGHT;
            Mix_PlayChannel(-1, pJumpSound, 0);
            /*if (*pJumpHeight < 1) {
                *pJumpHeight = 10 + pPlayer->height;
            }*/
        }
    }
} 

/*void playerCollidePlayer(Player* pPlayer1, Player* pPlayer2)
{

}*/

void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture){
    SDL_Rect rect = {pPlayer->xPos, pPlayer->yPos, pPlayer->width, pPlayer->height};

    SDL_RenderCopy(pRenderer, pTexture, NULL, &rect);
}

void destroyPlayer(Player* pPlayer){
    free(pPlayer);
}