#include "Bomb.h"
class player;
class enemy1;
class enemy2;

BombHandler::BombHandler() {
    this->activeBomb = nullptr;
}

bool BombHandler::placeBomb(int x, int y, int raggio, int timer) {

    if (this->activeBomb != nullptr) {
        return false;
    }

    this->activeBomb = new bomb{x, y, timer, raggio};
    return true;
}

void BombHandler::update(Map map) {
    if (this->activeBomb == nullptr) return;

    this->activeBomb->timer--;

    if (this->activeBomb->timer <= 0) {
        this->explode(map);
        delete this->activeBomb;
        this->activeBomb = nullptr;
    }
}

void BombHandler::explode(Map map, *player p, *enemy1 e1, *enemy2 e2) {
    if (this->activeBomb == nullptr) return;

    int bx = this->activeBomb->x;
    int by = this->activeBomb->y;
    int r = this->activeBomb->raggio;

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= r; j++) {
            int nx = bx + (dx[i] * j);
            int ny = by + (dy[i] * j);

            if (map[ny][nx] == '2') {
                p->liveschange(0);
            }

            if (map[ny][nx] == '3') {
                e1->setdead(true);
            }

            if (map[ny][nx] == '4') {
                e2->setdead(true);
            }

            if (map[ny][nx] == '0') {
                map[ny][nx] = ' ';
                break;
            }

            if (map[ny][nx] == '1') {
                break;
            }
        }
    }
}

bomb* BombHandler::getBomb() {
    return this->activeBomb;
}