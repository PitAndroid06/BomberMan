//
// Created by simone on 12/10/25.
//

#include "enemy1.h"
enemy1::enemy1(WINDOW*win, int y, int x, char c)
{
    curwin=win;
    yLoc=y;
    xLoc=x;
    getmaxyx(curwin,yMax,xMax);
    keypad (curwin, true);
    character=c;
    dead = false;
}


void enemy1::mvup() {
    if (!iswall(yLoc - 1, xLoc))
        yLoc--;
    if (yLoc<1) yLoc=1;

}
void enemy1::mvdown() {
    if (!iswall(yLoc + 1, xLoc))
        yLoc++;
    if (yLoc>yMax-2) yLoc=yMax-2;
}
void enemy1::mvleft() {
    if (!iswall(yLoc , xLoc-1))
        xLoc--;
    if (xLoc<1) xLoc=1;
}
void enemy1::mvright() {
    if (!iswall(yLoc , xLoc+1))
        xLoc++;
    if (xLoc>xMax-2) xLoc=xMax-2;
}
int enemy1::getmv(int choice) {
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

void enemy1::display() {
 wattron(curwin,COLOR_PAIR(2));
    mvwaddch(curwin,yLoc,xLoc,character);
wattroff(curwin,COLOR_PAIR(2));
}
bool enemy1::iswall(int y, int x) {
    char ch = mvwinch(curwin, y, x) & A_CHARTEXT;
    return ch == '#';
}
bool enemy1::isdead (){
return dead;
}

void enemy1::setdead(bool d) {
    dead = d;
}
