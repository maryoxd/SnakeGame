#include "snake.h"
#include <stdio.h>

void snake_init(Snake *snake, int start_x, int start_y) {
  snake->length = 3;
  snake->direction = DOWN;

  for(int i = 0; i < snake->length; i++) {
    snake->body[i].x = start_x - i;
    snake->body[i].y = start_y;
  }
}

void snake_set_direction(Snake *snake, Direction dir) {
  if ((snake->direction == UP && dir != DOWN) ||
      (snake->direction == DOWN && dir != UP) ||
      (snake->direction == LEFT && dir != RIGHT) ||
      (snake->direction == RIGHT && dir != LEFT)) {
      snake->direction = dir;
    }
}

int snake_move(Snake *snake, World *world, bool *ate_fruit) {
  Position new_head = snake->body[0];
  printf("Dlzka had pred pohybom: %d\n", snake->length);
  switch(snake->direction) {
    case UP: new_head.y--; break;
    case DOWN: new_head.y++; break;
    case LEFT: new_head.x--; break;
    case RIGHT: new_head.x++; break;
  }

  if(world->grid[new_head.y][new_head.x] == WALL || 
     world->grid[new_head.y][new_head.x] == SNAKE) {
    printf("Kolizia so stenou alebo hadom, koniech hry.\n");
    return -1;
  }

  *ate_fruit = (world->grid[new_head.y][new_head.x] == FRUIT);
  if(*ate_fruit) {
    printf("Had zjedol ovocie na suradniciach: %d %d\n", new_head.x, new_head.y);
  } else {
    printf("Had sa pohol na prazdnu suradnicu: %d %d\n", new_head.x, new_head.y);
  }
  
  for(int i = 0; i < snake->length; i++) {
    Position body_part = snake->body[i];
    world->grid[body_part.y][body_part.x]= EMPTY;

  }

  for(int i = snake->length - 1; i > 0; i--) {
    snake->body[i] = snake->body[i - 1];
  }
  snake->body[0] = new_head;
  
  if(!*ate_fruit) {
    Position tail = snake->body[snake->length - 1];
    printf("Odstranujem chvost na suradniciach: %d %d\n", tail.x, tail.y);
    world->grid[tail.y][tail.x] = EMPTY;
  } else {
    snake->length++;
    printf("Had sa zvacszil, nova dlzka: %d\n", snake->length);
  }

  for(int i = 0; i < snake->length; i++) {
    Position body_part = snake->body[i];
    world->grid[body_part.y][body_part.x] = SNAKE;
  }
  return 0;

}

bool snake_check_collision(const Snake *snake, const World *world) {
  Position head = snake->body[0];

  for(int i = 1; i < snake->length; i++) {
    if(snake->body[i].x == head.x && snake->body[i].y == head.y) {
        return true;
     }
  }
  return false;
}
