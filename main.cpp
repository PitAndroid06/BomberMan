#include "Game.h"

int main() {
    Game screen(10, 5, 40, 20);
    int c = 1;
    while(1){
        c = screen.showMenu();
        if(c == 0){ //Nuova partità
            screen.startNewGame();
        } else if(c == 1){ //Visualizza classifica
            screen.showClassifica();
        } else if(c == 2){ //Esci dal gioco
            screen.quitGame();
            return 0;
        }
    }
    
    screen.quitGame();
    return 0;
}