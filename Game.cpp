#include "Game.h"
#include <fstream>

/*caratteri per il gioco:

player: ◉
enemies: ▲, ■
bonus1: ✪
bonus2: ✚
bombs: o, ●
explosion: ✹
walls: █, ░
lives: ♥

*/

Game::Game(int start_x, int start_y, int width, int height)
    : bombHandler(width, height) {
  
  this->sizeGridX = width;
  this->sizeGridY = height;
  if(!this->loadLevels("config/levels.txt")) {
    cout<<"Avvio del programma non riuscito!"<<endl;
    return;
  }
  this->loadClassifica("config/scores.txt");

  setlocale(LC_ALL, "");
  initscr();
  cbreak();             // Permette input immediato
  noecho();             // Non mostra i tasti premuti
  curs_set(0);
  start_color();
  use_default_colors();

  short COLOR_ORANGE = 9;
  init_color(COLOR_ORANGE, 500,500,0);

  //hanno un valore +1 rispetto a EntityChar perchè init_pair come primo attributo ha value >= 1
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_WHITE);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_YELLOW, COLOR_BLACK);
  init_pair(7, COLOR_ORANGE, COLOR_BLACK);


  this->AllEntitiesChar[0] = {codeEntities::breakableWall, "░", 1};
  this->AllEntitiesChar[1] = {codeEntities::solidWall, "█", 2};
  this->AllEntitiesChar[2] = {codeEntities::player, "◉", 3};
  this->AllEntitiesChar[3] = {codeEntities::enemy1, "▲", 4};
  this->AllEntitiesChar[4] = {codeEntities::enemy2, "■", 4};
  this->AllEntitiesChar[5] = {codeEntities::bombAnimation1, "o", 5};
  this->AllEntitiesChar[6] = {codeEntities::bombAnimation2, "●", 5};
  this->AllEntitiesChar[7] = {codeEntities::bonus1, "✪", 6};
  this->AllEntitiesChar[8] = {codeEntities::bonus2, "✚", 3};
  this->AllEntitiesChar[9] = {codeEntities::explosion, "✹", 7};
  this->AllEntitiesChar[10] = {codeEntities::lives, "♥", 5};

  
  this->win = newwin(height+1, width, start_y, start_x);
  keypad(this->win, TRUE); // Abilita tasti speciali (F1, frecce, ecc.)
  nodelay(this->win, TRUE);// getch ritorna -1 se non c'è input
  
  mvprintw(start_y-1, start_x + (width/2)-4, "Bomberman");

  refresh();
}

void Game::printEntity(codeEntities c){
  int color = this->getEntityColor(c);
  wattron(this->win, COLOR_PAIR(color));
  wprintw(this->win, "%s", this->getEntityChar(c));
  wattroff(this->win, COLOR_PAIR(color));
}

const char* Game::getEntityChar(codeEntities c){
  for(int i = 0; i < CHARS_TO_DISPLAY; i++){
    if(this->AllEntitiesChar[i].codeEntity == c) return this->AllEntitiesChar[i].ch;
  }
  return "";
}

int Game::getEntityColor(codeEntities c){
  for(int i = 0; i < CHARS_TO_DISPLAY; i++){
    if(this->AllEntitiesChar[i].codeEntity == c) return this->AllEntitiesChar[i].colorPair;
  }
  return -1;
}

Game::~Game(){
  endwin();
  this->saveClassifica("config/scores.txt");
}

bool Game::isLoaded(){
  if(this->current_lvl == nullptr)return false;
  else return true;
}

int Game::checkInput(){
  return wgetch(this->win);
}

