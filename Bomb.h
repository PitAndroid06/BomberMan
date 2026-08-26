#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

#include "Levels.h"

struct bomb {
    int x, y;       // x = colonna, y = riga
    int timer;       // turni rimanenti prima dell'esplosione
    int raggio;      // celle raggiunte in ogni direzione
};

// Coordinate di una cella colpita dall'esplosione
struct ExplosionCell {
    int x, y;
};

// Massimo celle colpite: centro + 4 direzioni * raggio max ragionevole
const int MAX_EXPLOSION_CELLS = 41;

class BombHandler {
private:
    bomb* activeBomb;
    int gridW, gridH;

    // Celle colpite dall'ultima esplosione (per Game)
    ExplosionCell explosionCells[MAX_EXPLOSION_CELLS];
    int numExplosionCells;
    bool exploded;

public:
    BombHandler(int gridWidth, int gridHeight);
    ~BombHandler();

    bool placeBomb(int x, int y, int raggio, int timer = 5);
    void update(Map map);
    void explode(Map map);

    bomb* getBomb();

    // Game usa questi per sapere cosa è stato colpito
    bool hasExploded();
    ExplosionCell* getExplosionCells();
    int getExplosionSize();
    void clearExplosion();
};

#endif // BOMBERMAN_BOMB_H