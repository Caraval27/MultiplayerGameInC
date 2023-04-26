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
void renderPlatform(Platform* pPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void handlePlatforms(Platform** pPlatforms, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth);
void resetPlatforms(Platform** pPlatforms);
void destroyPlatform(Platform* pPlatform);
void destroyPlatforms(Platform** pPlatforms);