//
// Created by simone on 12/10/25.
//

#include "Enemy1.h"

Enemy1::Enemy1(int y, int x) : Enemy(y, x) {}

int Enemy1::choosePath() {
    return rand()%4;
    return 0; // Placeholder return value
}