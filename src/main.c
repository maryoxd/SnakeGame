#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "world.h"
#include "fruit.h"
#include "snake.h"
#include "common.h"
#include "input.h"
#include <time.h>

// Globálne premenné
int game_over = false;
int score = 0;
int elapsed_time = 0;  // Čas hry v sekundách
pthread_mutex_t lock;  // Mutex na synchronizáciu

void *timer_thread(void *arg) {
    while (!game_over) {
        sleep(1);  // Počká jednu sekundu
        pthread_mutex_lock(&lock);
        elapsed_time++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t timer_tid;  // ID vlákna pre časomieru
    pthread_mutex_init(&lock, NULL);

    // Herné premenné
    World world;
    FruitManager fruit_manager;
    Snake snake;
    Fruit current_fruit;
    bool ate_fruit = false;
    int key = 2;

    world_init(&world);
    fruit_manager_init(&fruit_manager);

    if (world_load(&world, "hernysvet.txt") != 0) return -1;
    if (fruit_manager_load(&fruit_manager, "ovocie.txt") != 0) return -1;

    snake_init(&snake, 15, 15);
    if (fruit_next(&fruit_manager, &current_fruit) == 0) {
        world.grid[current_fruit.y][current_fruit.x] = FRUIT;
    }

    enable_raw_mode();

    // Spustenie vlákna časomiery
    pthread_create(&timer_tid, NULL, timer_thread, NULL);

    while (!game_over) {
        system("clear");

        // Výpis skóre a času
        pthread_mutex_lock(&lock);
        printf("Skore: %d\n", score);
        printf("Cas: %d sekund\n", elapsed_time);
        pthread_mutex_unlock(&lock);

        world_draw(&world);
        input(&key, &game_over);

        if (snake_move(&snake, &world, &ate_fruit, key) != 0 || snake_check_collision(&snake)) {
            game_over = true;
            break;
        }

        if (ate_fruit) {
            pthread_mutex_lock(&lock);
            score++;
            pthread_mutex_unlock(&lock);

            if (fruit_next(&fruit_manager, &current_fruit) == 0) {
                world.grid[current_fruit.y][current_fruit.x] = FRUIT;
            }
        }

        sleep_microseconds(100000);
    }

    // Ukončenie časomiery
    pthread_join(timer_tid, NULL);
    disable_raw_mode();
    pthread_mutex_destroy(&lock);

    printf("Game over! Skore: %d\n", score);
    return 0;
}

