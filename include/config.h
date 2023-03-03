#ifndef CONFIG_H
#define CONFIG_H

// Frame‐rate
#define FPS        30
#define FRAME_US  (1000000 / FPS)

// Maze size limits & defaults
#define MAZE_MIN_SIZE        4
#define MAZE_MAX_SIZE       32
#define MAZE_DEFAULT_WIDTH   8
#define MAZE_DEFAULT_HEIGHT  8

// Game defaults
#define INITIAL_SHOTS            10

// UI settings
#define UI_NAME_MAX         16   // max chars in player_name

#endif // CONFIG_H
