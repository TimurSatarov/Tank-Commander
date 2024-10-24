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

    SDL_Window* window = SDL_CreateWindow("Tank Commander", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load textures
    SDL_Texture* tankBodyTexture = IMG_LoadTexture(renderer, "assets/tank_body_red.png");
    SDL_Texture* tankHeadTexture = IMG_LoadTexture(renderer, "assets/tank_turret_red.png");
    SDL_Texture* bulletTexture = IMG_LoadTexture(renderer, "assets/bullet_red.png");
    SDL_Texture* wallTexture = IMG_LoadTexture(renderer, "assets/wall.png");
    SDL_Texture* enemyBodyTexture = IMG_LoadTexture(renderer, "assets/EnemyAssets/enemy_tank_body.png");
    SDL_Texture* enemyHeadTexture = IMG_LoadTexture(renderer, "assets/EnemyAssets/enemy_tank_turret.png");

    const int numEnemies = 3;

    // Initialize player tank and enemy tanks
    Tank tank;
    EnemyTank enemies[numEnemies];
    initTank(&tank, 400, 500, tankBodyTexture, tankHeadTexture);
    initEnemyTank(&enemies[0], 400, 400, enemyBodyTexture, enemyHeadTexture);
    initEnemyTank(&enemies[1], 500, 100, enemyBodyTexture, enemyHeadTexture);
    initEnemyTank(&enemies[2], 600, 200, enemyBodyTexture, enemyHeadTexture);

    Bullet bullets[MAX_BULLETS] = {0};

    Obstacle obstacles[2] = {
        {{400, 300, 50, 50}, wallTexture},  // Position and dimensions
        {{450, 300, 50, 50}, wallTexture}
    };

    int score = 0;  // Declare and initialize score

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                fireBullet(&tank, bullets, bulletTexture);
            }
        }

        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        moveTank(&tank, keyState, obstacles, 2, enemies, numEnemies);

        

        moveBullets(bullets, obstacles, 2);
        checkBulletEnemyCollision(bullets, enemies, numEnemies, &score);

        // Render
        SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255); // Background color
        SDL_RenderClear(renderer);

        // Render obstacles
        for (int i = 0; i < 2; i++) {
            SDL_RenderCopy(renderer, obstacles[i].texture, NULL, &obstacles[i].rect);
        }

        // Render enemies
        for (int i = 0; i < numEnemies; i++) {
            renderEnemyTank(renderer, &enemies[i]);
        }

        // Render tank and bullets
        renderTank(renderer, &tank);
        renderBullets(renderer, bullets);

        // Render borders
        renderBorders(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Clean up
    SDL_DestroyTexture(tankBodyTexture);
    SDL_DestroyTexture(tankHeadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
