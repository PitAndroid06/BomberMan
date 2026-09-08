 BOMBERMAN
 =========

Videogioco da terminale ispirato a Bomberman, scritto in C++ e basato sulla
libreria ncurses. Il giocatore deve eliminare i nemici, raccogliere i bonus e
superare i livelli prima che scada il tempo.

Il programma richiede un terminale di almeno 75 colonne e 26 righe.


 REQUISITI
 ---------

* Compilatore C++ con supporto almeno a C++11.
* Libreria ncurses:
	* Linux: pacchetto di sviluppo ncurses.
	* Windows: MSYS2 con ambiente UCRT64 e ncurses o MinGW con ncurses funzionante.
* Un terminale compatibile con i caratteri e i colori ncurses.


 COMPILAZIONE SU LINUX
 ---------------------

Installare il compilatore e ncurses. Su Debian, Ubuntu e distribuzioni
compatibili:

		sudo apt update
		sudo apt install g++ libncurses-dev

Dalla cartella principale del progetto, compilare ed eseguire con lo script
incluso:

		./compile

In alternativa, compilare soltanto:

		g++ *.cpp -lncursesw -o bin/Bomberman

e avviare il gioco dalla cartella principale:

		./bin/Bomberman


 COMPILAZIONE SU WINDOWS (con MSYS2 con ambiente UCRT64 e ncurses)
 -----------------------

Eseguire `compileWindows.bat` da Esplora file oppure dal prompt dei
comandi Windows:

			 compileWindows.bat

Lo script usa il percorso predefinito `C:\msys64\ucrt64`. Se MSYS2 e stato
installato in un percorso diverso, modificare il percorso di `g++.exe`, delle
directory `include` e `lib` nello script prima di eseguirlo.

Il file eseguibile viene creato in `bin/Bomberman.exe`. Per avviare il gioco fare doppio clic sull'eseguibile dentro la cartella bin oppure dal prompt dei comandi, dalla cartella principale del progetto, usare:

		bin\Bomberman.exe


 COMANDI
 -------

 MENU

* Freccia su / freccia giu: seleziona una voce.
* Invio: conferma la voce selezionata.
* Freccia sinistra / freccia destra: cambia pagina nella classifica.

 PARTITA

* `W`: muovi verso l'alto.
* `A`: muovi verso sinistra.
* `S`: muovi verso il basso.
* `D`: muovi verso destra.
* Barra spaziatrice: piazza una bomba.
* `Q`: termina la partita corrente.