#include <cstring>
using namespace std;

struct record{
  char nome[8] = {};
  int punteggio;
  record* next = nullptr;
};

class Classifica{
  private:
    record* rec;
  public:
    Classifica();
    void addRecord(char name[8], int punteggio);
    record* getRecords();
};