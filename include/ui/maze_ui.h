#ifndef MAZE_UI_H
#define MAZE_UI_H

typedef struct Cell Cell;

// UI state, rendered alongside the maze
typedef struct {
    char   player_name[16];
    int    level;
    int    bullets;
    double time_secs;
} MazeUI;

// Draw the ASCII maze and the UI panel.
void draw_maze(Cell **grid, int rows, int cols, const MazeUI *ui);

#endif // MAZE_UI_H
