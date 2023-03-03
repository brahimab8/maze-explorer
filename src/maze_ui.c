#include "maze_ui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char *ui_lines[4] = {
    "Player: %s",
    "Level: %d",
    "Bullets: %d",
    "Time: %02d:%02d.%d"
};

static void build_line(char *buf, size_t buf_size, int cols, Cell **grid, int ln)
{
    int pos = 0;
    if (ln == 0) {
        for (int c = 0; c < cols; ++c)
            pos += snprintf(buf + pos, buf_size - pos, "+---");
        pos += snprintf(buf + pos, buf_size - pos, "+");
    } else if (ln & 1) {
        int r = ln / 2;
        for (int c = 0; c < cols; ++c)
            pos += snprintf(buf + pos, buf_size - pos, "%c   ",
                            grid[r][c].wallW ? '|' : ' ');
        pos += snprintf(buf + pos, buf_size - pos, "%c",
                        grid[r][cols - 1].wallE ? '|' : ' ');
    } else {
        int r = ln / 2 - 1;
        for (int c = 0; c < cols; ++c)
            pos += snprintf(buf + pos, buf_size - pos, "+%s",
                            grid[r][c].wallS ? "---" : "   ");
        pos += snprintf(buf + pos, buf_size - pos, "+");
    }
    buf[pos] = '\0';
}

void draw_maze(Cell **grid, int rows, int cols, const MazeUI *ui)
{
    system("clear");
    int total = 2 * rows + 1;
    int width = cols * 4 + 1;
    char line[256], info[64];

    for (int ln = 0; ln < total; ++ln) {
        build_line(line, sizeof(line), cols, grid, ln);
        int len = strlen(line);
        if (len < width) {
            memset(line + len, ' ', width - len);
            line[width] = '\0';
        }

        info[0] = '\0';
        if (ln < 4) {
            switch (ln) {
                case 0:
                    snprintf(info, sizeof(info), ui_lines[0], ui->player_name);
                    break;
                case 1:
                    snprintf(info, sizeof(info), ui_lines[1], ui->level);
                    break;
                case 2:
                    snprintf(info, sizeof(info), ui_lines[2], ui->bullets);
                    break;
                case 3: {
                    int mm = (int)(ui->time_secs / 60);
                    int ss = ((int)ui->time_secs) % 60;
                    int d  = (int)((ui->time_secs - (int)ui->time_secs) * 10);
                    snprintf(info, sizeof(info), ui_lines[3], mm, ss, d);
                    break;
                }
            }
        }

        printf("%s%s\n", line, info);
    }
}
