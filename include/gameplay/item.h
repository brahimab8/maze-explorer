#ifndef GAMEPLAY_ITEM_H
#define GAMEPLAY_ITEM_H

#include <stdbool.h>
#include "engine/maze.h"

#define MAX_ITEMS 2

typedef struct {
    int  x, y;
    bool active;
    char symbol;
} Item;

/**
 * Randomly place up to MAX_ITEMS items in the maze at positions
 * at least 1 cell away from (sx,sy).
 * Sets *count to the number placed (<= MAX_ITEMS).
 */
void items_init(Item items[], int *count,
                int rows, int cols,
                int sx, int sy,
                char pickup_symbol);

#endif // GAMEPLAY_ITEM_H
