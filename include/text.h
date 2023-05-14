#pragma once

#include "../include/world.h"

typedef struct {
    SDL_Texture* pTexture;
    SDL_Rect rect;
} Text;

Text* createText(GameDisplay* pGameDisplay, TTF_Font* pFont, char* pString, int red, int green, int blue, int xOffset, int yOffset);
void renderText(Text* pText, SDL_Renderer* pRenderer);
void destroyText(Text* pText);
