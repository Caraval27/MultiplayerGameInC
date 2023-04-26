#pragma once

#include "../include/libraries.h"
#include "../include/world.h"
#include "../include/files.h"
#include "../include/players.h"
#include "../include/platforms.h"
#include "../include/menu.h"
#include "../include/text.h"
#include "../include/network.h"


int initiateGame(Game* pGame);
void initiateLanguage(FILE *fp, Game *pGame);
int handleError(Game* pGame, void* pMember, const char* (*GetError)(void));

void runGame(Game* pGame);

void handleMainMenu(Game* pGame, SDL_Event event, int* pTime);
void renderMainMenu(Game* pGame);
void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum);
void renderSettingsMenu(Game *pGame);
void handleEnterInput(Game* pGame, SDL_Event event, int* pNum);
void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool* pRight, bool* pLeft, int* pTime);
void handleInputOngoing(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pRight, bool* pLeft);
void handleGameMenu(Game* pGame, SDL_Event event);
void renderGameMenu(Game* pGame);
void handleGameOverMenu(Game* pGame, SDL_Event event);

void handlePlayers(Game* pGame, bool* pLeft, bool* pRight);
void initPlayers(Game* pGame);

void resetGame(Game* pGame, int* pTime);

void quitGame(Game* pGame);