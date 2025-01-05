#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "gamestate.h"

GameState game_state;

// Funkcia na spracovanie klienta
void handle_client(int client_sock) {
    int direction = 2; // Predvolený smer (2 - dole)
    char input;

    printf("Klient pripojený, začínam spracovanie...\n");
    
    if(write(client_sock, &game_state, sizeof(game_state)) <= 0) {
    perror("Chyba pri posielani dat.\n");
    close(client_sock);
    return;
  }

    while (!gamestate_is_game_over(&game_state)) {
        // Prijatie vstupu od klienta
        if (read(client_sock, &input, sizeof(input)) <= 0) {
            perror("Chyba pri čítaní od klienta");
            break;
        }

        // Nastavenie smeru pohybu podľa vstupu
        if (input == 'w') direction = 0; // Hore
        if (input == 'a') direction = 1; // Doľava
        if (input == 's') direction = 2; // Dole
        if (input == 'd') direction = 3; // Doprava

        // Aktualizácia herného stavu
        gamestate_update(&game_state, direction);

        // Posielanie aktuálneho herného stavu klientovi
        if (write(client_sock, &game_state, sizeof(game_state)) <= 0) {
            perror("Chyba pri posielaní dát klientovi");
            break;
        }
    }

    printf("Klient sa odpojil alebo hra skončila.\n");
    close(client_sock);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použitie: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int sockfd, client_sock;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    // Inicializácia serverového socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Chyba pri vytváraní socketu");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Chyba pri bind() na socket");
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 5) < 0) {
        perror("Chyba pri listen()");
        close(sockfd);
        return 1;
    }

    printf("Server počúva na porte %d...\n", port);

    // Inicializácia herného stavu
    gamestate_init(&game_state, "hernysvet.txt", "ovocie.txt");

    // Prijímanie klientov
    while (!gamestate_is_game_over(&game_state)) {
        client_sock = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (client_sock < 0) {
            perror("Chyba pri accept()");
            continue;
        }

        handle_client(client_sock); // Spracovanie klienta
    }

    printf("Hra skončila! Server sa ukončuje.\n");
    close(sockfd);
    return 0;
}

