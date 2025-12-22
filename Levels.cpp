#include "Levels.h"

Lvl::Lvl(){
  this->map = nullptr;
  this->nextLvl = nullptr;
  this->precLvl = nullptr;
}

Lvl::Lvl(Map map){
  this->map = map;
  this->nextLvl = nullptr;
  this->precLvl = nullptr;
}

void Lvl::setNextLvl(Lvl* nextLvl){
  this->nextLvl = nextLvl;
}

void Lvl::setPrecLvl(Lvl* precLvl){
  this->precLvl = precLvl;
}

Lvl* Lvl::getNextLvl(){
  return this->nextLvl;
}

Lvl* Lvl::getPrecLvl(){
  return this->precLvl;
}

Map Lvl::getMap(){
  return this->map;
}

void Lvl::addLvl(Lvl* newLvl){
  newLvl->setPrecLvl(this);
  this->setNextLvl(newLvl);
}

bool Lvl::isFirst(){
  return this->precLvl == nullptr;
}

bool Lvl::isLast(){
  return this->nextLvl == nullptr;
}