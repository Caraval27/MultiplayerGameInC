#include "../include/main.h"

Text* createText(SDL_Renderer* pRenderer, TTF_Font* pFont, int r, int g, int b, char* pString, int posX, int posY) {
    Text* pText = malloc(sizeof(Text));

    pText->pRenderer = pRenderer;
    SDL_Color textColor = {r, g, b};
    SDL_Surface* pSurface = TTF_RenderText_Solid(pFont, pString, textColor);
    if (!pSurface) {
        printf("Error: %s\n",SDL_GetError());
        exit(1);
    }
    pText->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pText->pTexture) {
        printf("Error: %s\n",SDL_GetError());
        exit(1);
    }
    SDL_QueryTexture(pText->pTexture, NULL, NULL, &pText->textRect.w, &pText->textRect.h);
    pText->textRect.x = posX - pText->textRect.w/2;
    pText->textRect.y = posY - pText->textRect.h/2;

    return pText;
}

void renderText(Text *pText) {
    SDL_RenderCopy(pText->pRenderer, pText->pTexture, NULL, &pText->textRect);
}

void destroyText(Text *pText) {
    SDL_DestroyTexture(pText->pTexture);
    free(pText);
}