#include "input/input.h"

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>    
#include <string.h>


static struct termios orig_termios;

void input_restore(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void input_init(void) {
    struct termios t;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(input_restore);

    t = orig_termios;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN]  = 0;
    t.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    signal(SIGINT, SIG_DFL);
}

InputAction input_poll(void) {
    char buf[3];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) {
        return INP_NONE;
    }

    // Single‑char commands
    if (n == 1) {
        switch (buf[0]) {
            case 'q':  return INP_QUIT;
            case 'p':  return INP_PLAY;
            case 'w':  return INP_UP;
            case 's':  return INP_DOWN;
            case 'a':  return INP_LEFT;
            case 'd':  return INP_RIGHT;
            case '\n': return INP_SELECT;
            default:   return INP_NONE;
        }
    }

    // Arrow keys (ESC [ A/B/C/D)
    if (n == 3 && buf[0] == 0x1B && buf[1] == '[') {
        switch (buf[2]) {
            case 'A': return INP_UP;
            case 'B': return INP_DOWN;
            case 'C': return INP_RIGHT;
            case 'D': return INP_LEFT;
            default:  return INP_NONE;
        }
    }

    return INP_NONE;
}


// Read a line of input with echo & canonical mode
void input_get_line(const char *prompt, char *buf, size_t buflen) {
    struct termios cooked;
    tcgetattr(STDIN_FILENO, &cooked);

    // Switch to canonical+echo
    struct termios raw = cooked;
    raw.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    // Prompt + read
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, buflen, stdin)) {
        size_t len = strlen(buf);
        if (len && buf[len-1] == '\n') buf[len-1] = '\0';
    } else {
        buf[0] = '\0';
    }

    // Flush any extra keystrokes (e.g. leftover newline)
    tcflush(STDIN_FILENO, TCIFLUSH);

    // Re-enter raw, non-blocking, no-echo mode
    // (this will restore orig_termios & rewire VMIN/VTIME)
    input_init();
}
