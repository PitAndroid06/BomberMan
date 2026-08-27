//
// Created by simone on 12/11/25.
//

#include "Enemy2.h"

Enemy2::Enemy2(int y, int x, int xPosPlayer, int yPosPlayer) : Enemy(y, x) {
    this->xPosPlayer = xPosPlayer;
    this->yPosPlayer = yPosPlayer;
    /*
    lastBulletTime = std::chrono::steady_clock::now();
    for (int i = 0; i< BU; i++) {
        b[i]=new bullet(curwin,-1,-1,0,p);
    }
    */
}

int Enemy2::choosePath() {
    // Implement the logic for choosing a path for enemy2
    return 0; // Placeholder return value
}
/*
bool Enemy2::wallbetween(Player* p) {
    bool wall = false;

    if (yPosPlayer == yLoc) {
        int minx = min(xLoc, xPosPlayer);
        int maxx = max(xLoc, xPosPlayer);
        for (int x = minx + 1; x < maxx; x++)
            if ((mvwinch(curwin, yLoc, x) & A_CHARTEXT) == '#')
                wall = true;
    }
    else if (xPosPlayer == xLoc) {
        int miny = min(yLoc, yPosPlayer);
        int maxy = max(yLoc, yPosPlayer);
        for (int y = miny + 1; y < maxy; y++)
            if ((mvwinch(curwin, y, xLoc) & A_CHARTEXT) == '#')
                wall = true;
    }

    return wall;
}


bool Enemy2::seePlayer(Player *p) {
    bool see;
    if (xPosPlayer==xLoc||yPosPlayer==yLoc) {
        see = true;
    }else {see=false;}
    return see;
}

int Enemy2::wherePlayer(Player*p) {

    if (seePlayer(p)) {
        if (yPosPlayer==yLoc&&xPosPlayer<xLoc) {
            k=1;
        }else if (xPosPlayer==xLoc&&yPosPlayer<yLoc) {
            k=3;
        }else if (yPosPlayer==yLoc&&xPosPlayer>xLoc) {
            k=2;
        }else if (xPosPlayer==xLoc&&yPosPlayer>yLoc) {
            k=4;
        }
    }return k;
}

void Enemy2::firebullet(Player *p) {

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBulletTime).count() < 1000)
        return;
    if (!seePlayer(p)) return;
    if (wallbetween(p)) return;
    int dir = wherePlayer(p);
    if (dir == 0) return;
    for (int i = 0; i < BU; i++) {

        if (!b[i]->active) {

            b[i]->activate(xLoc, yLoc, dir);

            lastBulletTime = now;
            return;
        }
    }
}

*/


