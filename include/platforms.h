#pragma once

#define PLATFORM_WIDTH 200
#define PLATFORM_HEIGHT 20
#define PLATFORM_SPEED 2
#define MAX_NR_PLATFORMS 4

typedef struct {
	float xPos, yPos;
    float width, height;
} Platform;

Platform *createPlatform(float xPos, float yPos, float width, float height);
void scrollPlatform(Platform* pPlatform);
void renderPlatform(SDL_Renderer* pRenderer, Platform* pPlatform);
void handlePlatform(Platform** Platforms, SDL_Renderer* pRenderer, int windowWidth);
void destroyPlatform(Platform** Platforms);
void resetPlatform(Platform** platforms);