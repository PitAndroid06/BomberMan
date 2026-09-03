//
// Created by simone on 12/11/25.
//

#ifndef PLAYERENEMIES_ENEMY2_H
#define PLAYERENEMIES_ENEMY2_H

#define BU 20

#include "Enemy.h"
#include "bullet.h"
#include <ctime>
#include <algorithm>

class Enemy2 : public Enemy {

private:

  time_t lastBulletTime;

  bullet* b[BU];

  bool wallbetween(Player* p, Map map);
  bool seePlayer(Player* p);
  int wherePlayer(Player* p);

public:

  Enemy2(int y, int x);
  ~Enemy2();

  int choosePath() override;

  void initBullets(Map map, Player* p, int width, int height);
  void firebullet(Player* p, Map map);
  void updateBullets();
};

#endif