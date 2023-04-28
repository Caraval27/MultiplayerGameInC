#pragma once

#define PLATFORM_WIDTH 100
#define PLATFORM_HEIGHT 20
#define NR_PLATFORM_SAME_Y 2
#define PLATFORM_SPEED 2
#define MAX_NR_PLATFORMS 4

typedef struct {
	float xPos, yPos;
    float width, height;
} Platform;

Platform *createPlatform(float xPos, float yPos, float width, float height);
void scrollPlatform(Platform* pPlatform);
void renderPlatform(Platform* pPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void handlePlatforms(Platform** pPlatforms, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight);
void resetPlatforms(Platform** pPlatforms);
void destroyPlatform(Platform* pPlatform);
void destroyPlatforms(Platform** pPlatforms);
void cleanupPlatforms(Platform **pPlatforms, int windowHeight);
