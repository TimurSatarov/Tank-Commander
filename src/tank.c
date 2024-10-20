#include <math.h>
#include "tank.h"

void initTank(Tank* tank, float x, float y, SDL_Texture* bodyTexture, SDL_Texture* headTexture) {
    tank->x = x;
    tank->y = y;
    tank->rotation = 0.0f; 
    tank->headRotation = 0.0f;
    tank->bodyTexture = bodyTexture;
    tank->headTexture = headTexture;
}

void moveTank(Tank* tank, const Uint8* keyState) {
    const float speed = 2.5f; 
    const float backSpeed = 1.0f;

    if (keyState[SDL_SCANCODE_UP]) {
        tank->x += speed * cos(tank->rotation * M_PI / 180.0f);
        tank->y += speed * sin(tank->rotation * M_PI / 180.0f);
    }
    if(keyState[SDL_SCANCODE_DOWN]) {
        tank->x -= backSpeed * cos(tank->rotation * M_PI / 180.0f);
        tank->y -= backSpeed * sin(tank->rotation * M_PI / 180.0f);
    }
    if(keyState[SDL_SCANCODE_LEFT]) {
        tank->rotation -= 2.0f;
    }
    if(keyState[SDL_SCANCODE_RIGHT]) {
        tank->rotation += 2.0f;
    }
    if(keyState[SDL_SCANCODE_A]){
        tank->headRotation -= 3.0f;
    }
    if(keyState[SDL_SCANCODE_D]){
        tank->headRotation += 3.0f;
    }
}

void renderTank(SDL_Renderer* renderer, Tank* tank) {
    // Render body
    SDL_Rect bodyRect = { (int)tank->x, (int)tank->y, 36, 50 };
    SDL_RenderCopyEx(renderer, tank->bodyTexture, NULL, &bodyRect, tank->rotation + 90, NULL, SDL_FLIP_NONE);

    int headWidth = 30; // Width of the head image
    int headHeight = 50;

    
    // Render head
    //SDL_Rect headRect = { (int)tank->x -2, (int)tank->y - 10, headWidth, headHeight }; 
    float offsetX = -2.5f;  // X offset from the center of the body
    float offsetY = 0.0f;  // Y offset from the center of the body


    float rotatedOffsetX = offsetX * cos(tank->rotation * M_PI / 180.0f) - offsetY * sin(tank->rotation * M_PI / 180.0f);
    float rotatedOffsetY = offsetX * sin(tank->rotation * M_PI / 180.0f) + offsetY * cos(tank->rotation * M_PI / 180.0f);

    SDL_Rect headRect = {
        (int)(tank->x + (rotatedOffsetX + 4)),
        (int)(tank->y + (rotatedOffsetY -10)),
        headWidth,
        headHeight
    };
    // Correct rotation point of head
    SDL_Point headCenter = {(headWidth / 2), (headHeight / 2) + 10};


    SDL_RenderCopyEx(renderer, tank->headTexture, NULL, &headRect, tank->headRotation, &headCenter, SDL_FLIP_NONE);
}
