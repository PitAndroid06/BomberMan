#include "Classifica.hpp"

Classifica::Classifica(){
  rec = nullptr;
}

void Classifica::addRecord(char nome[8], int punteggio){
  record* new_record = new record;
  strncpy(new_record->nome, nome, sizeof(new_record->nome) - 1);
  new_record->nome[sizeof(new_record->nome) - 1] = '\0'; // Assicurati che la stringa sia terminata con '\0'
  new_record->punteggio = punteggio;

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