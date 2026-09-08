#include "Enemy2.h"

Enemy2::Enemy2(int y, int x)
    : Enemy(y, x) {

    lastBulletTime = 0;

    for(int i = 0; i < BU; i++) {
        b[i] = nullptr;
    }
}

int Enemy2::choosePath() {

    return rand() % 4;
}

bool Enemy2::seePlayer(int playerX, int playerY) {

    return playerX == getXLoc() || playerY == getYLoc();
}

int Enemy2::wherePlayer(int playerX, int playerY) {

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

bool Enemy2::wallbetween(int playerX, int playerY, const Map map) {

    int enemyX = getXLoc();
    int enemyY = getYLoc();

    // Stessa riga
    if (playerY == enemyY) {

        int minX = (enemyX < playerX) ? enemyX : playerX;
        int maxX = (enemyX > playerX) ? enemyX : playerX;

        for(int x = minX + 1; x < maxX; x++) {

            if(map[enemyY][x] == '0' ||
               map[enemyY][x] == '1') {

                return true;
            }
        }
    }

    // Stessa colonna
    else if (playerX == enemyX) {

        int minY = (enemyY < playerY) ? enemyY : playerY;
        int maxY = (enemyY > playerY) ? enemyY : playerY;

        for(int y = minY + 1; y < maxY; y++) {

            if(map[y][enemyX] == '0' ||
               map[y][enemyX] == '1') {

                return true;
            }
        }
    }

    return false;
}

bullet* Enemy2::firebullet(int playerX, int playerY, const Map map) {

    time_t now = time(NULL);
    if(now - lastBulletTime < BULLET_INTERVAL) {
        return nullptr;
    }
    if(!seePlayer(playerX, playerY)) {
        return nullptr;
    }
    if(wallbetween(playerX, playerY, map)) {
        return nullptr;
    }

    int dir = wherePlayer(playerX, playerY);

    if(dir == -1) {
        return nullptr;
    }

    //Controlla che il giocatore non sia adiacente all'enemy2 nella direzione del tiro
    switch(dir) {
        
        case 0: // UP
            if(getYLoc() - 1 == playerY) return nullptr;
            break;

        case 1: // DOWN
            if(getYLoc() + 1 == playerY) return nullptr;
            break;

        case 2: // LEFT
            if(getXLoc() - 1 == playerX) return nullptr;
            break;

        case 3: // RIGHT
            if(getXLoc() + 1 == playerX) return nullptr;
            break;
    }

    for(int i = 0; i < BU; i++) {

        if(b[i] != nullptr &&
           !b[i]->active) {

            switch(dir){
                case 0: // UP
                    b[i]->activate(getXLoc(), getYLoc()-1, dir);
                    break;
                case 1: // DOWN
                    b[i]->activate(getXLoc(), getYLoc()+1, dir);
                    break;
                case 2: // LEFT
                    b[i]->activate(getXLoc()-1, getYLoc(), dir);
                    break;
                case 3: // RIGHT
                    b[i]->activate(getXLoc()+1, getYLoc(), dir);
                    break;
            }
            

            lastBulletTime = now;

            return b[i];
        }
    }
    return nullptr;
}

void Enemy2::initBullets(int sizeGridX, int sizeGridY) {
    for(int i = 0; i < BU; i++) {
        b[i] = new bullet(-1, -1, 0, sizeGridX, sizeGridY);
    }
}

bullet* Enemy2::getBullet(int index) {
    if (index >= 0 && index < BU) {
        return b[index];
    }
    return nullptr;
  }