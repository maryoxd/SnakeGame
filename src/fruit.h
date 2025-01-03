#ifndef FRUIT_H
#define FRUIT_H

#include "world.h"
#include "common.h"

typedef struct {
  int x;
  int y;

} Fruit;

typedef struct {
  Fruit fruits[MAX_WIDTH * MAX_HEIGHT];
  int count;
  int current_index;

} FruitManager;

void fruit_manager_init(FruitManager *manager);
int fruit_manager_load(FruitManager *manager, const char *filename);
int fruit_next(FruitManager *manager, Fruit *fruit);

#endif
