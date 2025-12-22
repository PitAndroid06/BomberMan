#include "Screen.h"
#include <fstream>

Screen::Screen(int start_x, int start_y, int width, int height){
  
  this->sizeX = width;
  this->sizeY = height;
  if(!this->loadLevels("config/levels.txt")) {
    cout<<"Avvio del programma non riuscito!"<<endl;
    return;
  }
  this->loadClassifica("config/scores.txt");

  initscr();
  cbreak();             // Permette input immediato
  noecho();             // Non mostra i tasti premuti
  curs_set(0);
  
  this->win = newwin(height, width, start_y, start_x);
  keypad(this->win, TRUE); // Abilita tasti speciali (F1, frecce, ecc.)
  nodelay(this->win, TRUE);// getch ritorna -1 se non c'è input
  
  mvprintw(start_y-1, start_x + (width/2)-4, "Bomberman");

  refresh();
}

Screen::~Screen(){
  endwin();
  this->saveClassifica("config/scores.txt");
}

bool Screen::isLoaded(){
  if(this->current_lvl == nullptr)return false;
  else return true;
}

int Screen::checkInput(){
  return wgetch(this->win);
}

bool Screen::loadLevels(string file){
  fstream inputFile;
  inputFile.open(file);
  char ch;
  while(inputFile.get(ch)){
    Map map = new char*[this->sizeY];
    for (int i = 0; i < this->sizeY; i++)
      map[i] = new char[this->sizeX];
    int row = 0;
    int col = 0;
    map[row][col] = ch;
    col++;
    while (inputFile.get(ch) && row < this->sizeY){
      if(ch == '\r')continue;
      try{
        if(ch != '1' && ch != '0' && ch != ' ' && ch != '\n')throw ch;
        if (ch == '\n' || col >= this->sizeX){
          col = 0;
          row++;
          if (row >= this->sizeY) break;
          else continue;
        }
        map[row][col] = ch;
        col++;
      }catch(char Err){
        cout<<"Errore nella lettura del file \""<<file<<"\": carattere \'"<<Err<<"\' non valido!"<<endl;
        break;
      }
    }
    if(row == this->sizeY && col == 0){
      if(this->current_lvl == nullptr){
        this->current_lvl = new Lvl(map);
      } else {
        this->current_lvl->addLvl(new Lvl(map));
        this->current_lvl = this->current_lvl->getNextLvl();
      }
    } else {
      cout<<"Livello non caricato correttamente - Interrotto caricamento!"<<endl;
      return false;
    }
  }
  inputFile.close();
  while(this->current_lvl != nullptr && !this->current_lvl->isFirst()){
    this->current_lvl = this->current_lvl->getPrecLvl();
  }
  return true;
}

void Screen::loadClassifica(string file){
  fstream inputFile;
  inputFile.open(file);
  char ch;
  int stage = 0;
  string nome = "";
  string punteggio = "0";
  while(inputFile.get(ch)){
    if(ch == '\r' || ch == ' ')continue;
    if(ch == ','){
      stage++;
      continue;
    }
    if(ch == '\n'){
      stage++;
      if(stage == 2){
        this->classifica.addRecord(nome, stoi(punteggio));
        stage = 0;
        nome = "";
        punteggio = "0";
      }
      continue;
    }
    if(stage == 0){
      if(isalnum(ch))nome += ch;
    } else if(stage == 1){
      if(isdigit(ch))punteggio += ch;
      else {
        while(inputFile.get(ch) && ch != '\n'){}
        stage = 0;
        nome = "";
        punteggio = "0";
      }
    }
  }
  inputFile.close();
}

void Screen::saveClassifica(string file){
  ofstream outputFile;
  outputFile.open(file);
  record* r = this->classifica.getRecords();
  while(r != nullptr){
    outputFile << r->nome << ", " << r->punteggio << endl;
    r = r->next;
  }
  outputFile.close();
}

void Screen::drawLevel(){
  Map m = this->current_lvl->getMap();
  for(int i = 0; i < this->sizeY; i++){
    wmove(this->win, i, 0);
    
    for(int j = 0; j < this->sizeX; j++){
      if(m[i][j] == '1') wprintw(this->win, "0");
      else if(m[i][j] == '0') wprintw(this->win, "O");
      else if (m[i][j] == ' ') wprintw(this->win, " ");
    }
  }
  refresh();
}

int Screen::showMenu(){
  int ch;
  int choise = 0;
  int coord[3][2] = {{(this->sizeX/3)+1,(this->sizeY/4)},
                     {(this->sizeX/3)-3,(this->sizeY/4)+2},
                     {(this->sizeX/3)+1,(this->sizeY/4)+4}};
  
  wclear(this->win);
  mvwprintw(this->win, coord[0][1], coord[0][0]-3, ">  Nuova partita");
  mvwprintw(this->win, coord[1][1], coord[1][0]  , "Visualizza classifica");
  mvwprintw(this->win, coord[2][1], coord[2][0]  , "Esci dal gioco");
  while((ch = this->checkInput()) != '\n'){
    if(ch == KEY_UP){
      mvwprintw(this->win, coord[choise][1], coord[choise][0]-3, " ");
      if(choise == 0) choise = 2;
      else choise--;
    } else if(ch == KEY_DOWN){
      mvwprintw(this->win, coord[choise][1], coord[choise][0]-3, " ");
      if(choise == 2) choise = 0;
      else choise++;
    } else continue;
    mvwprintw(this->win, coord[choise][1], coord[choise][0]-3, ">");
  }
  return choise;
}

void Screen::showClassifica(int page, int rec_per_page){
  wclear(this->win);
  record* r = this->classifica.getRecords();
  if(r == nullptr && page == 0) mvwprintw(this->win, this->sizeY/4, (this->sizeX/4)+1, "Ancora nessun record");
  int pos = page*rec_per_page;
  for(int i = 0; i < pos; i++) r = r->next;
  do{
    string rec = r->nome + " - " + to_string(r->punteggio);
    if(pos < 3){
      rec = to_string((pos%5)+1) + ". " + rec;
      mvwprintw(this->win, (((pos%5)+1)*2), (this->sizeX/3), "%s", rec.c_str());
    } else 
    mvwprintw(this->win, (((pos%5)+1)*2), (this->sizeX/3)+3, "%s", rec.c_str());
    pos++;
    r = r->next;
    if(r == nullptr) break;
  }while(pos%5 != 0);
  string bar = "";
  if(page != 0)bar += "< ";
  else bar += "  ";
  bar += "Page " + to_string(page+1);
  if(r != nullptr)bar += " >";
  mvwprintw(this->win, this->sizeY - 2, (this->sizeX/2)-(bar.length()/2), "%s", bar.c_str());
  int ch;
  while((ch = this->checkInput()) != '\n'){
    if(ch == KEY_LEFT){
      if(page != 0){
        this->showClassifica(page-1);
        break;
      }
    } else if (ch == KEY_RIGHT){
      if(r != nullptr){
        this->showClassifica(page+1);
        break;
      }
    }
  }
}

bool Screen::goToNextLevel(){
  if(this->current_lvl->isLast()) return false;
  else this->current_lvl = this->current_lvl->getNextLvl();
  return true;
}

bool Screen::goToPrecLevel(){
  if(this->current_lvl->isFirst()) return false;
  else this->current_lvl = this->current_lvl->getPrecLvl();
  return true;
}