#include "Game.hpp"
#include <cstring>

/*

Caratteri per il gioco:

player: @
enemies: M, W
bonus1: $
bonus2: +
bombs: 0, O
explosion: *
walls: ' ', %
lives: V
bulletLeft: <
bulletRight: >
bulletUp: ^
bulletDown: v

*/

Game::Game(int initialX, int initialY, int gridWidth, int gridHeight)
    : bombHandler(gridWidth, gridHeight) {
  
  sizeGridX = gridWidth;
  sizeGridY = gridHeight;
  start_x = initialX;
  start_y = initialY;
  loadClassifica("config/scores.txt");

  setlocale(LC_ALL, "");
  initscr();
  cbreak();             // Permette input immediato
  noecho();             // Non mostra i tasti premuti
  curs_set(0);
  start_color();
  use_default_colors();

  //hanno un valore +1 rispetto a EntityChar perchè init_pair come primo attributo ha value >= 1
  init_pair(1, COLOR_BLACK, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_WHITE);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_YELLOW, COLOR_BLACK);


  AllEntitiesChar[0] = {codeEntities::breakableWall, "%", 1};
  AllEntitiesChar[1] = {codeEntities::solidWall, " ", 2};
  AllEntitiesChar[2] = {codeEntities::player, "@", 3};
  AllEntitiesChar[3] = {codeEntities::enemy1, "M", 4};
  AllEntitiesChar[4] = {codeEntities::enemy2, "W", 4};
  AllEntitiesChar[5] = {codeEntities::bombAnimation1, "0", 5};
  AllEntitiesChar[6] = {codeEntities::bombAnimation2, "O", 5};
  AllEntitiesChar[7] = {codeEntities::bonus1, "$", 6};
  AllEntitiesChar[8] = {codeEntities::bonus2, "+", 3};
  AllEntitiesChar[9] = {codeEntities::explosion, "*", 6};
  AllEntitiesChar[10] = {codeEntities::lives, "V", 5};
  AllEntitiesChar[11] = {codeEntities::bulletLeft, "<", 5};
  AllEntitiesChar[12] = {codeEntities::bulletRight, ">", 5};
  AllEntitiesChar[13] = {codeEntities::bulletUp, "^", 5};
  AllEntitiesChar[14] = {codeEntities::bulletDown, "v", 5};
  
  const int requiredRows = initialY + gridHeight + 1;
  const int requiredCols = initialX + gridWidth + 25;
  if(LINES < requiredRows || COLS < requiredCols){
    terminalTooSmall = true;
  }
  win = newwin(gridHeight + 1, gridWidth + 25, initialY, initialX);
  keypad(win, TRUE); // Abilita tasti speciali (F1, frecce, ecc.)
  nodelay(win, TRUE);// getch ritorna -1 se non c'è input
}

//Disegna il carattere corrispondente all'entity c, con il colore corretto
void Game::printEntity(codeEntities c){
  int color = getEntityColor(c);
  if(color == 1) wattron(win, COLOR_PAIR(color) | A_BOLD);
  else wattron(win, COLOR_PAIR(color));
  wprintw(win, "%s", getEntityChar(c));
  if(color == 1) wattroff(win, COLOR_PAIR(color) | A_BOLD);
  else wattroff(win, COLOR_PAIR(color));
}

const char* Game::getEntityChar(codeEntities c){
  for(int i = 0; i < CHARS_TO_DISPLAY; i++){
    if(AllEntitiesChar[i].codeEntity == c) return AllEntitiesChar[i].ch;
  }
  return "";
}

int Game::getEntityColor(codeEntities c){
  for(int i = 0; i < CHARS_TO_DISPLAY; i++){
    if(AllEntitiesChar[i].codeEntity == c) return AllEntitiesChar[i].colorPair;
  }
  return -1;
}

//Chiude il gioco, salvando la classifica
void Game::quitGame(){
  endwin();
  saveClassifica("config/scores.txt");
}

