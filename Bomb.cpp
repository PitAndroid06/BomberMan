#include "Bomb.h"
#include <cstdlib>

BombHandler::BombHandler(int gridWidth, int gridHeight) {
    this->activeBomb = nullptr;
    this->gridW = gridWidth;
    this->gridH = gridHeight;
    this->numExplosionCells = 0;
    this->exploded = false;
}

BombHandler::~BombHandler() {
    if (this->activeBomb != nullptr) {
        delete this->activeBomb;
        this->activeBomb = nullptr;
    }
}

bool BombHandler::placeBomb(int x, int y, int raggio, int timer) {
    // Una sola bomba attiva alla volta
    if (this->activeBomb != nullptr) {
        return false;
    }

    this->activeBomb = new bomb{x, y, timer, raggio};
    return true;
}

void BombHandler::update(Map map) {
    if (this->activeBomb == nullptr) return;

    this->activeBomb->timer--;

    if (this->activeBomb->timer <= 0) {
        this->explode(map);
        delete this->activeBomb;
        this->activeBomb = nullptr;
    }
}

void BombHandler::explode(Map map) {
    if (this->activeBomb == nullptr) return;

    int bx = this->activeBomb->x;
    int by = this->activeBomb->y;
    int r = this->activeBomb->raggio;

    // Reset dati esplosione precedente
    this->numExplosionCells = 0;
    this->exploded = true;

    // Il centro dell'esplosione (dove stava la bomba)
    this->explosionCells[this->numExplosionCells++] = {bx, by};

    // 4 direzioni: su, giù, destra, sinistra
    int dx[] = {0, 0, 1, -1};
    int dy[] = {-1, 1, 0, 0};

    for (int dir = 0; dir < 4; dir++) {
        for (int step = 1; step <= r; step++) {
            int nx = bx + dx[dir] * step;
            int ny = by + dy[dir] * step;

            // Controllo limiti matrice — FONDAMENTALE per evitare crash
            if (nx < 0 || nx >= this->gridW || ny < 0 || ny >= this->gridH) {
                break;
            }

            // Muro solido: l'esplosione si ferma, il muro resta intatto
            if (map[ny][nx] == '1') {
                break;
            }

            // Registra cella come colpita dall'esplosione
            if (this->numExplosionCells < MAX_EXPLOSION_CELLS) {
                this->explosionCells[this->numExplosionCells++] = {nx, ny};
            }

            // Muro distruttibile: distruggi nella mappa e fermati
            // (l'esplosione non attraversa un muro appena distrutto)
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

bomb* BombHandler::getBomb() {
    return this->activeBomb;
}

bool BombHandler::hasExploded() {
    return this->exploded;
}

ExplosionCell* BombHandler::getExplosionCells() {
    return this->explosionCells;
}

int BombHandler::getExplosionSize() {
    return this->numExplosionCells;
}

void BombHandler::clearExplosion() {
    this->exploded = false;
    this->numExplosionCells = 0;
}