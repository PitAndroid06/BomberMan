//
// Created by simone on 12/11/25.
//

#include "enemy2.h"
#include "Player.h"
#include "bullet.h"

int min(int a, int b) {return a > b ? b : a;}
int max(int a, int b) {return a < b ? b : a;}
enemy2::enemy2(WINDOW*win, int y, int x, char c,Player*pl)
{
    lastBulletTime = std::chrono::steady_clock::now();
    p=pl;
    curwin=win;
    yLoc=y;
    xLoc=x;
    getmaxyx(curwin,yMax,xMax);
    keypad (curwin, true);
    character=c;
    dead = false;
    bulletfired =false;
    getmaxyx(curwin, yMax, xMax);
    keypad(curwin, true);
    for (int i = 0; i< BU; i++) {
        b[i]=new bullet(curwin,-1,-1,0,p);
    }
}


void enemy2::mvup() {
    if (!iswall(yLoc - 1, xLoc))
        yLoc--;
    if (yLoc<1) yLoc=1;

}
void enemy2::mvdown() {
    if (!iswall(yLoc + 1, xLoc))
        yLoc++;
    if (yLoc>yMax-2) yLoc=yMax-2;
}
void enemy2::mvleft() {
    if (!iswall(yLoc , xLoc-1))
        xLoc--;
    if (xLoc<1) xLoc=1;
}
void enemy2::mvright() {
    if (!iswall(yLoc , xLoc+1))
        xLoc++;
    if (xLoc>xMax-2) xLoc=xMax-2;
}
int enemy2::getmv(int choice) {
    switch (choice) {
        case 1:
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvup();
            break;
        case 2:
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvdown();
            break;
        case 3:
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvright();
            break;
        case 4:
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvleft();
            break;
        default:
            break;
    }
    return choice;
}

void enemy2::display() {
    wattron(curwin,COLOR_PAIR(4));
    mvwaddch(curwin,yLoc,xLoc,character);
    wattroff(curwin,COLOR_PAIR(4));
}
bool enemy2::iswall(int y, int x) {
    char ch = mvwinch(curwin, y, x) & A_CHARTEXT;
    return ch == '#';
}
bool enemy2::isdead (){
    return dead;
}

void enemy2::setdead(bool d) {
    dead = d;
}

bool enemy2::wallbetween(Player* p) {
    bool wall = false;

    if (p->getYLoc() == yLoc) {
        int minx = min(xLoc, p->getXLoc());
        int maxx = max(xLoc, p->getXLoc());
        for (int x = minx + 1; x < maxx; x++)
            if ((mvwinch(curwin, yLoc, x) & A_CHARTEXT) == '#')
                wall = true;
    }
    else if (p->getXLoc() == xLoc) {
        int miny = min(yLoc, p->getYLoc());
        int maxy = max(yLoc, p->getYLoc());
        for (int y = miny + 1; y < maxy; y++)
            if ((mvwinch(curwin, y, xLoc) & A_CHARTEXT) == '#')
                wall = true;
    }

    return wall;
}


bool enemy2::seePlayer(Player *p) {
    bool see;
    if (p->getXLoc()==xLoc||p->getYLoc()==yLoc) {
        see = true;
    }else {see=false;}
    return see;
}

int enemy2::wherePlayer(Player*p) {

    if (seePlayer(p)) {
        if (p->getYLoc()==yLoc&&p->getXLoc()<xLoc) {
            k=1;
        }else if (p->getXLoc()==xLoc&&p->getYLoc()<yLoc) {
            k=3;
        }else if (p->getYLoc()==yLoc&&p->getXLoc()>xLoc) {
            k=2;
        }else if (p->getXLoc()==xLoc&&p->getYLoc()>yLoc) {
            k=4;
        }
    }return k;
}

void enemy2::firebullet(Player *p) {

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



