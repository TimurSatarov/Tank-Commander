#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y;        // tank position
    float rotation;    
    float headRotation;
    SDL_Texture* bodyTexture;   
    SDL_Texture* headTexture;   
} Tank;


void initTank(Tank* tank, float x, float y, SDL_Texture* bodyTexture, SDL_Texture* headTexture);
void moveTank(Tank* tank, const Uint8* keyState);
void renderTank(SDL_Renderer* renderer, Tank* tank);

#endif
