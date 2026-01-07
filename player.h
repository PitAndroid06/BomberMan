//
// Created by simone on 12/10/25.
//

#ifndef PLAYERENEMIES_PLAYER_H
#define PLAYERENEMIES_PLAYER_H
#include <ncurses.h>
#include <chrono>

class player {
    public:
    bool invulnerable = false;
    std::chrono::steady_clock::time_point lastDamageTime;

    int invulnerableTime = 1000;
    int yLoc, xLoc;
    player(WINDOW*win, int y, int x, char c);
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    int getmv(int ch);
    void display();
    bool iswall(int y, int x);
    bool isdead();
    void setdead(bool d);
    void invulnerability();
    void liveschange(int plusminus);

    int lives;
private:
    WINDOW *curwin;
    int yMax, xMax;
    char character;
    bool dead;

};


#endif //PLAYERENEMIES_PLAYER_H