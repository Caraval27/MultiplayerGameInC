#ifndef PLATFORMS_H
#define PLATFORMS_H

#define PLATFORM_WIDTH 200
#define PLATFORM_HEIGHT 20
#define MAX_NR_PLATFORMS 4

typedef struct {
	float xPos, yPos, width, height;
} Platform;

Platform *createPlatform(float x, float y, float w, float h);
void scrollPlatform(Platform* pPlatform);
void renderPlatform(SDL_Renderer* pRenderer, Platform* pPlatform);
void handlePlatform(Platform** Platforms, SDL_Renderer* pRenderer, int windowWidth);
void destroyPlatform(Platform** Platforms);

#endif