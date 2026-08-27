#ifndef ENEMIES
#define ENEMIES

class Enemy {
  private:
    int yLoc, xLoc;
    int enemyType;

  public:
    Enemy(int y, int x);
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();
    void UpdateLocation(int dir);
    int getXLoc();
    int getYLoc();

    // 0 = up, 1 = down, 2 = left, 3 = right
    virtual int choosePath() = 0;
};


#endif //ENEMIES