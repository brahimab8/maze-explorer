#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stddef.h>

// High‑level input actions for both menu & gameplay
typedef enum {
    INP_NONE,
    INP_SELECT,
    INP_QUIT,       
    INP_PLAY,       
    INP_SETTINGS,   
    INP_UP,         
    INP_DOWN,       
    INP_LEFT,      
    INP_RIGHT,     
} InputAction;

// Initialize raw nonblocking input
void input_init(void);

void input_restore(void);

InputAction input_poll(void);

// prompt for a line of text (e.g. player name)
void input_get_line(const char *prompt, char *buf, size_t buflen);
#endif // INPUT_H
