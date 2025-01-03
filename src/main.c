#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>
#include <unistd.h>

#include "world.h"
#include "fruit.h"
#include "snake.h"
#include "common.h"

int main() {
    World world;
    FruitManager fruit_manager;
    Snake snake;
    Fruit current_fruit;
    bool ate_fruit = false;
    bool game_over = false;

    world_init(&world);
    fruit_manager_init(&fruit_manager);

  
    if (world_load(&world, "hernysvet.txt") != 0) {
        return -1;
    }
    if (fruit_manager_load(&fruit_manager, "ovocie.txt") != 0) {
        return -1;
    }
    
    snake_init(&snake, 5, 5);  
    if (fruit_next(&fruit_manager, &current_fruit) == 0) {
        world.grid[current_fruit.y][current_fruit.x] = FRUIT;
    }
    while(!game_over) {
      system("clear");
      printf("Ovocie na suradniciach: (%d, %d)\n", current_fruit.x, current_fruit.y);
      world_draw(&world);
      if(snake_move(&snake, &world, &ate_fruit) != 0 || snake_check_collision(&snake, &world)) {
      game_over = true;
      break;
    }
      if(ate_fruit) {
        printf("Had zjedol ovocie, pridavam nove..\n");
        if(fruit_next(&fruit_manager, &current_fruit) == 0) {
          printf("Nove ovocie: %d %d\n", current_fruit.x, current_fruit.y);
          world.grid[current_fruit.y][current_fruit.x] = FRUIT;
        usleep(200000);
         } else {
            printf("Vsetko ovocie zjedene\n");
         } 
      }
    
      
      usleep(200000);
  }
    printf("Game over! Score: %d\n", snake.length - 3);
    return 0;
}
