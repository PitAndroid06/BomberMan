#include "classifica.h"

Classifica::Classifica(){
  rec = nullptr;
}

void Classifica::addRecord(string nome, int punteggio){
  record* new_record = new record{nome, punteggio};
  if(rec == nullptr || rec->punteggio <= punteggio){
    new_record->next = rec;
    rec = new_record;
    return;
  }
  record* temp = rec;
  record* prec = temp;
  while(temp->next != nullptr){
    if(temp->punteggio <= punteggio){
      prec->next = new_record;
      new_record->next = temp;
      return;
    }
    prec = temp;
    temp = temp->next;
  }
  temp->next = new_record;
}

record* Classifica::getRecords(){
  return rec;
}