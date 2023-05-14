#pragma once

#include "../include/world.h"
#include "../include/files.h"

typedef struct {
    SDL_Texture* pTexture;
    SDL_Rect rect;
} Text;

typedef struct {
    Text *pYouAreDeadText, *pWhoWonText, *pWaitingText, *pInputIPText, *pEnterIPText, *pNrClientsText;
} DisplayText;

typedef struct {
    bool chosenLanguage, showLang;
    int num;
    int keybinds[NR_OF_KEYBINDS];
    char language[NR_OF_WORDS][50];
} Language;

Text* createText(GameDisplay* pGameDisplay, TTF_Font* pFont, char* pString, int red, int green, int blue, int xOffset, int yOffset);
void renderText(Text* pText, SDL_Renderer* pRenderer);
void destroyText(Text* pText);
