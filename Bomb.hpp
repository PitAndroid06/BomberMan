#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

#include "Levels.hpp"

struct bomb {
    int x, y;       // x = colonna, y = riga
    int timer;       // turni rimanenti prima dell'esplosione
    int raggio;      // celle raggiunte in ogni direzione
    bool isActive;   // true se la bomba è piazzata
};

// Coordinate di una cella colpita dall'esplosione
struct ExplodedCell {
    int x, y;
};

// Massimo celle colpite: centro + 4 direzioni * raggio max ragionevole
const int MAX_EXPLOSION_CELLS = 41;

class BombHandler {
private:
    bomb activeBomb;
    int gridW, gridH;

    // Celle colpite dall'ultima esplosione (per Game)
    ExplodedCell explosionCells[MAX_EXPLOSION_CELLS];
    int numExplosionCells;
    bool exploded;

public:
    BombHandler(int gridWidth, int gridHeight);

    bool placeBomb(int x, int y, int raggio, int timer = 5);
    bool update(Map map);
    void explode(Map map);

    bomb getBomb();

    // Game usa questi per sapere cosa è stato colpito
    bool hasExploded();
    ExplodedCell* getExplodedCells();
    int getExplosionSize();
    void clearExplosion();
    bool isBombActive();
};

#endif // BOMBERMAN_BOMB_H