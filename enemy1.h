//
// Created by simone on 12/10/25.
//

#ifndef PLAYERENEMIES_ENEMY1_H
#define PLAYERENEMIES_ENEMY1_H
#include <ncurses.h>

class enemy1 {
    public:
    int yLoc, xLoc;
    enemy1(WINDOW * win, int y, int x, char c);
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    int getmv(int choice);
    void display();
    bool iswall(int y, int x);
    bool isdead();
    void setdead(bool d);
private:
           WINDOW *curwin;
int yMax, xMax;
    char character;
    bool dead;
};


#endif //PLAYERENEMIES_ENEMY1_H