#include <iostream>
#include "Levels.h" // Per il tipo Map

using namespace std;

struct bomb {
    int x, y;
    int timer;
    int raggio;
};

class BombHandler {
private:
    bomb* activeBomb;

public:
    BombHandler();
    ~BombHandler();


    bool placeBomb(int x, int y, int raggio, int timer = 5);

    void update(Map map);
    void explode(Map map);

    bomb* getBomb();
};