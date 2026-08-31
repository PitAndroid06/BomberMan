#include "Bomb.h"
#include <cstdlib>

BombHandler::BombHandler(int gridWidth, int gridHeight) {
    activeBomb.isActive = false;
    gridW = gridWidth;
    gridH = gridHeight;
    numExplosionCells = 0;
    exploded = false;
}

bool BombHandler::placeBomb(int x, int y, int raggio, int timer) {
    // Una sola bomba attiva alla volta
    if (activeBomb.isActive) {
        return false;
    }

    activeBomb.x = x;
    activeBomb.y = y;
    activeBomb.timer = timer;
    activeBomb.raggio = raggio;
    activeBomb.isActive = true;
    return true;
}

void BombHandler::update(Map map) {
    if (!activeBomb.isActive) return;

    activeBomb.timer--;

    if (activeBomb.timer <= 0) {
        explode(map);
        activeBomb.isActive = false; // "Distrugge" la bomba
    }
}

void BombHandler::explode(Map map) {
    if (!activeBomb.isActive) return;

    int bx = activeBomb.x;
    int by = activeBomb.y;
    int r = activeBomb.raggio;

    // Reset dati esplosione precedente
    numExplosionCells = 0;
    exploded = true;

    // Il centro dell'esplosione (dove stava la bomba)
    explosionCells[numExplosionCells++] = {bx, by};

    // 4 direzioni: su, giù, destra, sinistra
    int dx[] = {0, 0, 1, -1};
    int dy[] = {-1, 1, 0, 0};

    for (int dir = 0; dir < 4; dir++) {
        for (int step = 1; step <= r; step++) {
            int nx = bx + dx[dir] * step;
            int ny = by + dy[dir] * step;

            // Controllo limiti matrice — FONDAMENTALE per evitare crash
            if (nx < 0 || nx >= gridW || ny < 0 || ny >= gridH) {
                break;
            }

            // Muro solido: l'esplosione si ferma, il muro resta intatto
            if (map[ny][nx] == '1') {
                break;
            }

            // Registra cella come colpita dall'esplosione
            if (numExplosionCells < MAX_EXPLOSION_CELLS) {
                explosionCells[numExplosionCells++] = {nx, ny};
            }

            // Muro distruttibile: distruggi nella mappa e fermati
            if (map[ny][nx] == '0') {
                if (rand() % 100 < 30) {
                    map[ny][nx] = '5'; // Drop item
                } else {
                    map[ny][nx] = ' '; // Solo distruzione
                }
                break;
            }
        }
    }
}

bomb BombHandler::getBomb() {
    return activeBomb;
}

bool BombHandler::hasExploded() {
    return exploded;
}

ExplosionCell* BombHandler::getExplosionCells() {
    return explosionCells;
}

int BombHandler::getExplosionSize() {
    return numExplosionCells;
}

void BombHandler::clearExplosion() {
    exploded = false;
    numExplosionCells = 0;
}