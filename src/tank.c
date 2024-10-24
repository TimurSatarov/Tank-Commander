#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
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

void moveTank(Tank* tank, const Uint8* keyState, Obstacle obstacles[], int numObstacles, EnemyTank enemies[], int numEnemies) {
    float speed = 2.0f;
    float backSpeed = 1.0f;

    float newX = tank->x;
    float newY = tank->y;

    if (keyState[SDL_SCANCODE_UP]) {
        newX += speed * cos(tank->rotation * M_PI / 180.0f);
        newY += speed * sin(tank->rotation * M_PI / 180.0f);
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
        newX -= backSpeed * cos(tank->rotation * M_PI / 180.0f);
        newY -= backSpeed * sin(tank->rotation * M_PI / 180.0f);
    }

    SDL_Rect tankRect = {(int)newX, (int)newY, TANK_BODY_WIDTH, TANK_BODY_HEIGHT};

    bool canMove = true;

    for(int i = 0; i < numObstacles; i++) {
        if (checkRectCollision(tankRect, obstacles[i].rect)) {
            canMove = false;
            break;  
        }
    }

    for(int i = 0; i < numEnemies; i++) {
        if (enemies[i].active) {
            SDL_Rect enemyRect = {(int)enemies[i].tank.x, (int)enemies[i].tank.y, 50, 30};
            if (checkRectCollision(tankRect, enemyRect)) {
                canMove = false;
                break;
            }
        }
    }

    if (canMove) {
        tank->x = newX;
        tank->y = newY;
    }

    if (keyState[SDL_SCANCODE_LEFT]) {
        tank->rotation -= 2.0f;
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
        tank->rotation += 2.0f;
    }
    if (keyState[SDL_SCANCODE_A]) {
        tank->headRotation -= 3.0f;
    }
    if (keyState[SDL_SCANCODE_D]) {
        tank->headRotation += 3.0f;
    }

    if (tank->x < 0) tank->x = 0;
    if (tank->y < 0) tank->y = 0;
    if (tank->x > 800 - TANK_BODY_WIDTH) tank->x = 800 - TANK_BODY_WIDTH;
    if (tank->y > 600 - TANK_BODY_HEIGHT) tank->y = 600 - TANK_BODY_HEIGHT;

}


void renderTank(SDL_Renderer* renderer, Tank* tank) {
    // Render body
    SDL_Rect bodyRect = { (int)tank->x, (int)tank->y, TANK_BODY_WIDTH, TANK_BODY_HEIGHT };
    SDL_RenderCopyEx(renderer, tank->bodyTexture, NULL, &bodyRect, tank->rotation + 90, NULL, SDL_FLIP_NONE);

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

bool checkRectCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
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
                    SDL_Rect enemyRect = { (int)enemies[j].tank.x, (int)enemies[j].tank.y, 50, 36 };
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


void updateEnemy(EnemyTank* enemy, Tank* playerTank) {
    if (!enemy->active) return;

    // Calculate the direction to the player tank
    float dx = playerTank->x - enemy->tank.x;
    float dy = playerTank->y - enemy->tank.y;

    // Calculate the distance (length) between the enemy and player tanks
    float length = sqrt(dx * dx + dy * dy);

    // Normalize the direction vector (dx, dy)
    dx /= length;
    dy /= length;

    float rotatedDx = -dy;
    float rotatedDy = dx;

    enemy->tank.x += rotatedDx ;  
    enemy->tank.y += rotatedDy ;

    enemy->tank.rotation = atan2(rotatedDy, rotatedDx) * 180.0f / M_PI;

    enemy->tank.headRotation = enemy->tank.rotation;
}


void renderEnemyTank(SDL_Renderer* renderer, EnemyTank* enemy) {
    if (enemy->active) {
        // Render the tank's body with an extra 90-degree rotation
        SDL_Rect bodyRect = {(int)enemy->tank.x, (int)enemy->tank.y, TANK_BODY_WIDTH, TANK_BODY_HEIGHT};
        SDL_RenderCopyEx(renderer, enemy->tank.bodyTexture, NULL, &bodyRect, enemy->tank.rotation, NULL, SDL_FLIP_NONE);

        // Render the tank's head with an extra 90-degree rotation
        SDL_Rect headRect = {(int)enemy->tank.x+ 2, (int)enemy->tank.y - 10, TANK_HEAD_WIDTH, TANK_HEAD_HEIGHT};
        SDL_Point headCenter = {(TANK_HEAD_WIDTH / 2), (TANK_HEAD_HEIGHT / 2) + 10};
        SDL_RenderCopyEx(renderer, enemy->tank.headTexture, NULL, &headRect, enemy->tank.headRotation, &headCenter, SDL_FLIP_NONE);
    }
}
