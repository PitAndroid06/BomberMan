#include <iostream>
using namespace std;

struct record{
  string nome;
  int punteggio;
  record* next = nullptr;
};

class Classifica{
  private:
    record* rec;
  public:
    Classifica();
    void addRecord(string name, int punteggio);
    record* getRecords();
};