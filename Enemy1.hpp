//
// Created by simone on 12/10/25.
//

#include "Enemy.hpp"
#include <cstdlib>

class Enemy1 : public Enemy {
  private:
  public:
    Enemy1(int y, int x);
    int choosePath() override;
};