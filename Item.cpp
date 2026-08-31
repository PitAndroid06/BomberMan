#include "Item.h"
#include <cstdlib> // per rand()

Item::Item() {
    powerupTimer = 0;
    // 100 frame corrispondono a ~10 secondi se il game loop gira a 10 FPS (napms(100))
    durationFrames = 100; 
}

void Item::tryDropItem(Map map, int x, int y) {
    // 30% di probabilità di far cadere un item quando un muro esplode
    if (rand() % 100 < 30) {
        map[y][x] = '5'; // '5' nella mappa corrisponde a codeEntities::bonus1
    }
}

void Item::collect() {
    powerupTimer = durationFrames; // Resetta il timer al massimo
}

void Item::update() {
    if (powerupTimer > 0) {
        powerupTimer--; // Il tempo passa
    }
}

int Item::getBombRadius() {
    if (powerupTimer > 0) {
        return 3; // Raggio potenziato
    }
    return 1;     // Raggio normale
}

bool Item::isActive() {
    return powerupTimer > 0;
}
