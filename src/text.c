#include "../include/main.h"

Text* createText(SDL_Renderer* pRenderer, TTF_Font* pFont, char* pString, int red, int green, int blue, int windowWidth, int windowHeight, int xOffset, int yOffset) {
    Text* pText = malloc(sizeof(Text));

    SDL_Color color = {red, green, blue};
    SDL_Surface* pSurface = TTF_RenderText_Solid(pFont, pString, color);
    if (!pSurface) {
        printf("Error: %s\n",SDL_GetError());
        exit(1);
    }
    pText->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    if (!pText->pTexture) {
        printf("Error: %s\n",SDL_GetError());
        exit(1);
    }
    SDL_FreeSurface(pSurface);
    SDL_QueryTexture(pText->pTexture, NULL, NULL, &pText->rect.w, &pText->rect.h);
    pText->rect.x = (windowWidth - pText->rect.w) / 2 + xOffset;
    pText->rect.y = (windowHeight - pText->rect.h) / 2 + yOffset;

    return pText;
}

void renderText(Text* pText, SDL_Renderer* pRenderer) {
    SDL_RenderCopy(pRenderer, pText->pTexture, NULL, &pText->rect);
}

void destroyText(Text* pText) {
    if (pText) {
        SDL_DestroyTexture(pText->pTexture);
        free(pText);
    }
}

void destroyTexts(Text** pTexts){
    int i;

    for(i = 0; i < MAX_PLAYERS + 1; i++) {
        destroyText(pTexts[i]);
    }
}
