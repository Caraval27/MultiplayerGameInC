#pragma once

#define BACKGROUND_PICTURE "../assets/background.png"
#define BACKGROUND_WIDTH 1024
#define BACKGROUND_HEIGHT 1536
#define BACKGROUND_SPEED 1

typedef struct {
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    int windowWidth, windowHeight;
} GameDisplay;

typedef enum {
    MAIN_MENU,
	LOBBY_MENU,
    LOBBY,
	SETTINGS_MENU,
	ENTER_INPUT,
	ONGOING,
	GAME_MENU,
	GAME_OVER,
	QUIT
} State;

typedef struct{
    float upperSrcYPos, upperSrcHeight;
    float lowerSrcYPos, lowerSrcHeight;
    float upperDstYPos, upperDstHeight;
    float lowerDstYPos, lowerDstHeight;
} Background;

Background* createBackground(int windowHeight);
SDL_Texture* createPicture(GameDisplay* pGameDisplay, char picture[]);
void destroyTexture(SDL_Texture* pTexture);
void handleBackground(Background* pBackground, GameDisplay* pGameDisplay, SDL_Texture* pTexture);
void renderBackground(Background* pBackground, SDL_Renderer* pRenderer, SDL_Texture* pTexture, int windowWidth);
void destroyBackground(Background* pBackground);
void destroyMusic(Mix_Music* pMainSound);
void destroyChunk(Mix_Chunk* pMusicChunk);