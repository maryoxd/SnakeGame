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

// Funkcia na kontrolu, či server beží
int check_server_running(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Chyba pri vytváraní socketu\n");
        return 0;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        close(sockfd);
        return 0;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sockfd);
        return 0;
    }

    close(sockfd);
    return 1;
}

// Funkcia na spustenie servera
void start_server(const char *port) {
    pid_t pid = fork();

    if (pid == 0) {
        // Detský proces -> spustenie servera
        printf("Spúšťam server na porte %s...\n", port);
        execlp("./server", "./server", port, (char *)NULL);
        perror("Chyba pri spúšťaní servera");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("Chyba pri fork()");
        exit(EXIT_FAILURE);
    }

    // Rodič -> čakáme 1 sekundu na naštartovanie servera
    sleep(1);
}

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

        usleep(50000); // Minimalizácia záťaže CPU
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Použitie: %s <port>\n", argv[0]);
        return 1;
    }

    const char *port = argv[1];
    int port_num = atoi(port);

    // Kontrola, či server beží
    if (!check_server_running("127.0.0.1", port_num)) {
        printf("Server nebeží. Automaticky ho spúšťam...\n");
        start_server(port);
    } else {
        printf("Server už beží na porte %s\n", port);
    }

    // Pripojenie k serveru
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Chyba pri vytváraní socketu");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);

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

