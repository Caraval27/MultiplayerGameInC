#pragma once

#define Y_NR_OF_PLATFORMS 2
#define PLATFORM_PICTURE "../assets/steelPipe.png"
#define START_PLATFORM_PICTURE "../assets/iceBlock.png"
#define PLATFORM_WIDTH 100
#define PLATFORM_HEIGHT 20
#define PLATFORM_SPEED 2
#define NR_OF_PLATFORMS 30
#define PLATFORM_SPACING 100

typedef struct {
	float xPos, yPos;
    float width, height;
    bool created;
} Platform;

Platform *createPlatform(float xPos, float yPos, float width, float height);
void initPlatforms(Platform **pPlatforms, GameDisplay* pGameDisplay);
void scrollPlatform(Platform* pPlatform);
void renderPlatform(Platform* pPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture);
void handlePlatforms(Platform** pPlatforms, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth, int windowHeight, bool isHost);
void handleStartPlatform(Platform* pStartPlatform, Platform* pFirstPlatform, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowHeight, int* pTime);
void resetPlatforms(Platform** pPlatforms);
void resetStartPlatform(Platform* pStartPlatform, int windowHeight, int* pTime);
void destroyPlatform(Platform* pPlatform);
void destroyPlatforms(Platform** pPlatforms);
void cleanupPlatforms(Platform **pPlatforms, int windowHeight);