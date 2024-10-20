#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "tank.h"
#include "graphics.h"

void renderBorders(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 190, 0, 0, 255); // Red color for borders
    SDL_Rect borderRect = { 0, 0, 800, 600 }; // Full window size
    SDL_RenderDrawRect(renderer, &borderRect); // Draw the outline
}

int main(int argc, char* argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

   
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Tank Comander",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Texture* tankBodyTexture = IMG_LoadTexture(renderer, "assets/tank_body_red.png");
    if (!tankBodyTexture) {
        printf("Failed to load tank texture! IMG_Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Texture* tankHeadTexture = IMG_LoadTexture(renderer, "assets/tank_turret_red.png");
    if (!tankHeadTexture) {
        printf("Failed to load tank texture! IMG_Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Texture* bulletTexture = IMG_LoadTexture(renderer, "assets/bullet_red.png");
    if (!bulletTexture) {
        printf("Failed to load tank texture! IMG_Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
   
    Tank tank;
    initTank(&tank, 400, 300, tankBodyTexture, tankHeadTexture);

    Bullet bullets[MAX_BULLETS] = {0};
    
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){
                fireBullet(&tank, bullets, bulletTexture);
            }
        }

        
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        moveTank(&tank, keyState);
        moveBullets(bullets);


        if(tank.x < 0) tank.x = 0;
        if(tank.y < 0) tank.y = 0;
        if(tank.x > 750) tank.x = 750;
        if(tank.y > 550) tank.y = 550;    

        
        SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255); // Black background
        SDL_RenderClear(renderer);

        renderBorders(renderer);


        renderTank(renderer, &tank);
        renderBullets(renderer, bullets);

        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Clean 
    SDL_DestroyTexture(tankBodyTexture);
    SDL_DestroyTexture(tankHeadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
