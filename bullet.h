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

    Map map;
    Player* p;

    int gridWidth;
    int gridHeight;

    bullet(Map map,
           int x,
           int y,
           int direction,
           Player* p,
           int width,
           int height);

    void move();
    bool iswall(int x, int y);
    void activate(int x, int y, int dir);
    void deactivate();
};

#endif