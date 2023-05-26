#pragma once

#include "../include/main.h"

int initiateSDLLibraries(void);
int initiateDisplay(Game* pGame, GameDisplay* pGameDisplay);
int initiateTexture(Game* pGame, GameDisplay* pGameDisplay, Buttons* pButtons);
int initiateSound(Game* pGame, Sound* pSound);
int handleError(Game* pGame, void* pMember, const char* (*GetError)(void));
void initiateButtons(Buttons* pButtons, GameDisplay* pGameDisplay);
void initiateLanguage(FILE* fp, Language* pLanguage, GameDisplay* pGameDisplay, Buttons* pButtons, DisplayText* pDisplayText);