#include "Time.h"

Time::Time() {
    startTime = 0;
    timeLimit = 0;
}

void Time::start(int limitInSeconds) {
    // time(NULL) restituisce i secondi correnti dal 1970.
    startTime = time(NULL);
    timeLimit = limitInSeconds;
}

int Time::getRemainingTime() {
    time_t now = time(NULL);
    
    // Quanti secondi sono passati
    int passedSeconds = (int)(now - startTime);
    
    // Quanti secondi mancano
    int remaining = timeLimit - passedSeconds;
    
    // Non andiamo sotto zero
    if (remaining < 0) {
        return 0;
    }
    
    return remaining;
}

bool Time::isTimeUp() {
    return getRemainingTime() <= 0;
}