//Verifica se i livelli sono stati caricati correttamente
bool Game::isLoaded(){
  if(current_lvl == nullptr)return false;
  else return true;
}

//Verifica input da tastiera
int Game::checkInput(){
  return wgetch(win);
}

//Carica i livelli da file
bool Game::loadLevels(const char* file){
  current_lvl = nullptr;
  fstream inputFile;
  inputFile.open(file);
  if(!inputFile.is_open()){
    char fallback[100] = "../";
    inputFile.open(strcat(fallback, file));
  }
  char ch;
  while(inputFile.get(ch)){
    Map map = new char*[sizeGridY];
    for (int i = 0; i < sizeGridY; i++)
      map[i] = new char[sizeGridX];
    int row = 0;
    int col = 0;
    map[row][col] = ch;
    col++;
    while (inputFile.get(ch) && row < sizeGridY){
      if(ch == '\r')continue;
      try{
        if((ch >= 48 + 5 || ch < 48) && ch != '\n' && ch != ' ')throw ch;
        if (ch == '\n' || col >= sizeGridX){
          col = 0;
          row++;
          if (row >= sizeGridY) break;
          else continue;
        }
        map[row][col] = ch;
        col++;
      }catch(char Err){
        break;
      }
    }
    if(row == sizeGridY && col == 0){
      if(current_lvl == nullptr){
        current_lvl = new Lvl(map);
      } else {
        current_lvl->addLvl(new Lvl(map));
        current_lvl = current_lvl->getNextLvl();
      }
    } else {
      return false;
    }
  }
  inputFile.close();
  while(current_lvl != nullptr && !current_lvl->isFirst()){
    current_lvl = current_lvl->getPrecLvl();
  }
  return true;
}

//Carica la classifica da file
void Game::loadClassifica(const char* file){
  fstream inputFile;
  inputFile.open(file);
  if(!inputFile.is_open()){
    char fallback[100] = "../";
    inputFile.open(strcat(fallback, file));
  }
  char ch;
  int stage = 0;
  char nome[8] = {};
  char punteggio[8] = {};
  while(inputFile.get(ch)){
    if(ch == '\r' || ch == ' ')continue;
    if(ch == ','){
      stage++;
      continue;
    }
    if(ch == '\n'){
      stage++;
      if(stage == 2){
        int p = 0;
        int ind = 0;
        while(ind < strlen(punteggio)){
          p = p * 10 + (punteggio[ind] - '0');
          ind++;
        }
        classifica.addRecord(nome, p);
        stage = 0;
        nome[0] = '\0';
        punteggio[0] = '\0';
      }
      continue;
    }
    if(stage == 0){
      int length = strlen(nome);
      if(isalnum(ch)){
        nome[length] = ch;
        nome[length + 1] = '\0';
      }
    } else if(stage == 1){
      int length = strlen(punteggio);
      if(isdigit(ch)){
        punteggio[length] = ch;
        punteggio[length + 1] = '\0';
      }
      else {
        while(inputFile.get(ch) && ch != '\n'){}
        stage = 0;
        nome[0] = '\0';
        punteggio[0] = '\0';
      }
    }
  }
  inputFile.close();
}

//Salva la classifica su file
void Game::saveClassifica(const char* file){
  ofstream outputFile;
  outputFile.open(file);
  record* r = classifica.getRecords();
  while(r != nullptr){
    outputFile << r->nome << ", " << r->punteggio << endl;
    r = r->next;
  }
  outputFile.close();
}

