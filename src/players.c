#include "../include/main.h"

Player* createPlayer(float xPos, float yPos, float width, float height, float xVelocity, float yVelocity) {
    Player* pPlayer = malloc(sizeof(Player));

    pPlayer->ip = (IPaddress){0};
    pPlayer->alive = true;
    pPlayer->xPos = xPos;
    pPlayer->yPos = yPos;
    pPlayer->width = width;
    pPlayer->height = height;
    pPlayer->xVelocity = xVelocity;
    pPlayer->yVelocity = yVelocity;
    pPlayer->moveLeft = false;
    pPlayer->moveRight = false;
    pPlayer->flip = false;

    return pPlayer;
}

void initPlayer(Player** pPlayers, int* pNrOfPlayers, int* pNrOfPlayersLeft, GameDisplay* pGameDisplay, float startPlatformYPos, SDL_Texture** pPlayerTextures, int* pSubtractXPos, int* pIncreaseXPos, char avatar[]) {
        if (*pNrOfPlayers != MAX_PLAYERS) {
            pPlayerTextures[*pNrOfPlayers] = createPicture(pGameDisplay, avatar);
            if (*pNrOfPlayers % 2 == 1) {
                pPlayers[*pNrOfPlayers] = createPlayer((pGameDisplay->windowWidth / 2) + (*pSubtractXPos), startPlatformYPos - CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED); //?ndra starterpositions
                *pSubtractXPos -= 100;
            } else {
                pPlayers[*pNrOfPlayers] = createPlayer((pGameDisplay->windowWidth / 2) + (*pIncreaseXPos), startPlatformYPos - CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT, MOVE_SPEED, JUMP_SPEED); //?ndra starterpositions
                *pIncreaseXPos += 100;
            }

            (*pNrOfPlayers)++;
            (*pNrOfPlayersLeft) = 0;
        }
}

void movePlayer(Player* pPlayer, bool left, bool right, int windowWidth) {
    if (pPlayer->alive) {
        if (left && !right) {
            pPlayer->xPos -= (pPlayer->xVelocity) / 60;
        }
        else if (right && !left) {
            pPlayer->xPos += (pPlayer->xVelocity) / 60;
        }

        if (pPlayer->xPos < 0 - pPlayer->width/2) {
            pPlayer->xPos = windowWidth - pPlayer->width;
        }
        else if (pPlayer->xPos > windowWidth - pPlayer->width/2) {
            pPlayer->xPos = 0;
        }
    }
}

void jumpPlayer(Player* pPlayer, int startPlatformYPos, Mix_Chunk* pJumpSound, bool* pMute) {
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
                Mix_PlayChannel(-1, pJumpSound, 0);
            }
        }
    }
}

void playerCollidePlatform(Player* pPlayer, Platform** pPlatforms, Mix_Chunk* pJumpSound, bool* pMute) {
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

int playerIsDead(Player* pPlayer, int windowHeight) {
    if (pPlayer->alive && pPlayer->yPos + pPlayer->height >= windowHeight) {
        return 1;
    }
    return 0;
}

void checkIfPlayerDead(Player* pPlayer, int windowHeight, State* pState, int* pNrOfPlayersLeft) {
    if (playerIsDead(pPlayer, windowHeight)) {
        pPlayer->alive = false;
        (*pNrOfPlayersLeft)++;
    }
}

void handleWin(int nrOfPlayersLeft, int nrOfPlayers, State* pState, Mix_Chunk* pWinSound, bool* pMute) {
    if (nrOfPlayersLeft == nrOfPlayers - 1) {
        if (!(pMute)) {
            Mix_PlayChannel(-1, pWinSound, 0);
        }
        *pState = GAME_OVER;
    }
}

void renderPlayer(Player* pPlayer, SDL_Renderer* pRenderer, SDL_Texture* pTexture) {
    if (pPlayer->alive) {
        SDL_Rect rect = {pPlayer->xPos, pPlayer->yPos, pPlayer->width, pPlayer->height};

        SDL_RenderCopyEx(pRenderer, pTexture, NULL, &rect, 0.0, NULL, pPlayer->flip);
    }
}

void handlePlayers(Player** pPlayers, int pNrOfPlayers, int *nrOfPlayersLeft, bool* pMute, int windowWidth, int windowHeight, Platform* pStartPlatform, Mix_Chunk *pJumpSound, Mix_Chunk* pWinSound, State* pState, SDL_Renderer* pRenderer, SDL_Texture** pPlayerTextures, Platform** pPlatforms, Text* pGameOverText, bool* isHost) {
    for (int i = 0; i < pNrOfPlayers; i++) {
        if (*isHost) {
            movePlayer(pPlayers[i], pPlayers[i]->moveLeft, pPlayers[i]->moveRight, windowWidth);
            jumpPlayer(pPlayers[i], pStartPlatform->yPos, pJumpSound, pMute);
            playerCollidePlatform(pPlayers[i], pPlatforms, pJumpSound, pMute);
            checkIfPlayerDead(pPlayers[i], windowHeight, pState, nrOfPlayersLeft);
        }
        renderPlayer(pPlayers[i], pRenderer, pPlayerTextures[i]);
    }

    if (!pPlayers[0]->alive) {
        renderText(pGameOverText, pRenderer);
        *pState = GAME_OVER;
    }
    if (pNrOfPlayers == 1 && playerIsDead(pPlayers[0], windowWidth) == 1) {
        handleWin(*nrOfPlayersLeft, pNrOfPlayers, pState, pWinSound, pMute);
    }
    if (pNrOfPlayers > 1) {
        handleWin(*nrOfPlayersLeft, pNrOfPlayers, pState, pWinSound, pMute);
    }
}

void resetPlayers(Player** pPlayers, int* pNrOfPlayers, int* pNrOfPlayersLeft) {
    *pNrOfPlayers = 0;
    *pNrOfPlayersLeft = 0;
}

void destroyPlayers(Player** pPlayers) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (pPlayers[i]) {
            free(pPlayers[i]);
        }
    }
}

void destroyPlayerTextures(SDL_Texture** pPlayerTextures) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (pPlayerTextures[i]) {
            destroyTexture(pPlayerTextures[i]);
        }
    }
}