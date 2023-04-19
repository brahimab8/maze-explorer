#include "gameplay/item.h"
#include <stdlib.h>

void items_init(Item items[], int *count,
                int rows, int cols,
                int sx, int sy,
                char pickup_symbol)
{
    *count = 0;
    for (int i = 0; i < MAX_ITEMS; ++i) {
        int x, y;
        // find a spot at least 1 cell away from (sx,sy)
        do {
            x = rand() % cols;
            y = rand() % rows;
        } while (abs(x - sx) <= 1 && abs(y - sy) <= 1);

        items[i].x      = x;
        items[i].y      = y;
        items[i].active = true;
        items[i].symbol = pickup_symbol;
        (*count)++;
    }
}
