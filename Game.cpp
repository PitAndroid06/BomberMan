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
  this->start_x = start_x;
  this->start_y = start_y;
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

  
  this->win = newwin(height+1, width + 25, start_y, start_x);
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
  this->current_lvl = nullptr;
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
  bool drawedPlayer = false;
  this->currentEnemyCount = 0;

  //Disegna eventuali vie laterali
  if(!this->current_lvl->isFirst()){
    if(m[10][0] == '1'){
      m[10][0] = ' ';
      this->current_lvl->updateMap(m);
    }
    this->drawBackGateway(true);
  } else {
    m[10][0] = '1';
    this->current_lvl->updateMap(m);
    this->drawBackGateway(false);
  }
  this->drawNextGateway(this->isLevelCompleted());

  //Disegna tutto il livello
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
            this->pl.updatePosition(i, j);
            drawedPlayer = true;
            break;
          case codeEntities::enemy1:
            //init_enemy1
            if(this->currentEnemyCount < MAX_ENEMIES){
              printEntity(codeEntities::enemy1);
              enemies[this->currentEnemyCount] = new Enemy1(i, j);
              this->currentEnemyCount++;
            }
            break;
          case codeEntities::enemy2:
            //init_enemy2
            if(this->currentEnemyCount < MAX_ENEMIES){
              printEntity(codeEntities::enemy2);
              enemies[this->currentEnemyCount] = new Enemy2(i, j, this->pl.getXLoc(), this->pl.getYLoc());
              this->currentEnemyCount++;
            }
            break;
          case codeEntities::bonus1:
            printEntity(codeEntities::bonus1);
            break;
          case codeEntities::bonus2:
            printEntity(codeEntities::bonus2);
            break;
          default: break;
        }
      }
    }
  }
  
  //Disegna eventualmente il giocatore
  if(!drawedPlayer){
    wmove(this->win, this->pl.getYLoc(), this->pl.getXLoc());
    printEntity(codeEntities::player);
  }
  
  //Disegna le vite
  wmove(this->win, this->sizeGridY, 1);
  for(int i = 0; i < this->pl.getLives(); i++){
    this->printEntity(codeEntities::lives);
  }

  //Disegna il numero del livello
  wmove(this->win, this->sizeGridY, (this->sizeGridX/2)-4);
  string levelStr = "Level " + to_string(this->current_lvl->getLvlNumber());
  wprintw(this->win, "%s", levelStr.c_str());

  // Stampa tempo iniziale
  wmove(this->win, this->sizeGridY, this->sizeGridX - 12);
  string timeStr = "Time: " + to_string(this->timeHandler.getRemainingTime()) + "  ";
  wprintw(this->win, "%s", timeStr.c_str());

  refresh();
}

