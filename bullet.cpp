#include "bullet.h"

bullet::bullet(Map map,
               int x,
               int y,
               int direction,
               Player* p,
               int width,
               int height) {

    this->map = map;
    this->xLoc = x;
    this->yLoc = y;
    this->direction = direction;
    this->p = p;

    this->gridWidth = width;
    this->gridHeight = height;

    this->active = false;
}

void bullet::move() {

    switch (direction) {

        case 0: // UP
            yLoc--;
            break;

        case 1: // DOWN
            yLoc++;
            break;

        case 2: // LEFT
            xLoc--;
            break;

        case 3: // RIGHT
            xLoc++;
            break;
    }
    if (xLoc < 0 ||
        xLoc >= gridWidth ||
        yLoc < 0 ||
        yLoc >= gridHeight) {

        deactivate();
        return;
        }
    if (iswall(xLoc, yLoc)) {
        deactivate();
        return;
    }
    if (p->getXLoc() == xLoc &&
        p->getYLoc() == yLoc) {

        p->liveschange(0);
        deactivate();
        }
}

bool bullet::iswall(int x, int y) {

    if (x < 0 ||
        x >= gridWidth ||
        y < 0 ||
        y >= gridHeight) {

        return true;
        }
    return map[y][x] == '0' ||
           map[y][x] == '1';
}

void bullet::activate(int x, int y, int dir) {

    active = true;
    xLoc = x;
    yLoc = y;
    direction = dir;
}

void bullet::deactivate() {

    active = false;
}