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

void handleMainMenu(Game* pGame, SDL_Event event, bool* pMute);
void renderMainMenu(Game* pGame);
void handleSettingsMenu(Game* pGame, SDL_Event event, int* pNum, bool *pShowLang);
void renderSettingsMenu(Game *pGame);
void handleLanguageMenu(Game* pGame, SDL_Event event, bool* pShowLang);
void renderLanguageMenu(Game* pGame);
void readKeybindString(Game *pGame, int input);
void handleEnterInput(Game* pGame, SDL_Event event, int* pNum);
void handleLobbyMenu(Game* pGame, SDL_Event event, bool* pLeft, bool* pRight, int* pTime, bool* pCreateLobbyPressed);
void renderLobbyMenu(Game* pGame);
void handleLobby(Game* pGame, SDL_Event event, bool* pJoined, int* pIndex, bool* pCreateLobbyPressed);
void fillZero(Game* pGame, int max);
void handleOngoing(Game* pGame, SDL_Event event, bool* pIsRunning, bool *pLeft, bool *pRight, int* pTime, bool* pMute);
void handleOngoingInput(Game* pGame, SDL_Event* event, bool* pIsRunning, bool* pLeft, bool* pRight, bool* pMute);
void handleGameMenu(Game* pGame, SDL_Event event, bool* pMute);
void renderGameMenu(Game* pGame);
void handleGameOver(Game* pGame, SDL_Event event);
void renderGameOver(Game* pGame);

void resetGame(Game* pGame, bool* pLeft, bool* pRight, int* pTime);

void quitGame(Game* pGame);