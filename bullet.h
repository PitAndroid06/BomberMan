//
// Created by simone on 12/12/25.
//

#ifndef PLAYERENEMIES_BULLET_H
#define PLAYERENEMIES_BULLET_H
#include <ncurses.h>
#include "player.h"


class bullet {
public:
    int xLoc,yLoc,direction;
    bool active;
    WINDOW*curwin;
    player*p;
    bullet(WINDOW* win, int x, int y, int direction,player*p);
    void move();
    bool iswall(int x, int y);
    void display();
    void activate(int x,int y,int dir);
    void deactivate();
    bool bulletfired();
};


#endif //PLAYERENEMIES_BULLET_H