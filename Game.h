#include <ncurses.h>
#include <iostream>
#include "Levels.h"
#include "Classifica.h"
using namespace std;

enum codeEntities{breakableWall, solidWall, player, enemy1, enemy2, bonus1, bonus2, bombAnimation1, bombAnimation2, explosion, lives};

const int CHARS_TO_DISPLAY = 11;

struct Entity{
  codeEntities codeEntity;
  const char* ch;
  int colorPair;
};

class Game{
  private:
    WINDOW* win = nullptr;
    Lvl* current_lvl = nullptr;
    Classifica classifica;
    Entity AllEntitiesChar[CHARS_TO_DISPLAY];
    int sizeGridX;
    int sizeGridY;

    void printEntity(codeEntities c);
    const char* getEntityChar(codeEntities c);
    int getEntityColor(codeEntities c);
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