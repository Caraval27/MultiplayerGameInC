#ifndef PLATFORMS_H
#define PLATFORMS_H

#define PLATFORM_WIDTH 100
#define PLATFORM_HEIGHT 10
#define MAX_NR_PLATFORMS 4

typedef struct platform Platform;

Platform* createPlatform(int windowHeight, int y);
void updatePlatform(Platform* pPlatform, SDL_Rect* pPlatformRect);
void destroyPlatform(Platform* pPlatform);

#endif