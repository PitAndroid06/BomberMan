#include "bullet.h"

bullet::bullet(int x, int y, int dir, int sizeGridX, int sizeGridY) {

    xLoc = x;
    yLoc = y;
    direction = dir;

    gridWidth = sizeGridX;
    gridHeight = sizeGridY;

    active = false;
}

bool bullet::move(const Map map) {
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
        return false;
    }
    if (iswall(xLoc, yLoc, map)) {
        deactivate();
        return false;
    }
    if (map[yLoc][xLoc] == '2') {
        deactivate();
        return false;
    }
    lastMoveTime = clock();
    return true;
}

bool bullet::iswall(int x, int y, const Map map) {

    if (x < 0 ||
        x >= gridWidth ||
        y < 0 ||
        y >= gridHeight) {

        return true;
    }
    return map[y][x] == '0' || map[y][x] == '1';
}

void bullet::activate(int x, int y, int dir) {

    active = true;
    xLoc = x;
    yLoc = y;
    direction = dir;
    lastMoveTime = clock();
}

void bullet::deactivate() {
    active = false;
}