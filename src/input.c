#include "input.h"
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>


static struct termios oldt, newt;

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int kbhit(void) {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char get_input(void) {
    return getchar();
}

void input(int *key, int *gameover) {
    if (kbhit()) {
        char user_input = get_input();
        user_input = tolower(user_input);

        switch (user_input) {
            case 'w':
                if (*key != 4) *key = 3;  // Hore
                break;
            case 'a':
                if (*key != 2) *key = 1;  // Vľavo
                break;
            case 's':
                if (*key != 3) *key = 4;  // Dole
                break;
            case 'd':
                if (*key != 1) *key = 2;  // Vpravo
                break;
            case 'x':
                *gameover = true;  // Ukončenie hry
                break;
        }
    }
}

