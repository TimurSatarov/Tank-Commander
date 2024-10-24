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
    Tank tank;
    bool active;
} EnemyTank;


typedef struct{
    SDL_Rect rect;
    SDL_Texture* texture;
} Obstacle;




typedef struct{
    float x, y;
    float dx, dy;
    bool active;
    SDL_Texture* bulletTexture;
}Bullet;

void initTank(Tank* tank, float x, float y, SDL_Texture* bodyTexture, SDL_Texture* headTexture);
void moveTank(Tank* tank, const Uint8* keyState, Obstacle obstacles[], int numObstacles, EnemyTank enemies[], int numEnemies);
void renderTank(SDL_Renderer* renderer, Tank* tank);

//bullet func
void fireBullet(Tank* tank, Bullet bullet[], SDL_Texture* bulletTexture);
void moveBullets(Bullet bullets[], Obstacle obstacle[], int numObstacles);
void renderBullets(SDL_Renderer* renderer, Bullet bullet[]);

bool checkCollision(float bulletX, float bulletY, SDL_Rect wall); 
bool checkRectCollision(SDL_Rect a, SDL_Rect b);
void initEnemyTank(EnemyTank *enemy, float x, float y, SDL_Texture* bodyTexture, SDL_Texture* headTexture);
void updateEnemy(EnemyTank* enemy, Tank* playerTank);
void renderEnemyTank(SDL_Renderer* renderer, EnemyTank* enemy);
void checkBulletEnemyCollision(Bullet bullets[], EnemyTank enemies[], int numEnemies, int* score);
void renderEnemyTank(SDL_Renderer* renderer, EnemyTank* enemy);

#endif