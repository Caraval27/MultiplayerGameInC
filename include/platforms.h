#ifndef PLATFORMS_H
#define PLATFORMS_H

#define PLATFORM_WIDTH 200
#define PLATFORM_HEIGHT 20
#define MAX_NR_PLATFORMS 4

typedef struct platform Platform;

typedef struct {
	float xPos, yPos, width, height;
} Plank;

Plank *createPlank(float x, float y, float w, float h);
void scrollPlank(Plank *pPlank);
void renderPlank(SDL_Renderer *pRenderer, Plank *pPlank);

Platform* createPlatform(int windowHeight, int y);
void updatePlatform(Platform* pPlatform, SDL_Rect* pPlatformRect);
void destroyPlatform(Platform* pPlatform);

#endif