//Disegna il livello corrente, con tutti i suoi elementi
void Game::drawLevel(){
  Map m = current_lvl->getMap();
  bool drawedPlayer = false;
  currentEnemyCount = 0;

  //Disegna eventuali vie laterali
  if(!current_lvl->isFirst()){
    if(m[10][0] == '1'){
      m[10][0] = ' ';
      current_lvl->updateMap(m);
    }
    drawBackGateway(true);
  } else {
    m[10][0] = '1';
    current_lvl->updateMap(m);
    drawBackGateway(false);
  }
  drawNextGateway(isLevelCompleted());

  //Disegna tutto il livello
  for(int i = 0; i < sizeGridY; i++){
    for(int j = 0; j < sizeGridX; j++){
      wmove(win, i, j);
      if(m[i][j] == ' ') wprintw(win, " ");
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
            pl.updatePosition(i, j);
            drawedPlayer = true;
            break;
          case codeEntities::enemy1:
            //init_enemy1
            if(currentEnemyCount < MAX_ENEMIES){
              printEntity(codeEntities::enemy1);
              enemies[currentEnemyCount] = new Enemy1(i, j);
              currentEnemyCount++;
            }
            break;
          case codeEntities::enemy2:
            if(currentEnemyCount < MAX_ENEMIES) {
            
              printEntity(codeEntities::enemy2);
            
              Enemy2* enemy = new Enemy2(i, j);
            
              enemy->initBullets(sizeGridX, sizeGridY);
            
              enemies[currentEnemyCount] = enemy;
            
              currentEnemyCount++;
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
    wmove(win, pl.getYLoc(), pl.getXLoc());
    printEntity(codeEntities::player);
  }
  
  //Disegna le vite
  updateLives();

  //Disegna il numero del livello
  const char* levelStr = "Level ";
  mvprintw(start_y-1, start_x+32, "%s", levelStr);
  mvprintw(start_y-1, start_x+38, "%d", current_lvl->getLvlNumber());

  // Stampa tempo iniziale
  updateTime();

  // Stampa score
  updateScore();

  refresh();
}

//Mostra il menù iniziale
int Game::showMenu(){
  if(terminalTooSmall){
    clear();
    mvprintw(0, 0, "Terminal too small!");
    mvprintw(1, 0, "Minimum display: 75 x 26.");
    mvprintw(2, 0, "Current size: %d x %d.", COLS, LINES);
    refresh();
    while(checkInput() == -1){}
    return 2;
  }

  int ch;
  int choise = 0;
  int coord[3][2] = {{(sizeGridX/3)+1,(sizeGridY/4)},
                     {(sizeGridX/3)-3,(sizeGridY/4)+2},
                     {(sizeGridX/3)+1,(sizeGridY/4)+4}};

  wclear(win);
  mvprintw(start_y+9, start_x-1, " ");
  mvprintw(start_y+11, start_x-1, " ");
  move(start_y-1, start_x);
  for(int i = 0; i < sizeGridX; i++) printw(" ");
  mvprintw(start_y-1, start_x + (sizeGridX/2)-4, "Bomberman");
  refresh();
  mvwprintw(win, coord[0][1], coord[0][0]-3, ">  Nuova partita");
  mvwprintw(win, coord[1][1], coord[1][0]  , "Visualizza classifica");
  mvwprintw(win, coord[2][1], coord[2][0]  , "Esci dal gioco");
  while((ch = checkInput()) != '\n' && ch != '\r' && ch != KEY_ENTER){
    if(ch == KEY_UP){
      mvwprintw(win, coord[choise][1], coord[choise][0]-3, " ");
      if(choise == 0) choise = 2;
      else choise--;
    } else if(ch == KEY_DOWN){
      mvwprintw(win, coord[choise][1], coord[choise][0]-3, " ");
      if(choise == 2) choise = 0;
      else choise++;
    } else continue;
    mvwprintw(win, coord[choise][1], coord[choise][0]-3, ">");
  }
  return choise;
}

//Mostra la classifica, con possibilità di scorrere le pagine
void Game::showClassifica(int page, int rec_per_page){
  wclear(win);
  record* r = classifica.getRecords();
  if(r == nullptr && page == 0) mvwprintw(win, sizeGridY/4, (sizeGridX/4)+1, "Ancora nessun record");
  int pos = page*rec_per_page;
  for(int i = 0; i < pos; i++) r = r->next;
  do{
    char* rec = new char[20];
    strcpy(rec, r->nome);
    strcat(rec, "\t- ");
    if(pos < 3){
      mvwprintw(win, (((pos%5)+1)*2), (sizeGridX/3), "%d", (pos%5)+1);
      wprintw(win, "%s", ". ");
      wprintw(win, "%s", rec);
    } else mvwprintw(win, (((pos%5)+1)*2), (sizeGridX/3)+3, "%s", rec);
    wprintw(win, "%d", r->punteggio);
    pos++;
    r = r->next;
    if(r == nullptr) break;
  }while(pos%5 != 0);
  char* bar = new char[20];
  if(page != 0) strcpy(bar, "< ");
  else strcpy(bar, "  ");
  strcat(bar, "Page ");
  mvwprintw(win, sizeGridY - 2, (sizeGridX/2)-(strlen(bar)/2)-2, "%s", bar);
  wprintw(win, "%d", (page+1));
  if(r != nullptr) wprintw(win, " >");
  int ch;
  while((ch = checkInput()) != '\n' && ch != '\r' && ch != KEY_ENTER){
    if(ch == KEY_LEFT){
      if(page != 0){
        showClassifica(page-1);
        break;
      }
    } else if (ch == KEY_RIGHT){
      if(r != nullptr){
        showClassifica(page+1);
        break;
      }
    }
  }
}

//Vai al livello successivo
bool Game::goToNextLevel(){
  if(current_lvl->isLast()) return false;
  else current_lvl = current_lvl->getNextLvl();
  return true;
}

//Vai al livello precedente
bool Game::goToPrecLevel(){
  if(current_lvl->isFirst()) return false;
  else current_lvl = current_lvl->getPrecLvl();
  return true;
}

//Funzione principale del gioco, gestisce input, movimento e logica di gioco
void Game::runTime(){
  int ch;
  while(1){
    ch = checkInput();
    if(ch == 'q') break;
    //Gestione del movimento del giocatore
    else if(ch != -1){
      int x = pl.getXLoc();
      int y = pl.getYLoc();

      if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd') {
        int nextX = x, nextY = y;
        if (ch == 'w') nextY--;
        else if (ch == 's') nextY++;
        else if (ch == 'a') nextX--;
        else if (ch == 'd') nextX++;
        
        // Controlla se il giocatore sta per raccogliere un item ('5' = bonus1, 6 = bonus2)
        if (nextY >= 0 && nextY < sizeGridY && nextX >= 0 && nextX < sizeGridX) {
          if (current_lvl->getMap()[nextY][nextX] == '5') {
            itemHandler.collect();
          } else if (current_lvl->getMap()[nextY][nextX] == '6') {
            if(pl.getLives() < 4){
              pl.liveschange(1);
              updateLives();
            }
          }
        }
        
        //Vai al prossimo livello se il giocatore ha raggiunto le coordinate (40, 10) o (-1, 10)
        if((nextX == 40 || nextX == -1) && nextY == 10){
          bool isCleared = isLevelCleared();
          if(nextX == 40){
            if(goToNextLevel()){
              if(isCleared){
                current_lvl->deleteLvl(false);
              }
              if(!current_lvl->getTimerInitialize()){
                current_lvl->setTimerInitialize(true);
                timeHandler.start(200 + timeHandler.getRemainingTime()); // Riavvia timer
              }
              pl.resetPosition();
              drawLevel();
            } else {
              score += timeHandler.getRemainingTime(); // Bonus tempo
              GameOver("YOU WON!");
              break;
            }
          } else if(nextX == -1){
            if(goToPrecLevel()){
              if(isCleared){
                current_lvl->deleteLvl(true);
              }
              pl.resetPosition();
              drawLevel();
            }
          }
        } else {
          bool collision = false;
          Map update = current_lvl->getMap();
          if(update[nextY][nextX] == '4' || update[nextY][nextX] == '3' || update[nextY][nextX] == 'A' || update[nextY][nextX] == 'B' || update[nextY][nextX] == 'C' || update[nextY][nextX] == 'D'){
            pl.liveschange(0);
            pl.resetPosition();
            collision = true;
            updateLives();

            // Se il giocatore viene colpito da un proiettile
            if(update[nextY][nextX] == 'A' || update[nextY][nextX] == 'B' || update[nextY][nextX] == 'C' || update[nextY][nextX] == 'D'){
              update[nextY][nextX] = ' ';
              wmove(win, nextY, nextX);
              wprintw(win, " ");
            }
          }
          if(!collision){
            update = pl.getmv(ch, current_lvl->getMap());
          } else {
            update[y][x] = ' ';
            update[pl.getYLoc()][pl.getXLoc()] = '2';
          }
          if(update != nullptr){
            mvwprintw(win, y, x, " ");
            wmove(win, pl.getYLoc(), pl.getXLoc());
            printEntity(codeEntities::player);
            current_lvl->updateMap(update);
          }
        }
      }

      //Piazza la bomba
      if(ch == ' '){
        bombHandler.placeBomb(pl.getXLoc(), pl.getYLoc(), itemHandler.getBombRadius(), 200);
      }
    }
    
    // Aggiorna item
    itemHandler.update();

    // Aggiorna bomba e gestisci esplosioni
    if(bombHandler.update(current_lvl->getMap())) {

      ExplodedCell* cells = bombHandler.getExplodedCells();
      int size = bombHandler.getExplosionSize();
      
      //La bomba è esplosa
      if(bombHandler.isBombActive()){

        Map update = current_lvl->getMap();

        for (int i = 0; i < size; i++) {
        
          // Controlla se il giocatore è stato colpito
          if (cells[i].x == pl.getXLoc() && cells[i].y == pl.getYLoc()) {
            pl.liveschange(0);
            update[pl.getYLoc()][pl.getXLoc()] = ' ';
            pl.resetPosition();
            update[pl.getYLoc()][pl.getXLoc()] = '2';
            updateLives();
          }
        
          // Controlla se un nemico è stato colpito
          for(int j = 0; j < currentEnemyCount; j++){
            if(cells[i].x == enemies[j]->getXLoc() && cells[i].y == enemies[j]->getYLoc()){
              Enemy2* enemy2 = dynamic_cast<Enemy2*>(enemies[j]);
              if(enemy2 != nullptr){
                for(int i = 0;i < BU; i++){
                  bullet* b = enemy2->getBullet(i);
                  if(b != nullptr &&  b->active){
                    update[b->yLoc][b->xLoc] = ' ';
                    mvwprintw(win, b->yLoc, b->xLoc, " ");
                  } else break;
                }
              }
              delete enemies[j];
              for(int k = j; k < currentEnemyCount-1; k++){
                enemies[k] = enemies[k+1];
              }
              currentEnemyCount--;

              update[cells[i].y][cells[i].x] = ' ';
              score += 75;
              updateScore();
              if(currentEnemyCount == 0){
                update[10][39] = ' '; // Rimuovi il muro che blocca l'uscita
                drawNextGateway(true);
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
            score += 10;
            updateScore();
          }

          //Controlla se la cella colpita è un proiettile nemico
          if(update[cells[i].y][cells[i].x] == 'A' || update[cells[i].y][cells[i].x] == 'B' || update[cells[i].y][cells[i].x] == 'C' || update[cells[i].y][cells[i].x] == 'D'){
            update[cells[i].y][cells[i].x] = ' ';
          }

          // Disegna esplosione a schermo
          wmove(win, cells[i].y, cells[i].x);
          printEntity(codeEntities::explosion);
        }

        current_lvl->updateMap(update);
        wrefresh(win);
      }
      
      // La bomba ha finito il tempo di animazione dell'esplosione
      else {

        const Map lvl = current_lvl->getMap();

        // Pulisci celle dell'esplosione
        for (int i = 0; i < size; i++) {
          wmove(win, cells[i].y, cells[i].x);
          switch(lvl[cells[i].y][cells[i].x]){
            case codeEntities::bonus1+48:
              printEntity(codeEntities::bonus1);
              break;
            case codeEntities::bonus2+48:
              printEntity(codeEntities::bonus2);
              break;
            case ' ':
              wprintw(win, " ");
              break;
            default: break;
          }
        }
        
        if(!pl.isdead()){
          wmove(win, pl.getYLoc(), pl.getXLoc());
          printEntity(codeEntities::player);
        }
        wrefresh(win);
        bombHandler.clearExplosion();
      }
      
    }

    // Disegna bomba se attiva
    bomb b = bombHandler.getBomb();
    if (b.isActive && !bombHandler.hasExploded()) {
      wmove(win, b.y, b.x);
      if (b.timer % 50 < 25) {
        printEntity(codeEntities::bombAnimation1);
      } else {
        printEntity(codeEntities::bombAnimation2);
      }
    }

    //Gestione del movimento dei nemici e dei proiettili.
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    Map map = current_lvl->getMap();
    for(int i = 0; i < currentEnemyCount; i++) {

      Enemy2* enemy2 = dynamic_cast<Enemy2*>(enemies[i]);

      //Se il nemico è un enemy2
      if(enemy2 != nullptr) {
        bullet* newBullet = enemy2->firebullet(pl.getXLoc(), pl.getYLoc(), map);

        //Disegna il nuovo proiettile
        if(newBullet != nullptr) {
          wmove(win, newBullet->yLoc, newBullet->xLoc);
          switch(newBullet->direction) {
            case 0: // Up
              map[newBullet->yLoc][newBullet->xLoc] = 'A';
              printEntity(codeEntities::bulletUp);
              break;
            case 1: // Down
              map[newBullet->yLoc][newBullet->xLoc] = 'B';
              printEntity(codeEntities::bulletDown);
              break;
            case 2: // Left
              map[newBullet->yLoc][newBullet->xLoc] = 'C';
              printEntity(codeEntities::bulletLeft);
              break;
            case 3: // Right
              map[newBullet->yLoc][newBullet->xLoc] = 'D';
              printEntity(codeEntities::bulletRight);
              break;
          }
          current_lvl->updateMap(map);
        }

        //Aggiorna proiettili
        int index = 0;
        for(int i = 0; i < BU; i++){

          bullet* currentBullet = enemy2->getBullet(i);
          if(currentBullet != nullptr && currentBullet->active) {
            const std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
              now - currentBullet->lastMoveTime);
            if(elapsed.count() >= BULLET_SPEED) {

              // Cancella il proiettile nella vecchia posizione
              if(map[currentBullet->yLoc][currentBullet->xLoc] == ' '){
                currentBullet->deactivate();
                continue;
              } else {
                map[currentBullet->yLoc][currentBullet->xLoc] = ' ';
                wmove(win, currentBullet->yLoc, currentBullet->xLoc);
                wprintw(win, " ");
              }

              if(!currentBullet->move(map)){

                //Colpito il giocatore
                if(currentBullet->xLoc == pl.getXLoc() && currentBullet->yLoc == pl.getYLoc()) {
                  pl.liveschange(0);
                  map[pl.getYLoc()][pl.getXLoc()] = ' ';
                  wmove(win, pl.getYLoc(), pl.getXLoc());
                  wprintw(win, " ");
                  pl.resetPosition();
                  updateLives();
                  map[pl.getYLoc()][pl.getXLoc()] = '2';

                  // Disegna il giocatore alla nuova posizione
                  wmove(win, pl.getYLoc(), pl.getXLoc());
                  printEntity(codeEntities::player);
                }
              } else {

                // Disegna il proiettile alla nuova posizione
                wmove(win, currentBullet->yLoc, currentBullet->xLoc);
                switch(currentBullet->direction) {
                  case 0: // Up
                    map[currentBullet->yLoc][currentBullet->xLoc] = 'A';
                    printEntity(codeEntities::bulletUp);
                    break;
                  case 1: // Down
                    map[currentBullet->yLoc][currentBullet->xLoc] = 'B';
                    printEntity(codeEntities::bulletDown);
                    break;
                  case 2: // Left
                    map[currentBullet->yLoc][currentBullet->xLoc] = 'C';
                    printEntity(codeEntities::bulletLeft);
                    break;
                  case 3: // Right
                    map[currentBullet->yLoc][currentBullet->xLoc] = 'D';
                    printEntity(codeEntities::bulletRight);
                    break;
                }
              }
            }
          }
        }
        current_lvl->updateMap(map);
      }
      
      //Muovi i nemici
        const std::chrono::milliseconds enemyElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
          now - lastEnemyMoveTime);
        if(enemyElapsed.count() >= ENEMY_SPEED) {
        int oldX = enemies[i]->getXLoc();
        int oldY = enemies[i]->getYLoc();
        
        int direction = enemies[i]->choosePath();
        
        int newX = oldX;
        int newY = oldY;
    
        switch(direction) {
          case 0:
            newY--;
            break;
          case 1:
            newY++;
            break;
          case 2:
            newX--;
            break;
          case 3:
            newX++;
            break;
        }
        
        // Controlla i limiti della mappa
        if(newY >= 0 && newY < sizeGridY && newX >= 0 && newX < sizeGridX) {
          //Controlla se il nemico può muoversi nella nuova posizione
          if(map[newY][newX] == ' ') {
            // Aggiorna map
            map[oldY][oldX] = ' ';
            if(enemy2 != nullptr) map[newY][newX] = '4';
            else map[newY][newX] = '3';
          
            enemies[i]->UpdateLocation(direction);
            // Cancella il nemico alla vecchia posizione
            wmove(win, oldY, oldX);
            wprintw(win, " ");
            // Disegna il nemico alla nuova posizione
            wmove(win, newY, newX);
            if(enemy2 != nullptr) printEntity(codeEntities::enemy2);
            else printEntity(codeEntities::enemy1);

          }
          // Controlla se il nemico ha colpito il giocatore
          else if(map[newY][newX] == '2') {
            pl.liveschange(0);
            updateLives();
            pl.resetPosition();
            // aggiorna map
            map[pl.getYLoc()][pl.getXLoc()] = '2';
            map[oldY][oldX] = ' ';
            if(enemy2 != nullptr) map[newY][newX] = '4';
            else map[newY][newX] = '3';
            // Cancella il nemico alla vecchia posizione
            wmove(win, oldY, oldX);
            wprintw(win, " ");
            // Disegna il giocatore alla nuova posizione
            wmove(win, pl.getYLoc(), pl.getXLoc());
            printEntity(codeEntities::player);
            // Disegna il nemico alla nuova posizione
            wmove(win, newY, newX);
            if(enemy2 != nullptr) printEntity(codeEntities::enemy2);
            else printEntity(codeEntities::enemy1);
            enemies[i]->UpdateLocation(direction);
          }
        }
        
        current_lvl->updateMap(map);
      }
    }
    const auto enemyElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
      now - lastEnemyMoveTime);
    if(enemyElapsed.count() >= ENEMY_SPEED) lastEnemyMoveTime = now;

    // Game over: il giocatore non ha più vite
    if (pl.isdead()){
      GameOver("GAME OVER!");
      break;
    }
    
    // Game over: tempo scaduto
    if (timeHandler.isTimeUp()) {
      GameOver("TIME OUT!");
      break;
    }

    // Aggiorna a schermo il tempo rimasto
    updateTime();

    wrefresh(win);
    napms(10); // ~100 FPS
  }
}

