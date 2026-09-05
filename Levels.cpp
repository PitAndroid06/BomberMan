#include "Levels.h"

Lvl::Lvl(){
  map = nullptr;
  nextLvl = nullptr;
  precLvl = nullptr;
}

Lvl::Lvl(Map m){
  map = m;
  nextLvl = nullptr;
  precLvl = nullptr;
  LvlNumber = 1;
}

void Lvl::setNextLvl(Lvl* nextLevel){
  nextLvl = nextLevel;
}

void Lvl::setPrecLvl(Lvl* precLevel){
  precLvl = precLevel;
}

Lvl* Lvl::getNextLvl(){
  return nextLvl;
}

Lvl* Lvl::getPrecLvl(){
  return precLvl;
}

Map Lvl::getMap(){
  return map;
}

void Lvl::addLvl(Lvl* newLvl){
  newLvl->setPrecLvl(this);
  setNextLvl(newLvl);
  newLvl->LvlNumber = LvlNumber + 1;
}

int Lvl::getLvlNumber(){
  return LvlNumber;
}

bool Lvl::isFirst(){
  return precLvl == nullptr;
}

bool Lvl::isLast(){
  return nextLvl == nullptr;
}

void Lvl::updateMap(Map m){
  map = m;
}

void Lvl::deleteLvl(bool deleteNext) {
  if(deleteNext && !isLast()) {
    Lvl* temp = nextLvl;
    nextLvl = nextLvl->getNextLvl();
    if(nextLvl != nullptr) nextLvl->setPrecLvl(this);
    delete temp;
  }
  if (!deleteNext && !isFirst()) {
    Lvl* temp = precLvl;
    precLvl = precLvl->getPrecLvl();
    if(precLvl != nullptr) precLvl->setNextLvl(this);
    delete temp;
  }
}

bool Lvl::getTimerInitialize() {
  return timerInitialize;
}

void Lvl::setTimerInitialize(bool time) {
  timerInitialize = time;
}