#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "tank.h"


#define TANK_BODY_WIDTH 36
#define TANK_BODY_HEIGHT 50
#define TANK_HEAD_WIDTH 30
#define TANK_HEAD_HEIGHT 50

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
    SDL_Rect bodyRect = { (int)tank->x, (int)tank->y, TANK_BODY_WIDTH, TANK_BODY_HEIGHT };
    SDL_RenderCopyEx(renderer, tank->bodyTexture, NULL, &bodyRect, tank->rotation + 90, NULL, SDL_FLIP_NONE);

    // Render head
    //SDL_Rect headRect = { (int)tank->x -2, (int)tank->y - 10, headWidth, headHeight }; 

    float offsetX = -2.5f;  // X offset from the center of the body
    float offsetY = 0.0f;  // Y offset from the center of the body


    float rotatedOffsetX = offsetX * cos(tank->rotation * M_PI / 180.0f) - offsetY * sin(tank->rotation * M_PI / 180.0f);
    float rotatedOffsetY = offsetX * sin(tank->rotation * M_PI / 180.0f) + offsetY * cos(tank->rotation * M_PI / 180.0f);

    SDL_Rect headRect = {
        (int)(tank->x + (rotatedOffsetX + 4)),
        (int)(tank->y + (rotatedOffsetY -10)),
        TANK_HEAD_WIDTH,
        TANK_HEAD_HEIGHT
    };
    // Correct rotation point of head
    SDL_Point headCenter = {(TANK_HEAD_WIDTH / 2), (TANK_HEAD_HEIGHT / 2) + 10};


    SDL_RenderCopyEx(renderer, tank->headTexture, NULL, &headRect, tank->headRotation + 90, &headCenter, SDL_FLIP_NONE);
}


void fireBullet(Tank* tank, Bullet bullets[],SDL_Texture* bulletTexture){
    for(int i = 0;i < MAX_BULLETS; i++){
        if(!bullets[i].active){
            bullets[i].x = tank->x + 15;
            bullets[i].y = tank->y + 22;
            bullets[i].dx = 10.0f * cos(tank -> headRotation * M_PI / 180.0f);
            bullets[i].dy = 10.0f * sin(tank -> headRotation * M_PI / 180.0f);
            bullets[i].active = true;
            bullets[i].bulletTexture = bulletTexture;
            break;
        }
    }
}


void moveBullets(Bullet bullets[], Obstacle obstacle[], int numObstacles){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bullets[i].active){
            bullets[i].x += bullets[i].dx;
            bullets[i].y += bullets[i].dy;

            if(bullets[i].x < 0 || bullets[i].x > 800 || bullets[i].y < 0 || bullets[i].y > 600){
                bullets[i].active = false;
            }

            for(int j = 0;j < numObstacles; j++){
                if(checkCollision(bullets[i].x, bullets[i].y, obstacle[j].rect)){
                    bullets[i].active = false;
                    break;
                }
            }
        }
    }
}


bool checkCollision(float bulletX, float bulletY, SDL_Rect wall) {
    return (bulletX >= wall.x && bulletX <= (wall.x + wall.w) &&
            bulletY >= wall.y && bulletY <= (wall.y + wall.h));
}

void renderBullets(SDL_Renderer* renderer, Bullet bullets[]){

    for(int i = 0;i < MAX_BULLETS; i++){
        if(bullets[i].active){
            SDL_Rect bulletRect = {(int)bullets[i].x,(int)bullets[i].y, 10, 10};
            SDL_RenderCopy(renderer, bullets[i].bulletTexture, NULL, &bulletRect);
        }
    }
}

int score = 0;  // score

void checkBulletEnemyCollision(Bullet bullets[], EnemyTank enemies[], int numEnemies, int* score) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < numEnemies; j++) {
                if (enemies[j].active) {
                    SDL_Rect enemyRect = { (int)enemies[j].tank.x, (int)enemies[j].tank.y, 50, 50 };
                    if (checkCollision(bullets[i].x, bullets[i].y, enemyRect)) {
                        bullets[i].active = false;
                        enemies[j].active = false;
                        (*score)++;  // Increase score
                        printf("Enemy hit! Score: %d\n", *score);
                        break;
                    }
                }
            }
        }
    }
}




void initEnemyTank(EnemyTank *enemy, float x, float y, SDL_Texture* bodyTexture, SDL_Texture* headTexture){
    initTank(&enemy->tank, x, y, bodyTexture, headTexture);
    enemy->active = true;
}


void updateEnemy(EnemyTank* enemy, Tank* playerTank){
    if(!enemy->active) return;

    float dx = playerTank->x - enemy->tank.x;
    float dy = playerTank->y - enemy->tank.y;

    float length = sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;

    enemy->tank.x += dx * 2.0f;
    enemy->tank.y += dy * 2.0f;

    enemy->tank.rotation = atan2(dy, dx) * 180.0f/M_PI;

    enemy->tank.headRotation = enemy->tank.rotation;
}
void renderEnemyTank(SDL_Renderer* renderer, EnemyTank* enemy){
    if(enemy->active){
        renderTank(renderer, &enemy->tank);
    }

}