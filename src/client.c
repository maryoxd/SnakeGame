#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "gamestate.h"
#include "input.h"

GameState game_state;       // Herný stav
int running = 1;            // Indikátor pre hlavný cyklus klienta
int sockfd;                 // Socket klienta
pthread_mutex_t lock;       // Mutex pre synchronizáciu herného stavu

// Vlákno na čítanie stavu od servera
void *receive_thread(void *arg) {
    while (running) {
        GameState temp_state;

        if (read(sockfd, &temp_state, sizeof(temp_state)) <= 0) {
            printf("Spojenie so serverom bolo ukončené\n");
            running = 0;
            break;
        }

        pthread_mutex_lock(&lock);
        memcpy(&game_state, &temp_state, sizeof(GameState));
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

// Vlákno na odosielanie vstupov serveru
void *input_thread(void *arg) {
    char input_p;

    while (running) {
        if (kbhit()) {
            input_p = getchar();

            // Odosielanie vstupu serveru
            if (write(sockfd, &input_p, sizeof(input_p)) <= 0) {
                perror("Chyba pri posielaní vstupu serveru");
                running = 0;
                break;
            }
        }

        usleep(50000); // Minimalná záťaž na CPU
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použitie: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    struct sockaddr_in serv_addr;

    // Inicializácia socketu
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

    enable_raw_mode();

    // Inicializácia mutexu
    pthread_mutex_init(&lock, NULL);

    // Spustenie vlákien
    pthread_t recv_tid, input_tid;
    pthread_create(&recv_tid, NULL, receive_thread, NULL);
    pthread_create(&input_tid, NULL, input_thread, NULL);

    // Hlavný cyklus vykreslenia
    while (running) {
        pthread_mutex_lock(&lock);
        gamestate_draw(&game_state);
        pthread_mutex_unlock(&lock);

        if (gamestate_is_game_over(&game_state)) {
            printf("Hra skončila! Skóre: %d\n", game_state.score);
            running = 0;
        }

        usleep(100000); // Obnovenie vykreslenia každých 100 ms
    }

    // Čistenie
    pthread_join(recv_tid, NULL);
    pthread_join(input_tid, NULL);
    pthread_mutex_destroy(&lock);

    disable_raw_mode();
    close(sockfd);
    return 0;
}

