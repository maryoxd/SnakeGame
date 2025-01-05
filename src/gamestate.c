
#include "gamestate.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Inicializácia GameState
void gamestate_init(GameState *state, const char *world_file, const char *fruit_file) {
    world_init(&state->world);
    snake_init(&state->snake, 15, 15);
    fruit_manager_init(&state->world.fruit_manager);

    if (world_load(&state->world, world_file) != 0) {
        perror("Chyba pri načítaní sveta");
        state->game_over = 1;
        return;
    }

    if (fruit_manager_load(&state->world.fruit_manager, fruit_file) != 0) {
        perror("Chyba pri načítaní ovocia");
        state->game_over = 1;
        return;
    }
    for(int i = 0; i < state->snake.length; i++) {
      Position body_part = state->snake.body[i];
      state->world.grid[body_part.y][body_part.x] = SNAKE;
    }
    
    Fruit current_fruit;
    if(fruit_next(&state->world.fruit_manager, &current_fruit) == 0) {
      state->world.grid[current_fruit.y][current_fruit.x] = FRUIT;
    }

    state->score = 0;
    state->elapsed_time = 0;
    state->game_over = 0;
}

// Aktualizácia herného stavu
void gamestate_update(GameState *state, int direction) {
    if (state->game_over) return;

    bool ate_fruit = false;
    int result = snake_move(&state->snake, &state->world, &ate_fruit, direction);
    
    if(result < 0) {
      state->game_over = 1;
      return;
    }
    
    if(ate_fruit) {
      state->score++;
      Fruit current_fruit;
      if(fruit_next(&state->world.fruit_manager, &current_fruit) == 0) {
        state->world.grid[current_fruit.y][current_fruit.x] = FRUIT;
      }
    }
}

// Vykreslenie GameState
void gamestate_draw(const GameState *state) {
    system("clear");
    printf("Skore: %d\n", state->score);
    printf("Cas: %d sekund\n", state->elapsed_time);
    world_draw(&state->world);
}

// Kontrola stavu hry
int gamestate_is_game_over(const GameState *state) {
    return state->game_over;
}
