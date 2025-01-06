#ifndef MENU_H
#define MENU_H

#include "gamestate.h"

void handle_menu_server(int client_sock, GameState *game_state);
void show_menu_client(int sockfd);

#endif