int Game::showMenu(){
  int ch;
  int choise = 0;
  int coord[3][2] = {{(this->sizeGridX/3)+1,(this->sizeGridY/4)},
                     {(this->sizeGridX/3)-3,(this->sizeGridY/4)+2},
                     {(this->sizeGridX/3)+1,(this->sizeGridY/4)+4}};

  wclear(this->win);
  mvprintw(start_y+9, start_x-1, " ");
  mvprintw(start_y+11, start_x-1, " ");
  refresh();
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

      if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd') {
        int nextX = x, nextY = y;
        if (ch == 'w') nextY--;
        else if (ch == 's') nextY++;
        else if (ch == 'a') nextX--;
        else if (ch == 'd') nextX++;
        
        // Controlla se il giocatore sta per raccogliere un item ('5' = bonus1, 6 = bonus2)
        if (nextY >= 0 && nextY < this->sizeGridY && nextX >= 0 && nextX < this->sizeGridX) {
          if (this->current_lvl->getMap()[nextY][nextX] == '5') {
            this->itemHandler.collect();
          } else if (this->current_lvl->getMap()[nextY][nextX] == '6') {
            this->pl.liveschange(1);
            updateLives();
          }
        }
        
        //Go to the next level if the player reaches the exit point (40, 10)
        if((nextX == 40 || nextX == -1) && nextY == 10){
          bool isCleared = this->isLevelCleared();
          if(nextX == 40){
            if(this->goToNextLevel()){
              if(isCleared){
                this->current_lvl->deleteLvl(false);
                this->score += this->timeHandler.getRemainingTime(); // Bonus tempo
              }
              this->timeHandler.start(200); // Riavvia timer
              this->pl.resetPosition();
              this->drawLevel();
            } else {
              if (isCleared) {
                this->score += this->timeHandler.getRemainingTime(); // Bonus tempo
              }
              this->GameOver("YOU WON!");
              break;
            }
          } else if(nextX == -1){
            if(this->goToPrecLevel()){
              if(isCleared){
                this->current_lvl->deleteLvl(true);
              }
              this->timeHandler.start(200); // Riavvia timer
              this->pl.resetPosition();
              this->drawLevel();
            }
          }
        } else {
          bool collision = false;
          Map update;
          for(int i = 0; i < this->currentEnemyCount; i++){
            if(nextX == this->enemies[i]->getXLoc() && nextY == this->enemies[i]->getYLoc()){
              this->pl.liveschange(0);
              this->pl.resetPosition();
              collision = true;
              updateLives();
              break;
            }
          }
          if(!collision){
            update = this->pl.getmv(ch, this->current_lvl->getMap());
          } else {
            update = this->current_lvl->getMap();
            update[y][x] = ' ';
            update[1][1] = '2';
          }
          if(update != nullptr){
            mvwprintw(this->win, y, x, " ");
            wmove(this->win, this->pl.getYLoc(), this->pl.getXLoc());
            printEntity(codeEntities::player);
            this->current_lvl->updateMap(update);
          }
        }
      }

      //Piazza la bomba
      if(ch == ' '){
        this->bombHandler.placeBomb(this->pl.getXLoc(), this->pl.getYLoc(), this->itemHandler.getBombRadius(), 200);
      }
    }
    // Aggiorna item
    this->itemHandler.update();

    // Aggiorna bomba e gestisci esplosioni
    if(this->bombHandler.update(this->current_lvl->getMap())) {

      ExplodedCell* cells = this->bombHandler.getExplodedCells();
      int size = this->bombHandler.getExplosionSize();
      
      //La bomba è esplosa
      if(this->bombHandler.isBombActive()){

        Map update = this->current_lvl->getMap();

        for (int i = 0; i < size; i++) {
        
          // Controlla se il giocatore è stato colpito
          if (cells[i].x == this->pl.getXLoc() && cells[i].y == this->pl.getYLoc()) {
            this->pl.liveschange(0);
            update[this->pl.getYLoc()][this->pl.getXLoc()] = ' ';
            this->pl.resetPosition();
            update[this->pl.getYLoc()][this->pl.getXLoc()] = '2';
            updateLives();
          }
        
          // Controlla se un nemico è stato colpito
          for(int j = 0; j < this->currentEnemyCount; j++){
            if(cells[i].x == this->enemies[j]->getXLoc() && cells[i].y == this->enemies[j]->getYLoc()){
              delete this->enemies[j];
              for(int k = j; k < this->currentEnemyCount-1; k++){
                this->enemies[k] = this->enemies[k+1];
              }
              this->currentEnemyCount--;

              update[cells[i].y][cells[i].x] = ' ';
              this->score += 5;
              if(currentEnemyCount == 0){
                update[10][39] = ' '; // Rimuovi il muro che blocca l'uscita
                this->drawNextGateway(true);
              }
              break;
            }
          }

          //Controlla se la cella colpita è una parete distruttibile
          if(update[cells[i].y][cells[i].x] == '0'){
            int chance = rand() % 100;
            if (chance < DROP_CHANCE) {
              if(rand() % 2 == 0)update[cells[i].y][cells[i].x] = '5'; // Drop powerup
              else update[cells[i].y][cells[i].x] = '6'; // Drop cure
            } else {
              update[cells[i].y][cells[i].x] = ' '; // Solo distruzione
            }
          }

          // Disegna esplosione a schermo
          wmove(this->win, cells[i].y, cells[i].x);
          printEntity(codeEntities::explosion);
        }

        this->current_lvl->updateMap(update);
        wrefresh(this->win);
      }
      // La bomba ha finito il tempo di animazione dell'esplosione
      else {

        const Map lvl = this->current_lvl->getMap();

        // Pulisci celle dell'esplosione
        for (int i = 0; i < size; i++) {
          wmove(this->win, cells[i].y, cells[i].x);
          switch(lvl[cells[i].y][cells[i].x]){
            case codeEntities::bonus1+48:
              printEntity(codeEntities::bonus1);
              break;
            case codeEntities::bonus2+48:
              printEntity(codeEntities::bonus2);
              break;
            case ' ':
              wprintw(this->win, " ");
              break;
            default: break;
          }
        }
        
        if(!this->pl.isdead()){
          wmove(this->win, this->pl.getYLoc(), this->pl.getXLoc());
          printEntity(codeEntities::player);
        }
        wrefresh(this->win);
        this->bombHandler.clearExplosion();
      }
      
    }

    // Disegna bomba se attiva
    bomb b = this->bombHandler.getBomb();
    if (b.isActive && !this->bombHandler.hasExploded()) {
      wmove(this->win, b.y, b.x);
      if (b.timer % 50 < 25) {
        printEntity(codeEntities::bombAnimation1);
      } else {
        printEntity(codeEntities::bombAnimation2);
      }
    }

    //move all enemies.
    for (int i = 0; i < this->currentEnemyCount; i++) {
      //Logica di movimento nemici (da implementare)
      // Controlla se il nemico ha colpito il giocatore
    }

    // Game over: il giocatore non ha più vite
    if (this->pl.isdead()){
      this->GameOver("GAME OVER!");
      break;
    }
    
    // Game over: tempo scaduto
    if (this->timeHandler.isTimeUp()) {
      this->GameOver("TIME OUT!");
      break;
    }

    // Aggiorna a schermo il tempo rimasto
    wmove(this->win, this->sizeGridY, this->sizeGridX - 12);
    wprintw(this->win, "%s", ("Time: " + to_string(this->timeHandler.getRemainingTime()) + "  ").c_str());

    wrefresh(this->win);
    napms(10); // ~100 FPS
  }
}

