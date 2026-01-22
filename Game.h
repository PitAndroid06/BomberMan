#include <ncurses.h>
#include <iostream>
#include "Levels.h"
#include "Classifica.h"
using namespace std;

class Game{
  private:
    WINDOW* win = nullptr;
    Lvl* current_lvl = nullptr;
    Classifica classifica;
    int sizeX;
    int sizeY;

    void printEntity(int color, char ch);
  public:
    Game(int start_x, int start_y, int width, int height);
    ~Game();

    void drawLevel();
    
    int checkInput();

    bool loadLevels(string file);
    void loadClassifica(string file);
    void saveClassifica(string file);

    int showMenu();
    void showClassifica(int page = 0, int rec_per_page = 5);


    bool goToNextLevel();
    bool goToPrecLevel();

    bool isLoaded();
};