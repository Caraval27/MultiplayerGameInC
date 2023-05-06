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
	pPlayer->ip = (IPaddress){0};

    return pPlayer;
}


/*void initPlayers(Player** pPlayers, int* pNrOfPlayers, int* pNrOfPlayersLeft, int windowWidth, float startPlatformYPos, SDL_Texture** pPlayerTextures, SDL_Window* pWindow, SDL_Renderer* pRenderer){
    int startPosition = 1, i;

    *pNrOfPlayers = MAX_PLAYERS;
    *pNrOfPlayersLeft = MAX_PLAYERS;

    for(i = 0; i < *pNrOfPlayers; i++) {
        pPlayers[i] = createPlayer(startPosition * windowWidth / (*pNrOfPlayers + 1), startPlatformYPos - CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED); //?ndra starterpositions
        pPlayerTextures[i] = createPicture(pWindow, pRenderer, CHARACTER_PICTURE); //g?r en str?ng av detta ist
        startPosition += 1;
    }
}*/


void initPlayers(Player** pPlayers, int* pNrOfPlayers, int* pNrOfPlayersLeft, int windowWidth, float startPlatformYPos, SDL_Texture** pPlayerTextures, SDL_Window* pWindow, SDL_Renderer* pRenderer, int* pSubtractXpos, int* pIncreaseXpos){
        if(pPlayers[*pNrOfPlayers]==0 && *pNrOfPlayers != MAX_PLAYERS)
        {
            if (*pNrOfPlayers % 2 > 0)
            {
                pPlayers[*pNrOfPlayers] = createPlayer((windowWidth / 2) + (*pSubtractXpos), startPlatformYPos - CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED); //?ndra starterpositions
                *pSubtractXpos -= 100;
            }
            else{
                pPlayers[*pNrOfPlayers] = createPlayer((windowWidth / 2) + (*pIncreaseXpos), startPlatformYPos - CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED); //?ndra starterpositions
                *pIncreaseXpos += 100;
            }

            pPlayerTextures[*pNrOfPlayers] = createPicture(pWindow, pRenderer, CHARACTER_PICTURE); //g?r en str?ng av detta ist
            (*pNrOfPlayersLeft) ++;
            (*pNrOfPlayers) ++;
        }
}


void handlePlayers(Player** pPlayers, int pNrOfPlayers, int *nrOfPlayersLeft, bool* pLeft, bool* pRight, bool* pMute, int windowWidth, int windowHeight, Platform* pStartPlatform, Mix_Chunk *pJumpSound, Mix_Chunk* pWinSound, State* pState, SDL_Renderer* pRenderer, SDL_Texture** pPlayerTextures, SDL_RendererFlip flip, Platform** pPlatforms, Text* pGameOverText){

    for (int i = 0; i < pNrOfPlayers; i++) //av n?gon anledning dyker inte player 2 upp, f?rmodligen pga samma bild och position, samt b?da r?r sig med tangenttrycken
    {
        if (i == 0) { //bara f?r att prova om spelare 2 dyker upp i loopen
            movePlayer(pPlayers[i], *pLeft, *pRight, windowWidth);
            jumpPlayer(pPlayers[i], pStartPlatform->yPos, pJumpSound, pMute);
            playerCollidePlatform(pPlayers[i], pPlatforms, pJumpSound, pMute);
            checkIfPlayerDead(pPlayers[i], windowHeight, pState, nrOfPlayersLeft);
            renderPlayer(pPlayers[i], pRenderer, pPlayerTextures[i], flip);
            if (!pPlayers[i]->alive) {
                renderText(pGameOverText);
            }
        }
        else {
            jumpPlayer(pPlayers[i], pStartPlatform->yPos, pJumpSound, pMute);
            playerCollidePlatform(pPlayers[i], pPlatforms, pJumpSound, pMute);
            checkIfPlayerDead(pPlayers[i], windowHeight, pState, nrOfPlayersLeft);
            renderPlayer(pPlayers[i], pRenderer, pPlayerTextures[i], SDL_FLIP_NONE);
        }
    }
    handleWin(*nrOfPlayersLeft, pState, pWinSound, pMute);
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

void jumpPlayer(Player* pPlayer, int startPlatformYPos, Mix_Chunk* pJumpSound, bool* pMute){
    if (pPlayer->alive) {
        pPlayer->yVelocity += (GRAVITY / 60);
        pPlayer->yPos += (pPlayer->yVelocity / 60);

        if (pPlayer->yPos <= 0) {
            pPlayer->yPos = 0;
            pPlayer->yVelocity = -(pPlayer->yVelocity) / 4;
        }
        else if (pPlayer->yPos >= startPlatformYPos - pPlayer->height) {
            pPlayer->yPos = startPlatformYPos - pPlayer->height;
            pPlayer->yVelocity = JUMP_SPEED;
            if (!(*pMute)) {
                Mix_VolumeChunk(pJumpSound, 10);
                Mix_PlayChannel(-1, pJumpSound, 0);
            }
        }
    }
}

void playerCollidePlatform(Player* pPlayer, Platform** pPlatforms, Mix_Chunk* pJumpSound, bool* pMute){
    if (pPlayer->alive) {
        int i;

        for (i = 0; pPlatforms[i] != 0; i++) {
            if (pPlayer->yVelocity > 0 &&
            pPlayer->xPos + pPlayer->width >= pPlatforms[i]->xPos &&
            pPlayer->xPos <= pPlatforms[i]->xPos + pPlatforms[i]->width &&
            pPlayer->yPos + pPlayer->height >= pPlatforms[i]->yPos &&
            pPlayer->yPos + pPlayer->height < pPlatforms[i]->yPos + pPlayer->yVelocity / 60) {
                pPlayer->yVelocity = JUMP_SPEED;
                if (!(*pMute)) {
                    Mix_PlayChannel(-1, pJumpSound, 0);
                }
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
    if (pPlayers[0]) {
        for (int i = 0; i != MAX_PLAYERS; i++) { //�ndra h�r till nr of players
            if (pPlayers[i]) {
                free(pPlayers[i]);
            }
    }
    }
}

void destroyPlayerTextures(SDL_Texture** pPlayerTextures) {
    if (pPlayerTextures[0]) {
        for (int i = 0; i != MAX_PLAYERS; i++) { //�ndra till nrofplayers
            destroyTexture(pPlayerTextures[i]);
        }
    }
}

int playerIsDead(Player* pPlayer, int windowHeight){
    if(pPlayer->alive && pPlayer->yPos + pPlayer->height >= windowHeight) {
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

void handleWin(int nrOfPlayersLeft, State* pState, Mix_Chunk* pWinSound, bool* pMute){
    if (nrOfPlayersLeft <= 1) {
        *pState = GAME_OVER;
        if (!(pMute)){
            Mix_PlayChannel(-1, pWinSound, 0);
        }
    }
}
