#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "tank.h"
#include "graphics.h"

// Function to render the borders of the window
void renderBorders(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 190, 0, 0, 255); 
    SDL_Rect borderRect = { 0, 0, 800, 600 }; 
    SDL_RenderDrawRect(renderer, &borderRect); 
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }
    // Create the SDL window
    SDL_Window* window = SDL_CreateWindow("Tank Commander", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Create the SDL renderer
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
    SDL_Texture* enemyBulletTexture = IMG_LoadTexture(renderer, "assets/EnemyAssets/enemy_bullet.png");
    // Check for texture loading errors
    if (!tankBodyTexture || !tankHeadTexture || !bulletTexture || !wallTexture || !enemyBodyTexture || !enemyHeadTexture) {
        printf("Failed to load textures! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    const int numEnemies = 1;

    // Initialize player tank and enemy tanks
    Tank tank;
    initTank(&tank, 400, 500, tankBodyTexture, tankHeadTexture);

    EnemyTank enemies[numEnemies];
    for (int i = 0; i < numEnemies; i++) {
        initEnemyTank(&enemies[i], 300 + (i * 50), 100, enemyBodyTexture, enemyHeadTexture);
    }

    // Initialize bullets and obstacles
    Bullet bullets[MAX_BULLETS] = {0}; // Limit the number of bullets to avoid overflow
    Obstacle obstacles[2] = {
        {{400, 300, 50, 50}, wallTexture}, 
        {{450, 300, 50, 50}, wallTexture}
    };

    int score = 0; // Player's score

    SDL_Event e;
    bool quit = false;

    // Main game loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Fire a bullet when spacebar is pressed
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                fireBullet(&tank, bullets, bulletTexture);
            }
        }

        // Get keyboard state for tank movement
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        moveTank(&tank, keyState, obstacles, 2, enemies, numEnemies);

        // Move bullets and check for collisions
        moveBullets(bullets, obstacles, 2);
        checkBulletEnemyCollision(bullets, enemies, numEnemies, &tank, &score);

        // Render the background color
        SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255); // Background color
        SDL_RenderClear(renderer);

        // Render obstacles
        for (int i = 0; i < 2; i++) {
            SDL_RenderCopy(renderer, obstacles[i].texture, NULL, &obstacles[i].rect);
        }
        
        moveEnemies(enemies, numEnemies, &tank);


        // Move and render enemy tanks
        for (int i = 0; i < numEnemies; i++) {
            renderEnemyTank(renderer, &enemies[i]);
            // enemyFire(&enemies[i], bullets, 1000, &tank, enemyBulletTexture); // Check if the enemy should fire
        }


        renderTank(renderer, &tank);
        renderBullets(renderer, bullets);
        renderBorders(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);  
    }

    // Cleanup and free resources
    SDL_DestroyTexture(tankBodyTexture);
    SDL_DestroyTexture(tankHeadTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(enemyBodyTexture);
    SDL_DestroyTexture(enemyHeadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