//Gestisce la schermata di fine partita
void Game::GameOver(const char* msg) {
  const int sidePanelX = sizeGridX + 3;

  mvwprintw(win, 5, sidePanelX+3, "%s", msg);
  mvwprintw(win, 7, sidePanelX+4, "You got");
  mvwprintw(win, 9, sidePanelX+3, "%d points", score);
  mvwprintw(win, 11, sidePanelX, "Insert your name:");
  wmove(win, 12, sidePanelX + 4);
  wrefresh(win);

  char name[8] = {};
  nodelay(win, FALSE);
  echo();
  curs_set(1);
  wmove(win, 12, sidePanelX + 4);
  wgetnstr(win, name, sizeof(name) - 1);
  noecho();
  curs_set(0);
  nodelay(win, TRUE);

  if (strlen(name) == 0) strcpy(name, "Anonimo");
  classifica.addRecord(name, score);
  mvwprintw(win, 14, sidePanelX, "Salvato in classifica");
  wrefresh(win);
}

//Inizializza una nuova partita
void Game::startNewGame() {
  loadLevels("config/levels.txt");
  if(!isLoaded()) return;
  pl = Player();
  score = 0;
  current_lvl->setTimerInitialize(true);
  timeHandler.start(200); // Inizia timer (200 secondi a livello)
  lastEnemyMoveTime = std::chrono::steady_clock::now();
  drawLevel();
  runTime();
}

