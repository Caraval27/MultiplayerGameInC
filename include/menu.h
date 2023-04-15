#ifndef MENU_H
#define MENU_H

#define MENU_IMAGE_WIDTH 2200
#define MENU_IMAGE_HEIGHT 1524

SDL_Texture* initMenuBackground(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pMenuBackgroundRect, int windowWidth, int windowHeight);
void renderMenuBackground(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect menuBackgroundRect);

#endif