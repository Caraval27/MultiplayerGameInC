#include "../include/main.h"

Player* createPlayer(float xPos, float yPos, float width, float height, float xVelocity, float yVelocity){
    Player* pPlayer = malloc(sizeof(Player));

    pPlayer->xPos = xPos;
    pPlayer->yPos = yPos;
    pPlayer->width = width;
    pPlayer->height = height;
    pPlayer->xVelocity = xVelocity;
    pPlayer->yVelocity = yVelocity;
    pPlayer->alive = true;

    return pPlayer;
}

void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth){
    if (pPlayer->alive) {
        if (left && !right) {
            pPlayer->xPos -= (pPlayer->xVelocity) / 60;
        }
        else if (right && !left) {
            pPlayer->xPos += (pPlayer->xVelocity) / 60;
        }

        if (pPlayer->xPos < 0) {
            pPlayer->xPos = 0;
        }
        else if (pPlayer->xPos > windowWidth - pPlayer->width) {
            pPlayer->xPos = windowWidth - pPlayer->width;
        }
    }
}

void jumpPlayer(Player* pPlayer, float jumpHeight, int startPlatformHeight, Mix_Chunk* pJumpSound){
    if (pPlayer->alive) {
        pPlayer->yVelocity += GRAVITY / 60;
        pPlayer->yPos += pPlayer->yVelocity / 60;

        if (pPlayer->yPos <= 0) {
            pPlayer->yPos = 0;
            pPlayer->yVelocity = MOVE_SPEED;
        }
        else if (pPlayer->yPos >= startPlatformHeight - pPlayer->height) {
            pPlayer->yPos = startPlatformHeight - pPlayer->height;
            pPlayer->yVelocity = -(MOVE_SPEED);
            Mix_VolumeChunk(pJumpSound, 3);
            Mix_PlayChannel(-1, pJumpSound, 0);
        }
        else if (pPlayer->yPos <= jumpHeight - pPlayer->height) {
            pPlayer->yPos = jumpHeight - pPlayer->height;
            pPlayer->yVelocity = MOVE_SPEED;
        }
    }
}

void playerCollidePlatform(Player* pPlayer, Platform** pPlatforms, float* pJumpHeight, int windowHeight, Mix_Chunk* pJumpSound){
    if (pPlayer->alive) {
        int i;

        for (i = 0; pPlatforms[i] != 0; i++) {
            if (pPlayer->yVelocity > 0 &&
            pPlayer->xPos + pPlayer->width >= pPlatforms[i]->xPos &&
            pPlayer->xPos <= pPlatforms[i]->xPos + pPlatforms[i]->width &&
            pPlayer->yPos + pPlayer->height >= pPlatforms[i]->yPos &&
            pPlayer->yPos + pPlayer->height < pPlatforms[i]->yPos + pPlayer->yVelocity / 60) {
                pPlayer->yVelocity = -(MOVE_SPEED);
                *pJumpHeight = pPlatforms[i]->yPos - JUMP_HEIGHT;
                Mix_VolumeChunk(pJumpSound, 3);
                Mix_PlayChannel(-1, pJumpSound, 0);
                /*if (*pJumpHeight < 1) {
                    *pJumpHeight = 10 + pPlayer->height;
                }*/
            }
        }
    }
}

/*void playerCollidePlayer(Player* pPlayer1, Player* pPlayer2)
{

}*/

void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture){
    if (pPlayer->alive) {
        SDL_Rect rect = {pPlayer->xPos, pPlayer->yPos, pPlayer->width, pPlayer->height};

        SDL_RenderCopy(pRenderer, pTexture, NULL, &rect);
    }
}

void destroyPlayers(Player** pPlayers) {
    for (int i = 0; pPlayers[i] != 0; i++) {
        if (pPlayers[i]) {
            free(pPlayers[i]);
        }
    }
}

void destroyPlayerTextures(SDL_Texture** pPlayerTextures) {
    for (int i = 0; pPlayerTextures[i] != 0; i++) {
        if (pPlayerTextures[i]) {
            SDL_DestroyTexture(pPlayerTextures[i]);
        }
    }
}
