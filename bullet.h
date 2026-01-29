//
// Created by simone on 12/12/25.
//

#ifndef PLAYERENEMIES_BULLET_H
#define PLAYERENEMIES_BULLET_H
#include <ncurses.h>
#include "Player.h"


class bullet {
public:
    int xLoc,yLoc,direction;
    bool active;
    WINDOW*curwin;
    Player*p;
    bullet(WINDOW* win, int x, int y, int direction,Player*p);
    void move();
    bool iswall(int x, int y);
    void display();
    void activate(int x,int y,int dir);
    void deactivate();
    bool bulletfired();
};


#endif //PLAYERENEMIES_BULLET_H