#include "engine/maze.h"
#include "game/game.h"
#include "menu/main_menu.h"
#include "util/save_game.h"
#include <stdlib.h>
#include <string.h>

// Show the main menu and perform the selected action.
GameState game_menu(GameContext *g, UI *ui)
{
  int indices[MAIN_COUNT], total = 0;
  if (g->grid)
  {
    indices[total++] = MAIN_CONTINUE;
  }
  indices[total++] = MAIN_NEW;
  indices[total++] = MAIN_LOAD;
  indices[total++] = MAIN_SETTINGS;
  indices[total++] = MAIN_QUIT;

  static const char *all_labels[MAIN_COUNT] = {
      "Continue", "New Game", "Load Game", "Settings", "Quit"};

  const char **labels = malloc(sizeof(*labels) * total);
  for (int i = 0; i < total; i++)
  {
    labels[i] = all_labels[indices[i]];
  }

  int raw = ui->run_menu("Main Menu", labels, total);
  free(labels);
  if (raw < 0 || raw >= total)
  {
    return STATE_MENU;
  }

  MainOption choice = (MainOption)indices[raw];
  switch (choice)
  {
  case MAIN_CONTINUE:
    return STATE_PLAY_LEVEL;

  case MAIN_NEW:
    game_clear_grid(g);
    g->maze.level = 1;
    g->maze.time_secs = 0.0;
    ui->save_slot(g->slot,
                  &g->maze,
                  g->cfg.height,
                  g->cfg.width,
                  g->maze.time_secs);
    return STATE_SETUP_LEVEL;

  case MAIN_LOAD:
  {
    char *slot = ui->run_load_menu(ui);
    if (slot)
    {
      free(g->slot);
      g->slot = slot;
      load_game_slot(slot,
                     &g->maze,
                     &g->cfg.height,
                     &g->cfg.width,
                     &g->maze.time_secs);
      return STATE_SETUP_LEVEL;
    }
    return STATE_MENU;
  }

  case MAIN_SETTINGS:
    ui->run_settings_menu(ui);
    settings_get(&g->cfg);
    g->frame_delay_ms = 1000 / g->cfg.fps;

    game_clear_grid(g);
    ui->save_slot(g->slot,
                  &g->maze,
                  g->cfg.height,
                  g->cfg.width,
                  g->maze.time_secs);
    return STATE_MENU;

  case MAIN_QUIT:
    return STATE_EXIT;

  default:
    return STATE_MENU;
  }
}
