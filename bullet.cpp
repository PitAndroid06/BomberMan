//
// Created by simone on 12/12/25.
//

#include "bullet.h"
#include "player.h"
bullet::bullet(WINDOW *win, int x, int y, int direction,player*p) {
    curwin=win;
    xLoc=x;
    yLoc=y;
    this->direction=direction;
    this ->p=p;
    active=false;
}

void bullet::move() {
        switch(direction) {
            case 2:
                mvwaddch(curwin,yLoc,xLoc,' ');
                xLoc++;
                break;
            case 4:
                mvwaddch(curwin,yLoc,xLoc,' ');
                yLoc++;
                break;
            case 1:
                mvwaddch(curwin,yLoc,xLoc,' ');
                xLoc--;
                break;
            case 3:
                mvwaddch(curwin,yLoc,xLoc,' ');
                yLoc--;
                break;
        }
        if (iswall(xLoc,yLoc)||xLoc<=1||yLoc<=1||xLoc>=48||yLoc>=18) {deactivate();}
        else mvwaddch(curwin,yLoc,xLoc,'*');

        if (p->xLoc == xLoc && p->yLoc == yLoc) {
            p->setdead(true);
            deactivate();
        }
}
    bool bullet::iswall(int x, int y) {
        char ch = mvwinch(curwin, y, x) & A_CHARTEXT;
        return ch == '#';
    }
void bullet::activate(int x, int y, int direction) {
    active=true;
    xLoc=x;
    yLoc=y;
    this->direction=direction;


}
    void bullet::deactivate() {
        active=false;
        mvwaddch(curwin,yLoc,xLoc,' ');
    }