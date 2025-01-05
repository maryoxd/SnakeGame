#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "gamestate.h" // Zabezpečíme správny include GameState

// Funkcia na vykreslenie herného stavu
void draw_game(const GameState *state) {
    system("clear");
    printf("Skore: %d\n", state->score);
    printf("Cas: %d sekund\n", state->elapsed_time);
    for (int i = 0; i < state->world.height; i++) {
        for (int j = 0; j < state->world.width; j++) {
            putchar(state->world.grid[i][j]);
        }
        putchar('\n');
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použitie: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in serv_addr;
    char input;
    GameState game_state;

    // Vytvorenie socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Chyba pri vytváraní socketu");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Pripojenie k localhost
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Chyba pri nastavovaní IP adresy");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Chyba pri pripojení na server");
        return 1;
    }

    printf("Pripojené na server\n");

    // Hlavná herná slučka
    while (1) {
        
        printf("Cakam na udaje od servera..\n");
        if (read(sockfd, &game_state, sizeof(game_state)) <= 0) {
            printf("Spojenie so serverom bolo ukončené\n");
            break;
        }
        printf("Herny stav prijaty.\n");

        // Vykreslenie herného stavu
        draw_game(&game_state);

        // Kontrola, či hra skončila
        if (gamestate_is_game_over(&game_state)) {
            printf("Hra skončila! Skóre: %d\n", game_state.score);
            break;
        }

        // Posielanie vstupu serveru
        printf("Napíšte smer pohybu (w/a/s/d): ");
        input = getchar();
        getchar(); // Odstránenie '\n' zo vstupu

        if (write(sockfd, &input, sizeof(input)) <= 0) {
            perror("Chyba pri posielaní vstupu serveru");
            break;
        }
    }

    close(sockfd);
    return 0;
}
