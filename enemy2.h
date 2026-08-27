//
// Created by simone on 12/11/25.
//


#define BU 20
#include "Enemy.h"
#include <chrono>
#include "bullet.h"

class Enemy2 : public Enemy {
  private:
    int xPosPlayer;
    int yPosPlayer;
    bool bulletfired;
    std::chrono::steady_clock::time_point lastBulletTime;
    bullet*b[BU];
    bool wallbetween(Player*p);
    bool seePlayer(Player*p);
    int wherePlayer(Player*p);
    void firebullet(Player*p);
  public:
    Enemy2(int y, int x, int xPosPlayer, int yPosPlayer);

    int choosePath() override;
};