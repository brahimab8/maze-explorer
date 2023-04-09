#include "gameplay/player.h"
#include "gameplay/projectile.h"

void player_init(Player *p, int sx, int sy, int initial_bullets, char symbol) {
    p->x = sx; p->y = sy;
    p->dir = DIR_DOWN;
    p->bullets = initial_bullets;
    p->symbol  = symbol;
}

void player_move(Player *p,
                 InputAction act,
                 Cell **grid,
                 int rows,
                 int cols)
{
    Direction dir;
    switch (act) {
      case INP_UP:    dir = DIR_UP;    break;
      case INP_DOWN:  dir = DIR_DOWN;  break;
      case INP_LEFT:  dir = DIR_LEFT;  break;
      case INP_RIGHT: dir = DIR_RIGHT; break;
      default:        return;          // ignore anything else
    }

    // only step if no wall blocks us
    if (can_step(grid, rows, cols, p->x, p->y, dir)) {
        switch (dir) {
          case DIR_UP:    p->y--; break;
          case DIR_DOWN:  p->y++; break;
          case DIR_LEFT:  p->x--; break;
          case DIR_RIGHT: p->x++; break;
        }
    }
    p->dir = dir;
}

bool player_shoot(Player *p,
                  Projectile *plist,
                  int *pcount)
{
    if (p->bullets <= 0) {
        return false;
    }
    p->bullets--;

    // // compute spawn cell just ahead of the player
    // int sx = p->x, sy = p->y;
    // switch (p->dir) {
    //   case DIR_UP:    sy--; break;
    //   case DIR_DOWN:  sy++; break;
    //   case DIR_LEFT:  sx--; break;
    //   case DIR_RIGHT: sx++; break;
    // }

    // shoot from exactly where the player stands
    projectile_fire(plist, pcount, p->x, p->y, p->dir);
    return true;
}
