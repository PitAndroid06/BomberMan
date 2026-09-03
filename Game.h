#ifndef BOMBERMAN_GAME_H
#define BOMBERMAN_GAME_H

#include <ncurses.h>
#include <iostream>
#include <string>
#include <ctime>
#include "Classifica.h"
#include "Player.h"
#include "Bomb.h"
#include "Item.h"
#include "Time.h"
#include "enemy1.h"
#include "enemy2.h"
using namespace std;

enum codeEntities{breakableWall, solidWall, player, enemy1, enemy2, bonus1, bonus2, bombAnimation1, bombAnimation2, explosion, lives};

const int CHARS_TO_DISPLAY = 11;
const int MAX_ENEMIES = 10;
const int DROP_CHANCE = 30;
const int ENEMY_MOVE_INTERVAL = 1;

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
    Player pl;
    Enemy* enemies[MAX_ENEMIES];
    int currentEnemyCount = 0;
    int sizeGridX;
    int sizeGridY;
    int start_x;
    int start_y;
    int score;
    BombHandler bombHandler;
    Item itemHandler;
    Time timeHandler;
    time_t lastEnemyMoveTime;

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

    void runTime();

    void GameOver(const char* msg);

    void startNewGame();

    bool isLevelCompleted();

    bool isLevelCleared();

    void updateLives();

    void drawNextGateway(bool open);
    void drawBackGateway(bool open);
};

#endif // BOMBERMAN_GAME_H