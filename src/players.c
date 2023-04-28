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


void initPlayer(Player** pPlayers, int *pNrOfPlayersLeft, int pNrOfPlayers, int windowWidth, int windowHeight, SDL_Texture** pPlayerTextures, SDL_Window* pWindow, SDL_Renderer* pRenderer){
    float startPosition = 1;
    *pNrOfPlayersLeft = MAX_PLAYERS;
    for(int i = 0; i < pNrOfPlayers; i++) {
        pPlayers[i] = createPlayer(startPosition * windowWidth / 7, windowHeight, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED); //ändra starterpositions
        pPlayerTextures[i] = createPicture(pWindow, pRenderer, CHARACTER_PICTURE); //gör en sträng av detta ist
        startPosition += 1;
    }
}

void handlePlayers(Player** pPlayers, int pNrOfPlayers, int *nrOfPlayersLeft, bool *pLeft, bool *pRight, int windowWidth, int windowHeight, Platform* pStartPlatform, Mix_Chunk *pJumpSound, State* pState, SDL_Renderer* pRenderer, SDL_Texture** pPlayerTextures, SDL_RendererFlip flip, Platform** pPlatforms, Text* pGameOverText){

    for (int i = 0; i < pNrOfPlayers; i++) //av någon anledning dyker inte player 2 upp, förmodligen pga samma bild och position, samt båda rör sig med tangenttrycken
    {
        if (i == 0) { //bara för att prova om spelare 2 dyker upp i loopen
            movePlayer(pPlayers[i], *pLeft, *pRight, windowWidth);
            jumpPlayer(pPlayers[i], pStartPlatform->yPos, pJumpSound);
            playerCollidePlatform(pPlayers[i], pPlatforms, pJumpSound);
            checkIfPlayerDead(pPlayers[i], windowHeight, pState, nrOfPlayersLeft);
            renderPlayer(pPlayers[i], pRenderer, pPlayerTextures[i], flip);
            if (!pPlayers[i]->alive) {
                renderText(pGameOverText);
            }
        }
        else {
            jumpPlayer(pPlayers[i], pStartPlatform->yPos, pJumpSound);
            playerCollidePlatform(pPlayers[i], pPlatforms, pJumpSound);
            checkIfPlayerDead(pPlayers[i], windowHeight, pState, nrOfPlayersLeft);
            renderPlayer(pPlayers[i], pRenderer, pPlayerTextures[i], SDL_FLIP_NONE);
        }
    }
    handleWin(*nrOfPlayersLeft, pState);
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

void jumpPlayer(Player* pPlayer, int startPlatformHeight, Mix_Chunk* pJumpSound){
    if (pPlayer->alive) {
        pPlayer->yVelocity += (GRAVITY / 60);
        pPlayer->yPos += (pPlayer->yVelocity / 60);

        if (pPlayer->yPos <= 0) {
            pPlayer->yPos = 0;
            pPlayer->yVelocity = -(pPlayer->yVelocity) / 4;
        }
        else if (pPlayer->yPos >= startPlatformHeight - pPlayer->height) {
            pPlayer->yPos = startPlatformHeight - pPlayer->height;
            pPlayer->yVelocity = JUMP_SPEED;
            Mix_VolumeChunk(pJumpSound, 10);
            Mix_PlayChannel(-1, pJumpSound, 0);
        }
    }
}

void playerCollidePlatform(Player* pPlayer, Platform** pPlatforms, Mix_Chunk* pJumpSound){
    if (pPlayer->alive) {
        int i;

        for (i = 0; pPlatforms[i] != 0; i++) {
            if (pPlayer->yVelocity > 0 &&
            pPlayer->xPos + pPlayer->width >= pPlatforms[i]->xPos &&
            pPlayer->xPos <= pPlatforms[i]->xPos + pPlatforms[i]->width &&
            pPlayer->yPos + pPlayer->height >= pPlatforms[i]->yPos &&
            pPlayer->yPos + pPlayer->height < pPlatforms[i]->yPos + pPlayer->yVelocity / 60) {
                pPlayer->yVelocity = JUMP_SPEED;
                Mix_PlayChannel(-1, pJumpSound, 0);
            }
        }
    }
}

/*void playerCollidePlayer(Player* pPlayer1, Player* pPlayer2)
{

}*/

void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_RendererFlip flip){
    if (pPlayer->alive) {
        SDL_Rect rect = {pPlayer->xPos, pPlayer->yPos, pPlayer->width, pPlayer->height};

        SDL_RenderCopyEx(pRenderer, pTexture, NULL, &rect, 0.0, NULL, flip);
    }
}


void destroyPlayers(Player** pPlayers) {
    for (int i = 0; i != MAX_PLAYERS; i++) {
        if (pPlayers[i]) {
            free(pPlayers[i]);
        }
    }
}

void destroyPlayerTextures(SDL_Texture** pPlayerTextures) {
    for (int i = 0; i != MAX_PLAYERS; i++) {
        if (pPlayerTextures[i]) {
            SDL_DestroyTexture(pPlayerTextures[i]);
        }
    }
}

int playerIsDead(Player* pPlayer, int windowHeight){
    if(pPlayer->alive && pPlayer->yPos + pPlayer->height >= windowHeight + pPlayer->yVelocity / 60) {
        return 1;
    }
    return 0;
}

void checkIfPlayerDead(Player* pPlayer, int windowHeight, State* pState, int* pNrOfPlayersLeft){
    if(playerIsDead(pPlayer, windowHeight)) {
        pPlayer->alive = false;
        (*pNrOfPlayersLeft)--;
    }
}

void handleWin(int nrOfPlayersLeft, State* pState){
    if (nrOfPlayersLeft <= 1) {
        *pState = GAME_OVER;
    }
}
