#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 1536

SDL_Texture * initBackground(SDL_Window *pWindow, SDL_Renderer *pRenderer);
void scrollBackground(SDL_Renderer *pRenderer, SDL_Texture *pTexture);