// Controlla se il livello è stato completato (tutti i nemici eliminati)
bool Game::isLevelCompleted(){
  Map m = current_lvl->getMap();
  for(int i = 0; i < sizeGridY; i++){
    for(int j = 0; j < sizeGridX; j++){
      if(m[i][j]-48 == codeEntities::enemy1 || m[i][j]-48 == codeEntities::enemy2){
        return false;
      }
    }
  }
  return true;
}

// Controlla se il livello è stato pulito completamente
bool Game::isLevelCleared(){
  Map m = current_lvl->getMap();
  for(int i = 0; i < sizeGridY; i++){
    for(int j = 0; j < sizeGridX; j++){
      if(m[i][j]-48 ==  codeEntities::breakableWall || m[i][j]-48 == codeEntities::enemy1 || m[i][j]-48 == codeEntities::enemy2){
        return false;
      }
    }
  }
  return true;
}

//Aggiorna le vite a schermo
void Game::updateLives(){
  wmove(win, sizeGridY, sizeGridX/2-2);
  wprintw(win, "    ");
  wmove(win, sizeGridY, sizeGridX/2-2);
  for (int i = 0; i < pl.getLives(); i++) {
    printEntity(codeEntities::lives);
  }
}

//Disegna o cancella la porta di destra
void Game::drawNextGateway(bool open){
  if(open){
    wmove(win, 9, 40);
    printEntity(codeEntities::solidWall);
    wmove(win, 10, 39);
    wprintw(win, " ");
    wmove(win, 11, 40);
    printEntity(codeEntities::solidWall); // Disegna l'uscita
  } else {
    mvprintw(start_y+9, start_x+40, "%s", " ");
    mvprintw(start_y+11, start_x+40, "%s", " ");
    wmove(win, 10, 39);
    printEntity(codeEntities::solidWall); //Cancella l'uscita
  }
}

