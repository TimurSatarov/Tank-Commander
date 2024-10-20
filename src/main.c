#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "tank.h"
#include "graphics.h"

int main(int argc, char* argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

   
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Battle City",
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
   
    Tank tank;
    initTank(&tank, 400, 300, tankBodyTexture, tankHeadTexture);

    
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Handle key states for movement
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        moveTank(&tank, keyState);

        
        SDL_SetRenderDrawColor(renderer, 70, 130, 170, 255); // Black background
        SDL_RenderClear(renderer);


        renderTank(renderer, &tank);

        
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
