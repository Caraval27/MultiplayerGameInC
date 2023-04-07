#ifndef PLATFORMS_H
#define PLATFORMS_H

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 1536

SDL_Texture * initBackground(SDL_Window *pWindow, SDL_Renderer *pRenderer, SDL_Rect *pWindowUpper, SDL_Rect *pWindowLower, SDL_Rect *pImageUpper, SDL_Rect *pImageLower, int w, int h);
void scrollBackground(SDL_Rect *pWindowUpper, SDL_Rect *pWindowLower, SDL_Rect *pImageUpper, SDL_Rect *pImageLower, int h, SDL_Renderer *pRenderer, SDL_Texture *pTexture);

#endif
