//
// Created by simone on 12/10/25.
//

#include "Player.h"
Player::Player(int y, int x)
{
    yLoc=y;
    xLoc=x;
    lives=3;
}

bool Player::mvup(Map grid) {
    if (!iswall(grid, yLoc - 1, xLoc)){
        yLoc--;
        return true;
    }
    return false;

}

bool Player::mvdown(Map grid) {
    if (!iswall(grid, yLoc + 1, xLoc)){
        yLoc++;
        return true;
    }
    return false;
}

bool Player::mvleft(Map grid) {
    if (!iswall(grid, yLoc , xLoc-1)){
        xLoc--;
        return true;
    }
    return false;
}

bool Player::mvright(Map grid) {
    if (!iswall(grid, yLoc , xLoc+1)) {
        xLoc++;
        return true;
    }
    return false;
}

bool Player::getmv(int choice, Map grid) {
    switch (choice) {
        case 'w':
            return mvup(grid);
        case 's':
            return mvdown(grid);
        case 'd':
            return mvright(grid);
        case 'a':
            return mvleft(grid);
        default:
            break;
    }
}

bool Player::iswall(Map grid, int x, int y) {
    return grid[x][y] == 0 || grid[x][y] == 1;
}

bool Player::isdead() {
    return dead;
}

void Player::setdead(bool d) {
    if (d && !invulnerable) {
        dead= true;
        liveschange(0);
        invulnerable = true;
        lastDamageTime = std::chrono::steady_clock::now();

    }
}
void Player::invulnerability() {
    if (invulnerable) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDamageTime).count() >= invulnerableTime) {
            invulnerable = false;
            dead = false;
        }
    }
}

void Player::liveschange(int plusminus) {
    if (plusminus==1){lives++;}
    if (plusminus==0){lives--;}
}
