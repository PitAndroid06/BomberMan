//
// Created by simone on 12/11/25.
//

#ifndef PLAYERENEMIES_ENEMY2_H
#define PLAYERENEMIES_ENEMY2_H
#define BU 20
#include <ncurses.h>
#include <chrono>

#include "bullet.h"
#include "player.h"
class enemy2 {
public:
    std::chrono::steady_clock::time_point lastBulletTime;

    int yLoc, xLoc;
    int k;
    enemy2(WINDOW * win, int y, int x, char c,player*p);
    bullet*b[BU];
    player*p;
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    int getmv(int choice);
    void display();
    bool iswall(int y, int x);
    bool isdead();
    void setdead(bool d);
    bool wallbetween(player*p);
    bool seeplayer(player*p);
    int whereplayer(player*p);
    void firebullet(player*p);
private:
    WINDOW *curwin;
    int yMax, xMax;
    char character;
    bool dead,bulletfired;
};



#endif //PLAYERENEMIES_ENEMY2_H