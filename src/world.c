#include "world.h"
#include <stdio.h>
#include <string.h>

void world_init(World *world) {
  world->width = 0;
  world->height = 0;
  memset(world->grid, ' ', sizeof(world->grid));
}

int world_load(World *world, const char *filename) {
  FILE *file = fopen(filename, "r");
  if(!file) {
    perror("Chyba pri otvarani suboru na nacitanie herneho sveta!\n");
    return -1;
  }

  if(fscanf(file, "%d %d", &world->width, &world->height) != 2) {
    fclose(file);
    fprintf(stderr, "Neplatny format suboru! Pouzitie: int int\n");
    return -1;
  }

  for(int i = 0; i < world->height; i++) {
    for(int j = 0; j < world->width; j++) {
      if(i == 0 || i == world->height - 1 || j == 0 ||j == world->width - 1) {
        world->grid[i][j] = '#';
      } else {
        world->grid[i][j] = ' ';
         }
      }
  }

fclose(file);
return 0;
}

void world_draw(const World *world) {
  for(int i = 0; i < world->height; i++) {
    for(int j = 0; j < world->width; j++) {
      printf("%c", world->grid[i][j]);
    }
    printf("\n");
  }


}
