#include <math.h>
#include "tank.h"

void initTank(Tank* tank, float x, float y, SDL_Texture* bodyTexture, SDL_Texture* headTexture) {
    tank->x = x;
    tank->y = y;
    tank->rotation = 0.0f; 
    
    tank->bodyTexture = bodyTexture;
    tank->headTexture = headTexture;
}

void moveTank(Tank* tank, const Uint8* keyState) {
    const float speed = 5.0f; 

    if (keyState[SDL_SCANCODE_UP]) {
        tank->x += speed * cos(tank->rotation * M_PI / 180.0f);
        tank->y += speed * sin(tank->rotation * M_PI / 180.0f);
    }
    if(keyState[SDL_SCANCODE_DOWN]) {
        tank->x -= speed * cos(tank->rotation * M_PI / 180.0f);
        tank->y -= speed * sin(tank->rotation * M_PI / 180.0f);
    }
    if(keyState[SDL_SCANCODE_LEFT]) {
        tank->rotation -= 5.0f;
    }
    if(keyState[SDL_SCANCODE_RIGHT]) {
        tank->rotation += 5.0f;
    }
    if(keyState[SDL_SCANCODE_A]){

    }
}

void renderTank(SDL_Renderer* renderer, Tank* tank) {
    // Render body
    SDL_Rect bodyRect = { (int)tank->x, (int)tank->y, 40, 50 };
    SDL_RenderCopyEx(renderer, tank->bodyTexture, NULL, &bodyRect, tank->rotation + 90, NULL, SDL_FLIP_NONE);

    // Render head
    SDL_Rect headRect = { (int)tank->x + 15, (int)tank->y + 15, 20, 20 }; 
    SDL_RenderCopyEx(renderer, tank->headTexture, NULL, &headRect, tank->rotation, NULL, SDL_FLIP_NONE);
}
