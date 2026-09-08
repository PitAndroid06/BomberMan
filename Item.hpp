#ifndef BOMBERMAN_ITEM_H
#define BOMBERMAN_ITEM_H

class Item {
private:
    int powerupTimer;    // Timer per la durata del powerup (in frame)
    int durationFrames;  // Durata totale in frame (es. 100 frame = 10 secondi a 10 FPS)

public:
    Item();

    // Raccoglie l'item e fa partire il timer del potenziamento
    void collect();

    // Da chiamare ogni iterazione del game loop per scalare il timer
    void update();

    // Restituisce il raggio della bomba: potenziato se l'item è attivo, normale altrimenti
    int getBombRadius();

    // Solo per la UI, per sapere se mostrare "Powerup attivo"
    bool isActive();
};

#endif // BOMBERMAN_ITEM_H
