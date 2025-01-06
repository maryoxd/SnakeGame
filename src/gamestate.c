#include "gamestate.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Inicializácia GameState
void gamestate_init(GameState *state, const char *world_file, const char *fruit_file) {
    // Inicializácia sveta, hada a ovocia
    world_init(&state->world);
    snake_init(&state->snake, 15, 15);
    fruit_manager_init(&state->world.fruit_manager);

    // Načítanie sveta
    if (world_load(&state->world, world_file) != 0) {
        perror("Chyba pri načítaní sveta");
        state->game_over = 1;
        return;
    }

    // Načítanie ovocia
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

    // Inicializácia skóre, času a stavu hry
    state->score = 0;
    state->elapsed_time = 0;
    state->game_over = 0;
    state->update_counter = 0;
}

// Aktualizácia herného stavu
void gamestate_update(GameState *state, int direction) {
    if (state->game_over) return; // Ak je hra skončená, neaktualizujeme

    state->update_counter++;

    // Aktualizácia času každých 10 cyklov
    if (state->update_counter >= 10) {
        state->elapsed_time++;
        state->update_counter = 0;
    }

    // Pohyb hada a kontrola kolízií
    bool ate_fruit = false;
    if (snake_move(&state->snake, &state->world, &ate_fruit, direction) != 0) {
        state->game_over = 1; // Kolízia alebo neplatný pohyb
        return;
    }

    // Ak had zjedol ovocie, zvýšime skóre a pridáme nové ovocie
    if (ate_fruit) {
        state->score++;
        Fruit current_fruit;
        if (fruit_next(&state->world.fruit_manager, &current_fruit) == 0) {
            state->world.grid[current_fruit.y][current_fruit.x] = FRUIT;
        }
    }
}

// Vykreslenie herného stavu
void gamestate_draw(const GameState *state) {
    system("clear");
    printf("Skóre: %d\n", state->score);
    printf("Čas: %d sekúnd\n", state->elapsed_time);
    world_draw(&state->world); // Použitie metódy na vykreslenie sveta
}

// Kontrola, či je hra skončená
int gamestate_is_game_over(const GameState *state) {
    return state->game_over;
}

