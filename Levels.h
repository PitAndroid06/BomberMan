#ifndef BOMBERMAN_LEVELS_H
#define BOMBERMAN_LEVELS_H

typedef char** Map;

//const char* SOLID_WALL = "0\0";
//const char* BREAKABLE_WALL = "O\0";

class Lvl{
  private:
    Map map;
    Lvl* nextLvl;
    Lvl* precLvl;
    int LvlNumber;
    int timeCompletion = 200; // Tempo impiegato per completare il livello
    void setNextLvl(Lvl* nextLvl);
    void setPrecLvl(Lvl* precLvl);
  public:
    Lvl();
    Lvl(Map map);
    int getLvlNumber();
    Lvl* getNextLvl();
    Lvl* getPrecLvl();
    Map getMap();
    int getTimeCompletion();
    void setTimeCompletion(int time);
    void updateMap(Map map);
    void addLvl(Lvl* newLvl);
    bool isFirst();
    bool isLast();
    void deleteLvl(bool deleteNext = true);
};

#endif // BOMBERMAN_LEVELS_H