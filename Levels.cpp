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
  this->LvlNumber = 1;
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
  newLvl->LvlNumber = this->LvlNumber + 1;
}

int Lvl::getLvlNumber(){
  return this->LvlNumber;
}

bool Lvl::isFirst(){
  return this->precLvl == nullptr;
}

bool Lvl::isLast(){
  return this->nextLvl == nullptr;
}

void Lvl::updateMap(Map map){
  this->map = map;
}

void Lvl::deleteLvl(bool deleteNext) {
  if(deleteNext && !this->isLast()) {
    Lvl* temp = this->nextLvl;
    this->nextLvl = this->nextLvl->getNextLvl();
    if(this->nextLvl != nullptr) this->nextLvl->setPrecLvl(this);
    delete temp;
  }
  if (!deleteNext && !this->isFirst()) {
    Lvl* temp = this->precLvl;
    this->precLvl = this->precLvl->getPrecLvl();
    if(this->precLvl != nullptr) this->precLvl->setNextLvl(this);
    delete temp;
  }
}

int Lvl::getTimeCompletion() {
  return this->timeCompletion;
}

void Lvl::setTimeCompletion(int time) {
  this->timeCompletion = time;
}