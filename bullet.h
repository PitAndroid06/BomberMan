#ifndef PLAYERENEMIES_BULLET_H
#define PLAYERENEMIES_BULLET_H

#include "Levels.h"
#include "Player.h"

class bullet {

public:

    int xLoc;
    int yLoc;
    int direction;
    bool active;

    time_t lastMoveTime;

    int gridWidth;
    int gridHeight;

    bullet(int x, int y, int direction, int sizeGridX, int sizeGridY);

    bool move(const Map map);
    bool iswall(int x, int y, const Map map);
    void activate(int x, int y, int dir);
    void deactivate();
};

#endif