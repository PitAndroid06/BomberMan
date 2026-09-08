#ifndef BOMBERMAN_TIME_H
#define BOMBERMAN_TIME_H

#include <ctime>

class Time {
private:
    time_t startTime; // Il momento (in secondi) in cui inizia il livello
    int timeLimit;    // Quanti secondi hai a disposizione (es. 120)
    bool isRunning = true;   // Indica se il timer è attivo
    bool timeOut = false;

public:
    Time();
    
    // Fa partire il cronometro
    void start(int limitInSeconds);
    
    // Restituisce i secondi rimanenti
    int getRemainingTime();
    
    // Controlla se il tempo è scaduto
    bool isTimeUp();
};

#endif // BOMBERMAN_TIME_H
