#include "classifica.h"

Classifica::Classifica(){
  this->rec = nullptr;
}

void Classifica::addRecord(string nome, int punteggio){
  record* new_record = new record{nome, punteggio};
  if(this->rec == nullptr || this->rec->punteggio <= punteggio){
    new_record->next = this->rec;
    this->rec = new_record;
    return;
  }
  record* temp = this->rec;
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
  return this->rec;
}