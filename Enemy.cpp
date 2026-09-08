#include "Enemy.hpp"

Enemy::Enemy(int y, int x) : yLoc(y), xLoc(x) {}

void Enemy::mvup() {
    yLoc--;
}

void Enemy::mvdown() {
    yLoc++;
}

void Enemy::mvleft() {
    xLoc--;
}

void Enemy::mvright() {
    xLoc++;
}

void Enemy::UpdateLocation(int dir) {
  switch(dir) {
    case 0: // up
      mvup();
      break;
    case 1: // down
      mvdown();
      break;
    case 2: // left
      mvleft();
      break;
    case 3: // right
      mvright();
      break;
    default:
      break;
  }
}

int Enemy::getXLoc() { return xLoc; }
int Enemy::getYLoc() { return yLoc; }