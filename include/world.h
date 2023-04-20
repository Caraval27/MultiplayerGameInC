#ifndef WORLD_H
#define WORLD_H

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 1536

typedef struct background Background;

Background* createBackground(int windowHeight);
SDL_Texture* createBackgroundImage(SDL_Window* pWindow, SDL_Renderer* pRenderer);
void handleBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth);
void destroyBackground(Background* pBackground);
void destroyMusic(Mix_Music* pMainSound);

#endif
