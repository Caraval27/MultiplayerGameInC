#ifndef menu_h
#define menu_h

#define MENU_IMAGE_WIDTH 2200
#define MENU_IMAGE_HEIGHT 1524

SDL_Texture* initMenuBackground(SDL_Window* pWindow, SDL_Renderer* pRenderer, SDL_Rect* pWindowUpperRect, SDL_Rect* pWindowLowerRect, SDL_Rect* pImageUpperRect, SDL_Rect* pImageLowerRect, int windowWidth, int windowHeight);
void renderMenuBackground(SDL_Renderer* pRenderer, SDL_Texture* pTexture, SDL_Rect imageUpperRect, SDL_Rect imageLowerRect, SDL_Rect windowUpperRect, SDL_Rect windowLowerRect);

#endif