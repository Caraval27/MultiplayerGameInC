#pragma once

typedef struct {
    SDL_Texture* pTexture;
    SDL_Rect rect;
} Text;

Text* createText(SDL_Renderer* pRenderer, TTF_Font* pFont, char* pString, int red, int green, int blue, int windowWidth, int windowHeight, int xOffset, int yOffset);
void renderText(Text* pText, SDL_Renderer* pRenderer);
void destroyText(Text* pText);
void destroyTexts(Text** pTexts);
