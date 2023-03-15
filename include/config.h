#ifndef CONFIG_H
#define CONFIG_H

// Maze size limits & defaults
#define MAZE_MIN_SIZE        4
#define MAZE_MAX_SIZE       32

#define MAZE_DEFAULT_WIDTH   8
#define MAZE_DEFAULT_HEIGHT  8

// Game defaults
#define INITIAL_SHOTS            10

#define MIN_INITIAL_SHOTS        0
#define MAX_INITIAL_SHOTS        100

// FPS limits & defaults
#define MIN_FPS 1
#define MAX_FPS 240
#define DEFAULT_FPS 30

// UI settings
#define UI_NAME_MAX         16   // max chars in player_name

// Persistent config file
#define CONFIG_FILE_PATH "persistent.cfg"

#endif // CONFIG_H
