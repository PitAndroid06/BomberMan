#include "Enemy2.h"

Enemy2::Enemy2(int y, int x)
    : Enemy(y, x) {

    lastBulletTime = 0;

    for(int i = 0; i < BU; i++) {
        b[i] = nullptr;
    }
}

Enemy2::~Enemy2() {

    for(int i = 0; i < BU; i++) {
        delete b[i];
        b[i] = nullptr;
    }
}

int Enemy2::choosePath() {

    return rand() % 4;
}

bool Enemy2::seePlayer(Player* p) {

    int playerX = p->getXLoc();
    int playerY = p->getYLoc();

    return playerX == getXLoc() ||
           playerY == getYLoc();
}

int Enemy2::wherePlayer(Player* p) {

    int playerX = p->getXLoc();
    int playerY = p->getYLoc();

    if (playerY == getYLoc() &&
        playerX < getXLoc()) {

        return 2; // LEFT
    }

    if (playerY == getYLoc() &&
        playerX > getXLoc()) {

        return 3; // RIGHT
    }

    if (playerX == getXLoc() &&
        playerY < getYLoc()) {

        return 0; // UP
    }

    if (playerX == getXLoc() &&
        playerY > getYLoc()) {

        return 1; // DOWN
    }

    return -1;
}

bool Enemy2::wallbetween(Player* p, Map map) {

    int playerX = p->getXLoc();
    int playerY = p->getYLoc();

    int enemyX = getXLoc();
    int enemyY = getYLoc();

    // Stessa riga
    if (playerY == enemyY) {

        int minX = std::min(enemyX, playerX);
        int maxX = std::max(enemyX, playerX);

        for(int x = minX + 1; x < maxX; x++) {

            if(map[enemyY][x] == '0' ||
               map[enemyY][x] == '1') {

                return true;
            }
        }
    }

    // Stessa colonna
    else if (playerX == enemyX) {

        int minY = std::min(enemyY, playerY);
        int maxY = std::max(enemyY, playerY);

        for(int y = minY + 1; y < maxY; y++) {

            if(map[y][enemyX] == '0' ||
               map[y][enemyX] == '1') {

                return true;
            }
        }
    }

    return false;
}

void Enemy2::firebullet(Player* p, Map map) {

    time_t now = time(NULL);
    if(now - lastBulletTime < 1) {
        return;
    }
    if(!seePlayer(p)) {
        return;
    }
    if(wallbetween(p, map)) {
        return;
    }

    int dir = wherePlayer(p);

    if(dir == -1) {
        return;
    }
    for(int i = 0; i < BU; i++) {

        if(b[i] != nullptr &&
           !b[i]->active) {

            b[i]->activate(
                getXLoc(),
                getYLoc(),
                dir
            );

            lastBulletTime = now;

            return;
        }
    }
}

void Enemy2::initBullets(Map map,
                         Player* p,
                         int width,
                         int height) {

    for(int i = 0; i < BU; i++) {

        b[i] = new bullet(
            map,
            -1,
            -1,
            0,
            p,
            width,
            height
        );
    }
}

void Enemy2::updateBullets() {

    for(int i = 0; i < BU; i++) {

        if(b[i] != nullptr &&
           b[i]->active) {

            b[i]->move();
        }
    }
}