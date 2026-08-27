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

Player::Player(){
    xLoc = 1;
    yLoc = 1;
    lives = 3;
}

bool Player::mvup(Map grid) {
    if (!iswall(grid, this->xLoc, this->yLoc - 1)){
        this->yLoc--;
        return true;
    }
    return false;
}

bool Player::mvdown(Map grid) {
    if (!iswall(grid, this->xLoc, this->yLoc + 1)){
        this->yLoc++;
        return true;
    }
    return false;
}

bool Player::mvleft(Map grid) {
    if (!iswall(grid, this->xLoc-1, this->yLoc)){
        this->xLoc--;
        return true;
    }
    return false;
}

bool Player::mvright(Map grid) {
    if (!iswall(grid, this->xLoc+1, this->yLoc)) {
        this->xLoc++;
        return true;
    }
    return false;
}

Map Player::getmv(int choice, Map grid) {
    switch (choice) {
        case 'w':
            if (mvup(grid)){
                grid[yLoc+1][xLoc] = ' ';
                grid[yLoc][xLoc] = '2';
                return grid;
            }
            break;
        case 's':
            if (mvdown(grid)){
                grid[yLoc-1][xLoc] = ' ';
                grid[yLoc][xLoc] = '2';
                return grid;
            }
            break;
        case 'd':
            if (mvright(grid)){
                grid[yLoc][xLoc-1] = ' ';
                grid[yLoc][xLoc] = '2';
                return grid;
            }
            break;
        case 'a':
            if (mvleft(grid)){
                grid[yLoc][xLoc+1] = ' ';
                grid[yLoc][xLoc] = '2';
                return grid;
            }
            break;
        default:
            break;
    }
    return nullptr;
}

bool Player::iswall(Map grid, int x, int y) {
    return grid[y][x] == '0' || grid[y][x] == '1';
}

bool Player::isdead() {
    return this->lives <= 0;
}

void Player::invulnerability() {
    if (invulnerable) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDamageTime).count() >= invulnerableTime) {
            invulnerable = false;
        }
    }
}

void Player::liveschange(int plusminus) {
    if (plusminus==1){lives++;}
    if (plusminus==0){lives--;}
}

int Player::getXLoc() {return xLoc;}

int Player::getYLoc(){return yLoc;}

void Player::resetPosition() {
    xLoc = 1;
    yLoc = 1;
}

int Player::getLives() {
    return lives;
}

void Player::updatePosition(int y, int x) {
    yLoc = y;
    xLoc = x;
}