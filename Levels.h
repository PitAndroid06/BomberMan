typedef char** Map;

//const char* SOLID_WALL = "0\0";
//const char* BREAKABLE_WALL = "O\0";

class Lvl{
  private:
    Map map;
    Lvl* nextLvl;
    Lvl* precLvl;
    void setNextLvl(Lvl* nextLvl);
    void setPrecLvl(Lvl* nextLvl);
  public:
    Lvl();
    Lvl(Map map);
    Lvl* getNextLvl();
    Lvl* getPrecLvl();
    Map getMap();
    void updateMap(Map map);
    void addLvl(Lvl* newLvl);
    bool isFirst();
    bool isLast();
};