#include "gamestate.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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

    // Umiestnenie hada do herného sveta
    for (int i = 0; i < state->snake.length; i++) {
        Position body_part = state->snake.body[i];
        state->world.grid[body_part.y][body_part.x] = SNAKE;
    }

    // Umiestnenie prvého ovocia
    Fruit current_fruit;
    if (fruit_next(&state->world.fruit_manager, &current_fruit) == 0) {
        state->world.grid[current_fruit.y][current_fruit.x] = FRUIT;
    }

    state->score = 0;
    state->elapsed_time = 0;
    state->game_over = 0;
    state->update_counter = 0;
}

// Aktualizácia herného stavu
void gamestate_update(GameState *state, int direction) {
    
    if(state->game_over) return;
    
    state->update_counter++;
    
    if(state->update_counter >= 10) {
      state->elapsed_time++;
      state->update_counter = 0; 
    }
    printf("counter: %d, elapsnute: %d \n", state->update_counter, state->elapsed_time);


    Position next = state->snake.body[0];  // Hlava hada
    if (direction == 0) next.y--;          // Hore
    if (direction == 1) next.x--;          // Doľava
    if (direction == 2) next.y++;          // Dole
    if (direction == 3) next.x++;          // Doprava

    // Kontrola kolízie
    if (state->world.grid[next.y][next.x] == WALL ||
        state->world.grid[next.y][next.x] == SNAKE) {
        state->game_over = 1;
        return;
    }

    // Kontrola ovocia
    bool grow = false;
    if (state->world.grid[next.y][next.x] == FRUIT) {
        grow = true;
        state->score++;
        Fruit current_fruit;
        if (fruit_next(&state->world.fruit_manager, &current_fruit) == 0) {
            state->world.grid[current_fruit.y][current_fruit.x] = FRUIT;
        }
    }

    // Pohyb hada
    if (snake_move(&state->snake, &state->world, &grow, direction) != 0) {
        state->game_over = 1; // Kolízia v hadovi
    }

    // Aktualizácia času
    
}

// Vykreslenie GameState
void gamestate_draw(const GameState *state) {
    system("clear");
    printf("Skóre: %d\n", state->score);
    printf("Čas: %d sekúnd\n", state->elapsed_time);
    world_draw(&state->world);
}

// Kontrola stavu hry
int gamestate_is_game_over(const GameState *state) {
    return state->game_over;
}

