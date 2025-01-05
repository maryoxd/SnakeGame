#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "world.h"
#include "snake.h"
#include "fruit.h"

typedef struct GameState {
    World world;         // Herný svet (mapa)
    Snake snake;         // Had
    int score;           // Aktuálne skóre
    int elapsed_time;    // Uplynulý čas v sekundách
    int game_over;       // Indikátor, či hra skončila (1 = áno, 0 = nie)
} GameState;

// Funkcie na prácu s GameState
void gamestate_init(GameState *state, const char *world_file, const char *fruit_file);
void gamestate_update(GameState *state, int direction);
void gamestate_draw(const GameState *state);
int gamestate_is_game_over(const GameState *state);

#endif

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

    state->score = 0;
    state->elapsed_time = 0;
    state->game_over = 0;
}

// Aktualizácia herného stavu
void gamestate_update(GameState *state, int direction) {
    if (state->game_over) return;

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
    bool grow = 0;
    if (state->world.grid[next.y][next.x] == FRUIT) {
        grow = true;
        state->score++;
        Fruit current_fruit;
        if (fruit_next(&state->world.fruit_manager, &current_fruit) == 0) {
            state->world.grid[current_fruit.y][current_fruit.x] = FRUIT;
        }
    }

    snake_move(&state->snake, &state->world, &grow, direction);
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
