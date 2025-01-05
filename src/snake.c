#include "snake.h"
#include <stdio.h>
#include <unistd.h>

void snake_init(Snake *snake, int start_x, int start_y) {
  snake->length = 1;
  snake->direction = DOWN;

  for(int i = 0; i < snake->length; i++) {
    snake->body[i].x = start_x - i;
    snake->body[i].y = start_y;
  }
  printf("Snake initialized at position: %d %d \n", snake->body[0].x, snake->body[0].y);
}

void snake_set_direction(Snake *snake, Direction dir) {
  if ((snake->direction == UP && dir != DOWN) ||
      (snake->direction == DOWN && dir != UP) ||
      (snake->direction == LEFT && dir != RIGHT) ||
      (snake->direction == RIGHT && dir != LEFT)) {
      snake->direction = dir;
    } 
}

int snake_move(Snake *snake, World *world, bool *ate_fruit, int key) {

  Position new_head = snake->body[0];
  printf("Moving snake. currennt pos: %d %d, dir: %d\n", new_head.x, new_head.y, key);

  switch(key) {
    case 0: new_head.y--; break;
    case 1: new_head.x--; break;
    case 2: new_head.y++; break;
    case 3: new_head.x++; break;
    default: return -1;
  }

  if(world->grid[new_head.y][new_head.x] == WALL || 
     world->grid[new_head.y][new_head.x] == SNAKE) {
    return -1;
  }

  *ate_fruit = (world->grid[new_head.y][new_head.x] == FRUIT);
  
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
    world->grid[tail.y][tail.x] = EMPTY;
  } else {
    snake->length++;
  }

  for(int i = 0; i < snake->length; i++) {
    Position body_part = snake->body[i];
    world->grid[body_part.y][body_part.x] = SNAKE;
  }
  return 0;

}

bool snake_check_collision(const Snake *snake) {
  Position head = snake->body[0];

  for(int i = 1; i < snake->length; i++) {
    if(snake->body[i].x == head.x && snake->body[i].y == head.y) {
        return true;
     }
  }
  return false;
}

