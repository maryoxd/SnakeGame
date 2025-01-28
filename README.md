# SnakeGame
## [SK] Popis projektu
Tento projekt je implementáciou klasickej arkádovej hry Hadík v programovacom jazyku C, ktorá bola vytvorená ako tímová semestrálna práca. Hra podporuje režim pre jedného alebo viacerých hráčov, pričom je realizovaná na báze klient-server architektúry. Server spravuje herný svet a komunikuje s klientmi, ktorí ovládajú svojich hadov a interagujú so svetom v reálnom čase.

### Štruktúra projektu
Server (server_t.c):

Zodpovedá za inicializáciu herného sveta, správu pohybu hadov, generovanie ovocia, detekciu kolízií a synchronizáciu klientov.
Implementuje viacvláknové spracovanie pomocou pthread pre každého pripojeného klienta.
Komunikácia medzi serverom a klientmi prebieha cez TCP sockety.
Klient (klient_t.c):

Pripája sa k serveru, odosiela vstupy (napr. smer pohybu hada) a prijíma aktuálny stav herného sveta.
Vykresľuje hernú mapu v termináli pomocou knižnice ncurses.
Herný svet (world.c):

Spravuje hernú mapu, pozície hráčov, ovocia a prekážok.
Obsahuje logiku pre rôzne herné režimy a generovanie objektov na mape.
Had (snake.c):

Implementuje správu hada (rast, pohyb, detekciu kolízií).
Obsahuje logiku pre aktualizáciu pozícií tela hada na základe pohybu.
Ovocie (fruit.c):

Generuje ovocie na náhodných pozíciách na mape.
Spravuje jeho zber hráčmi.
Vstupy (input.c):

Spracováva vstupy hráča (WASD na ovládanie pohybu hada).
Obsahuje logiku pre zmenu smeru pohybu hada.
Herné mechaniky
Režimy hry:

Štandardný režim: Hra trvá, kým nie sú všetci hráči mŕtvi.
Časový režim: Hra sa končí po uplynutí stanoveného času.
Typy svetov:

Bez prekážok: Had môže prechádzať cez okraje mapy a objaví sa na opačnej strane.
S prekážkami: Svet obsahuje pevné prekážky, pričom had sa musí vyhnúť zrážke.
Herné objekty:

Had: Rastie po zjedení ovocia. Hra pre neho končí pri kolízii so stenou, prekážkou, vlastným telom alebo iným hadom.
Ovocie: Generuje sa náhodne na mape.
Prekážky: Statické objekty generované podľa veľkosti mapy.
Spustenie hry
Závislosti:

Operačný systém Linux.
GCC kompilátor.
Knižnica ncurses (inštalácia: sudo apt-get install libncurses5-dev libncursesw5-dev).
Kompilácia:

Pre kompiláciu projektu použite príkaz:
bash
Kopírovať
Upraviť
make all
Spustenie hry:

Server:
bash
Kopírovať
Upraviť
./server_t
Klient:
bash
Kopírovať
Upraviť
./klient_t
Ovládanie:

Pohyb hada: Používajte klávesy W (hore), A (vľavo), S (dole), D (vpravo).
Technické výzvy a riešenia
Synchronizácia: Použité mutexy na zabezpečenie konzistencie údajov pri spracovaní vstupov od viacerých klientov.
Medziprocesná komunikácia: Realizovaná pomocou TCP socketov, ktoré zabezpečujú výmenu dát medzi serverom a klientmi.
Flexibilita hry: Podpora rôznych režimov hry a typov svetov.
Možné vylepšenia
Podpora grafického rozhrania pre lepšiu vizualizáciu.
Rozšírenie o špeciálne herné prvky (napr. power-upy, dynamické prekážky).
Optimalizácia prenosu dát medzi serverom a klientmi.
