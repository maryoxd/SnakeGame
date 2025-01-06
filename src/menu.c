#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "gamestate.h"

void handle_menu_server(int client_sock, GameState *game_state) {
    int choice;

    while (1) {
        if (read(client_sock, &choice, sizeof(choice)) <= 0) {
            printf("Klient sa odpojil.\n");
            close(client_sock);
            return;
        }
        printf("Prijatá voľba od klienta: %d\n", choice);

        switch (choice) {
            case 1:
                printf("Klient si zvolil novú hru.\n");
                gamestate_init(game_state, "hernysvet.txt", "ovocie.txt");
                return;

            case 2: {
                char scores[1024] = {0};
                FILE *file = fopen("highscores.txt", "r");
                if (file) {
                    fread(scores, sizeof(char), sizeof(scores), file);
                    fclose(file);
                } else {
                    strcpy(scores, "Žiadne skóre k dispozícii.\n");
                }
                if (write(client_sock, scores, sizeof(scores)) <= 0) {
                    printf("Chyba pri posielaní skóre klientovi.\n");
                }
                break;
            }

            case 3: {
                FILE *file = fopen("highscores.txt", "a");
                if (file) {
                    time_t now = time(NULL);
                    fprintf(file, "Skóre: %d - %s", game_state->score, ctime(&now));
                    fclose(file);
                    printf("Skóre bolo zapísané.\n");
                } else {
                    printf("Chyba pri otváraní súboru pre zápis skóre.\n");
                }
                break;
            }

            case 4:
                printf("Klient odchádza.\n");
                close(client_sock);
                exit(0);

            default:
                printf("Neplatná voľba od klienta: %d\n", choice);
                break;
        }
    }
}

void show_menu_client(int sockfd) {
    int choice;
    char buffer[1024] = {0};

    while (1) {
        printf("\nHra skončila!\n");
        printf("1. Začať novú hru\n");
        printf("2. Zobraziť TOP 10 skóre\n");
        printf("3. Zapísať skóre do súboru\n");
        printf("4. Odísť\n");
        printf("Zadajte číslo: ");
        
        if(scanf("%d", &choice) != 1) {
          printf("Neplatna volba, skuste to znova.\n");
          while(getchar() != '\n');
          continue;
        }

        if (write(sockfd, &choice, sizeof(choice)) <= 0) {
            perror("Chyba pri posielaní výberu serveru");
            break;
        }

        if (choice == 1) {
            printf("Spúšťam novú hru...\n");
            return;
        } else if (choice == 2) {
            if (read(sockfd, buffer, sizeof(buffer)) > 0) {
                printf("TOP 10 skóre:\n%s\n", buffer);
            } else {
                printf("Chyba pri čítaní skóre od serveru.\n");
            }
        } else if (choice == 3) {
            printf("Skóre zapísané.\n");
        } else if (choice == 4) {
            printf("Odchádzam...\n");
            exit(0);
        } else {
            printf("Neplatná voľba, skúste znova.\n");
        }
    }
}

