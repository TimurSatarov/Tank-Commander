#ifndef TANK_H
#define TANK_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define MAX_BULLETS 1

typedef struct {
    float x, y;        // tank position
    float rotation;    
    float headRotation;
    SDL_Texture* bodyTexture;   
    SDL_Texture* headTexture;   
} Tank;


typedef struct{
    float x, y;
    float dx, dy;
    bool active;
    SDL_Texture* bulletTexture;
}Bullet;

void initTank(Tank* tank, float x, float y, SDL_Texture* bodyTexture, SDL_Texture* headTexture);
void moveTank(Tank* tank, const Uint8* keyState);
void renderTank(SDL_Renderer* renderer, Tank* tank);

//bullet func
void fireBullet(Tank* tank, Bullet bullet[], SDL_Texture* bulletTexture);
void moveBullets(Bullet bullets[]);
void renderBullets(SDL_Renderer* renderer, Bullet bullet[]);

#endif
