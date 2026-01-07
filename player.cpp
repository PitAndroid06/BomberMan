//
// Created by simone on 12/10/25.
//

#include "player.h"
player::player(WINDOW*win, int y, int x, char c)
{
    curwin=win;
    yLoc=y;
    xLoc=x;
    getmaxyx(curwin,yMax,xMax);
    keypad (curwin, true);
    character=c;
    lives=3;
}

void player::mvup() {
    if (!iswall(yLoc - 1, xLoc))
    yLoc--;
    if (yLoc<1) yLoc=1;

}
void player::mvdown() {
    if (!iswall(yLoc + 1, xLoc))
    yLoc++;
    if (yLoc>yMax-2) yLoc=yMax-2;
}
void player::mvleft() {
    if (!iswall(yLoc , xLoc-1))
    xLoc--;
    if (xLoc<1) xLoc=1;
}
void player::mvright() {
    if (!iswall(yLoc , xLoc+1))
    xLoc++;
    if (xLoc>xMax-2) xLoc=xMax-2;
}
int player::getmv(int choice) {
    switch (choice) {
        case 'w':
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvup();
            break;
        case 's':
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvdown();
            break;
        case 'd':
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvright();
            break;
        case 'a':
            mvwaddch(curwin,yLoc,xLoc,' ');
            mvleft();
            break;
        default:
            break;
    }
    return choice;
}

void player::display() {
     wattron(curwin,COLOR_PAIR(1));
    mvwaddch(curwin,yLoc,xLoc,character);
    wattroff(curwin,COLOR_PAIR(1));
}
bool player::iswall(int y, int x) {

    char ch = mvwinch(curwin, y, x) & A_CHARTEXT;
    return ch == '#';
}
bool player::isdead() {
    return dead;

}

void player::setdead(bool d) {
    if (d && !invulnerable) {
        dead= true;
        liveschange(0);
        invulnerable = true;
        lastDamageTime = std::chrono::steady_clock::now();

    }
}
void player::invulnerability() {
    if (invulnerable) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDamageTime).count() >= invulnerableTime) {
            invulnerable = false;
            dead = false;
        }
    }
}

void player::liveschange(int plusminus) {
    if (plusminus==1){lives++;}
    if (plusminus==0){lives--;}
}
