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


void cleanup() {
  disable_raw_mode();

}

int main() {
    World world;
    FruitManager fruit_manager;
    Snake snake;
    Fruit current_fruit;
    bool ate_fruit = false;
    int game_over = false;
    int key = 2;
    
    time_t start_time;
  


    atexit(cleanup);

    world_init(&world);
    fruit_manager_init(&fruit_manager);

    if (world_load(&world, "hernysvet.txt") != 0) {
        return -1;
    }
    if (fruit_manager_load(&fruit_manager, "ovocie.txt") != 0) {
        return -1;
    }

    snake_init(&snake, 15, 15);
    if (fruit_next(&fruit_manager, &current_fruit) == 0) {
        world.grid[current_fruit.y][current_fruit.x] = FRUIT;
    }

    enable_raw_mode();  
    
    start_time = time(NULL);

    while (!game_over) {
        system("clear");
        world_draw(&world);
        
        int score = snake.length - 1;
        int current_time = time(NULL);
        int elapsed_time = (int)difftime(current_time, start_time);
        printf("\nSkore: %d\n", score);
        printf("Cas: %d sekund\n", elapsed_time);


        input(&key, &game_over);

        if (snake_move(&snake, &world, &ate_fruit, key) != 0 || 
            snake_check_collision(&snake)) {
            game_over = true;
            break;
        }

        if (ate_fruit) {
            if (fruit_next(&fruit_manager, &current_fruit) == 0) {
                world.grid[current_fruit.y][current_fruit.x] = FRUIT;
            } 
        }

        sleep_microseconds(100000);
    }
    printf("Game over! Score: %d\n", snake.length - 1);
    return 0;
}

