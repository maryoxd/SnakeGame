#ifndef COMMON_H
#define COMMON_H

#include <time.h>  // Ak je to potrebné aj tu

typedef struct {
    int x;
    int y;
} Position;

#define EMPTY ' '   // Prazdno
#define WALL '#'    // Stena
#define SNAKE 'O'   // Had
#define FRUIT '*'   // Ovocie

void sleep_microseconds(long microseconds);

#endif

