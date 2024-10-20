#include "graphics.h"
#include <SDL2/SDL_image.h>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Texture* texture = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(filePath); // For PNG, use SDL_image library
    if (loadedSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    } else {
        printf("Unable to load texture: %s\n", SDL_GetError());
    }
    return texture;
}
