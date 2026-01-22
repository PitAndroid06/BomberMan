#include <ncurses.h>
#include "Game.h"

int main() {
    Game screen(10, 5, 40, 20);
    if(!screen.isLoaded()) return 0;
    int c = 1;
    while(1){
        c = screen.showMenu();
        if(c == 0){ //Nuova partità
            screen.drawLevel();
            int ch;
            while ((ch = screen.checkInput()) != 'q') {
                if(ch == 'd'){
                    if(screen.goToNextLevel()) screen.drawLevel();
                } else if(ch == 'a'){
                    if(screen.goToPrecLevel()) screen.drawLevel();
                }
            }
        } else if(c == 1){ //Visualizza classifica
            screen.showClassifica();
        } else if(c == 2){ //Esci dal gioco
            screen.~Game();
            return 0;
        }
    }
    
    screen.~Game();
    return 0;
}