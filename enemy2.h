//
// Created by simone on 12/11/25.
//

#ifndef PLAYERENEMIES_ENEMY2_H
#define PLAYERENEMIES_ENEMY2_H

#define BU 20
#define BULLET_INTERVAL 10     // 3 secondi tra un colpo e l'altro

#include "Enemy.h"
#include "bullet.h"
#include <ctime>
#include <cstdlib>

class Enemy2 : public Enemy {

private:

  time_t lastBulletTime;

  bullet* b[BU];

  bool wallbetween(int playerX, int playerY, const Map map);
  bool seePlayer(int playerX, int playerY);
  int wherePlayer(int playerX, int playerY);

public:

  Enemy2(int y, int x);

  int choosePath() override;

  void initBullets(int sizeGridX, int sizeGridY);
  bullet* firebullet(int playerX, int playerY, const Map map);
  bullet* getBullet(int index);
};

#endif