//Disegna o cancella la porta di sinistra
void Game::drawBackGateway(bool open){
  if(open){
    wmove(win, 10, 0);
    wprintw(win, "%s", " ");
    attron(COLOR_PAIR(getEntityColor(codeEntities::solidWall)));
    mvprintw(start_y+9, start_x-1, "%s", getEntityChar(codeEntities::solidWall));
    mvprintw(start_y+11, start_x-1, "%s", getEntityChar(codeEntities::solidWall));
    attroff(COLOR_PAIR(getEntityColor(codeEntities::solidWall))); //Disegna l'uscita
  } else {
    wmove(win, start_y+10, start_x);
    printEntity(codeEntities::solidWall);
    mvprintw(start_y+9, start_x-1, " ");
    mvprintw(start_y+11, start_x-1, " "); //Cancella l'uscita
  }
}

//Aggiorna il tempo a schermo
void Game::updateTime(){
  wmove(win, sizeGridY, sizeGridX - 10);
  wprintw(win, "%s", "Time: ");
  wprintw(win, "%d", timeHandler.getRemainingTime());
  wprintw(win, "%s", "  ");
}

//Aggiorna il punteggio a schermo
void Game::updateScore(){
  wmove(win, sizeGridY, 1);
  wprintw(win, "%s", "Score: ");
  wprintw(win, "%d", score);
  wprintw(win, "%s", "  ");
}