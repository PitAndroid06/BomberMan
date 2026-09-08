//
// Created by simone on 12/10/25.
//

#ifndef PLAYERENEMIES_PLAYER_H
#define PLAYERENEMIES_PLAYER_H
#include <chrono>
#include "Levels.hpp"

class Player {
    public:
    bool invulnerable = false;
    std::chrono::steady_clock::time_point lastDamageTime;

    int invulnerableTime = 1000;
    
    Player();
    Player(int y, int x);
    bool mvup(Map grid);
    bool mvdown(Map grid);
    bool mvleft(Map grid);
    bool mvright(Map grid);
    Map getmv(int ch, Map grid);
    bool iswall(Map grid, int y, int x);
    bool isdead();
    void invulnerability();
    void liveschange(int plusminus);
    int getXLoc();
    int getYLoc();
    void resetPosition();
    int getLives();
    void updatePosition(int y, int x);

  private:
    int lives;
    int yLoc, xLoc;
};


#endif //PLAYERENEMIES_PLAYER_H