void Game::GameOver(const char* msg) {
  const int sidePanelX = this->sizeGridX + 3;

  mvwprintw(this->win, 5, sidePanelX+3, "%s", msg);
  mvwprintw(this->win, 7, sidePanelX+4, "You got");
  mvwprintw(this->win, 9, sidePanelX+3, "%d points", this->score);
  mvwprintw(this->win, 11, sidePanelX, "Insert your name:");
  wmove(this->win, 12, sidePanelX + 4);
  wrefresh(this->win);

  char name[18] = {};
  nodelay(this->win, FALSE);
  echo();
  curs_set(1);
  wmove(this->win, 12, sidePanelX + 4);
  wgetnstr(this->win, name, sizeof(name) - 1);
  noecho();
  curs_set(0);
  nodelay(this->win, TRUE);

  string playerName = name;
  if (playerName.empty()) playerName = "Anonimo";
  this->classifica.addRecord(playerName, this->score);
  mvwprintw(this->win, 14, sidePanelX, "Salvato in classifica");
  wrefresh(this->win);
}

void Game::startNewGame() {
  this->loadLevels("config/levels.txt");
  if(!this->isLoaded()) return;
  this->pl = Player();
  this->score = 0;
  this->timeHandler.start(200); // Inizia timer (200 secondi a livello)
  this->drawLevel();
  this->runTime();
}

bool Game::isLevelCompleted(){
  Map m = this->current_lvl->getMap();
  for(int i = 0; i < sizeGridY; i++){
    for(int j = 0; j < sizeGridX; j++){
      if(m[i][j]-48 == codeEntities::enemy1 || m[i][j]-48 == codeEntities::enemy2){
        return false;
      }
    }
  }
  return true;
}

bool Game::isLevelCleared(){
  Map m = this->current_lvl->getMap();
  for(int i = 0; i < sizeGridY; i++){
    for(int j = 0; j < sizeGridX; j++){
      if(m[i][j]-48 ==  codeEntities::breakableWall || m[i][j]-48 == codeEntities::enemy1 || m[i][j]-48 == codeEntities::enemy2){
        return false;
      }
    }
  }
  return true;
}

void Game::updateLives(){
  wmove(this->win, this->sizeGridY, 1);
  wprintw(this->win, "        ");
  wmove(this->win, this->sizeGridY, 1);
  for (int i = 0; i < this->pl.getLives(); i++) {
    printEntity(codeEntities::lives);
  }
}

void Game::drawNextGateway(bool open){
  if(open){
    wmove(this->win, 9, 40);
    printEntity(codeEntities::solidWall); // Disegna l'uscita
    wmove(this->win, 10, 39);
    wprintw(this->win, " ");
    wmove(this->win, 11, 40);
    printEntity(codeEntities::solidWall); // Disegna l'uscita
  } else {
    mvprintw(start_y+9, start_x+40, "%s", " ");
    mvprintw(start_y+11, start_x+40, "%s", " ");
    wmove(this->win, 10, 39);
    printEntity(codeEntities::solidWall);
  }
}

void Game::drawBackGateway(bool open){
  if(open){
    wmove(this->win, 10, 0);
    wprintw(this->win, "%s", " ");
    mvprintw(start_y+9, start_x-1, "%s", this->getEntityChar(codeEntities::solidWall));
    mvprintw(start_y+11, start_x-1, "%s", this->getEntityChar(codeEntities::solidWall));
  } else {
    mvprintw(start_y+10, start_x, "%s", this->getEntityChar(codeEntities::solidWall));
    mvprintw(start_y+9, start_x-1, " ");
    mvprintw(start_y+11, start_x-1, " ");
  }
}