bool Game::loadLevels(string file){
  fstream inputFile;
  inputFile.open(file);
  char ch;
  while(inputFile.get(ch)){
    Map map = new char*[this->sizeGridY];
    for (int i = 0; i < this->sizeGridY; i++)
      map[i] = new char[this->sizeGridX];
    int row = 0;
    int col = 0;
    map[row][col] = ch;
    col++;
    while (inputFile.get(ch) && row < this->sizeGridY){
      if(ch == '\r')continue;
      try{
        if((ch >= 48 + 5 || ch < 48) && ch != '\n' && ch != ' ')throw ch;
        if (ch == '\n' || col >= this->sizeGridX){
          col = 0;
          row++;
          if (row >= this->sizeGridY) break;
          else continue;
        }
        map[row][col] = ch;
        col++;
      }catch(char Err){
        cout<<"Errore nella lettura del file \""<<file<<"\": carattere \'"<<Err<<"\' non valido!"<<endl;
        break;
      }
    }
    if(row == this->sizeGridY && col == 0){
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

void Game::loadClassifica(string file){
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

void Game::saveClassifica(string file){
  ofstream outputFile;
  outputFile.open(file);
  record* r = this->classifica.getRecords();
  while(r != nullptr){
    outputFile << r->nome << ", " << r->punteggio << endl;
    r = r->next;
  }
  outputFile.close();
}

void Game::drawLevel(){
  Map m = this->current_lvl->getMap();
  for(int i = 0; i < this->sizeGridY; i++){
    for(int j = 0; j < this->sizeGridX; j++){
      wmove(this->win, i, j);
      if(m[i][j] == ' ') wprintw(this->win, " ");
      else {
        switch(int(m[i][j]-48)){
          case codeEntities::breakableWall:
            printEntity(codeEntities::breakableWall);
            break;
          case codeEntities::solidWall:
            printEntity(codeEntities::solidWall);
            break;
          case codeEntities::player:
            printEntity(codeEntities::player);
            this->pl = Player(i, j);
            break;
          case codeEntities::enemy1:
            printEntity(codeEntities::enemy1);
            //init_enemy1
            break;
          case codeEntities::enemy2:
            printEntity(codeEntities::enemy2);
            //init_enemy2
            break;
          case codeEntities::bonus1:
            printEntity(codeEntities::bonus1);
            break;
          default: break;
        }
      }
    }
  }
  //n_lives
  wmove(this->win, this->sizeGridY, 1);
  for(int i = 0; i < 3; i++){
    this->printEntity(codeEntities::lives);
  }
  refresh();
}

int Game::showMenu(){
  int ch;
  int choise = 0;
  int coord[3][2] = {{(this->sizeGridX/3)+1,(this->sizeGridY/4)},
                     {(this->sizeGridX/3)-3,(this->sizeGridY/4)+2},
                     {(this->sizeGridX/3)+1,(this->sizeGridY/4)+4}};
  
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

void Game::showClassifica(int page, int rec_per_page){
  wclear(this->win);
  record* r = this->classifica.getRecords();
  if(r == nullptr && page == 0) mvwprintw(this->win, this->sizeGridY/4, (this->sizeGridX/4)+1, "Ancora nessun record");
  int pos = page*rec_per_page;
  for(int i = 0; i < pos; i++) r = r->next;
  do{
    string rec = r->nome + " - " + to_string(r->punteggio);
    if(pos < 3){
      rec = to_string((pos%5)+1) + ". " + rec;
      mvwprintw(this->win, (((pos%5)+1)*2), (this->sizeGridX/3), "%s", rec.c_str());
    } else 
    mvwprintw(this->win, (((pos%5)+1)*2), (this->sizeGridX/3)+3, "%s", rec.c_str());
    pos++;
    r = r->next;
    if(r == nullptr) break;
  }while(pos%5 != 0);
  string bar = "";
  if(page != 0)bar += "< ";
  else bar += "  ";
  bar += "Page " + to_string(page+1);
  if(r != nullptr)bar += " >";
  mvwprintw(this->win, this->sizeGridY - 2, (this->sizeGridX/2)-(bar.length()/2), "%s", bar.c_str());
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

bool Game::goToNextLevel(){
  if(this->current_lvl->isLast()) return false;
  else this->current_lvl = this->current_lvl->getNextLvl();
  return true;
}

bool Game::goToPrecLevel(){
  if(this->current_lvl->isFirst()) return false;
  else this->current_lvl = this->current_lvl->getPrecLvl();
  return true;
}

void Game::runTime(){
  int ch;
  while(1){
    ch = this->checkInput();
    if(ch == 'q') break;
    else if(ch != -1){
      int x = this->pl.getXLoc();
      int y = this->pl.getYLoc();

      // Controlla se il giocatore sta per raccogliere un item ('5' = bonus1)
      if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd') {
        int nextX = x, nextY = y;
        if (ch == 'w') nextY--;
        else if (ch == 's') nextY++;
        else if (ch == 'a') nextX--;
        else if (ch == 'd') nextX++;
        
        if (nextY >= 0 && nextY < this->sizeGridY && nextX >= 0 && nextX < this->sizeGridX) {
            if (this->current_lvl->getMap()[nextY][nextX] == '5') {
                this->itemHandler.collect();
            }
        }
      }

      Map update = this->pl.getmv(ch, this->current_lvl->getMap());
      if(update != nullptr){
        mvwprintw(this->win, y, x, " ");
        wmove(this->win, this->pl.getYLoc(), this->pl.getXLoc());
        printEntity(codeEntities::player);
        this->current_lvl->updateMap(update);
      }
      switch(ch){
        case ' ':
          this->bombHandler.placeBomb(this->pl.getXLoc(), this->pl.getYLoc(), this->itemHandler.getBombRadius(), 30);
          break;
        case 'e':
          //use cure
          break;
        default: break;
      }
    }
    // Aggiorna bomba e item
    this->itemHandler.update();
    this->bombHandler.update(this->current_lvl->getMap());

    // Gestisci esplosione
    if (this->bombHandler.hasExploded()) {
      ExplosionCell* cells = this->bombHandler.getExplosionCells();
      int size = this->bombHandler.getExplosionSize();

      // Controlla se il giocatore è stato colpito
      for (int i = 0; i < size; i++) {
        if (cells[i].x == this->pl.getXLoc() && cells[i].y == this->pl.getYLoc()) {
          this->pl.liveschange(0);
          break; // colpito una sola volta per esplosione
        }
      }

      // Disegna esplosione a schermo
      for (int i = 0; i < size; i++) {
        wmove(this->win, cells[i].y, cells[i].x);
        printEntity(codeEntities::explosion);
      }
      wrefresh(this->win);
      napms(500); // mostra esplosione per mezzo secondo

      // Pulisci celle dell'esplosione
      for (int i = 0; i < size; i++) {
        wmove(this->win, cells[i].y, cells[i].x);
        wprintw(this->win, " ");
      }
      // Ridisegna il giocatore (potrebbe essere su una cella colpita)
      wmove(this->win, this->pl.getYLoc(), this->pl.getXLoc());
      printEntity(codeEntities::player);

      // Aggiorna display vite
      wmove(this->win, this->sizeGridY, 1);
      for (int i = 0; i < this->pl.lives; i++) {
        printEntity(codeEntities::lives);
      }
      wprintw(this->win, "  ");

      this->bombHandler.clearExplosion();

      // Game over: il giocatore non ha più vite
      if (this->pl.lives <= 0) break;
    }

    // Disegna bomba se attiva
    bomb* b = this->bombHandler.getBomb();
    if (b != nullptr) {
      wmove(this->win, b->y, b->x);
      if (b->timer > 15) {
        printEntity(codeEntities::bombAnimation1);
      } else {
        printEntity(codeEntities::bombAnimation2);
      }
    }

    //move all enemies.
    //check if level is completed and break.
    wrefresh(this->win);
    napms(100); // ~10 FPS
  }
}