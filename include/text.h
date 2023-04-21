#pragma once

typedef struct {
    SDL_Rect textRect;
    SDL_Texture* pTexture;
    SDL_Renderer* pRenderer;
} Text;

Text* createText(SDL_Renderer* pRenderer, TTF_Font* pFont, int r, int g, int b, char* pString, int windowWidth, int windowHeight, int addY, int addX);
void renderText(Text *pText);
void destroyText(Text *pText);
