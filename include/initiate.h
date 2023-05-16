#pragma once

#include "../include/main.h"

int initiateSDLLibraries(void);
//handle error
//createpicture
void initiateButtons(Buttons* pButtons, GameDisplay* pGameDisplay);
void initiateLanguage(FILE* fp, Language* pLanguage, GameDisplay* pGameDisplay, Buttons* pButtons, DisplayText* pDisplayText);
