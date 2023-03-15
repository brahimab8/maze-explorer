#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

/** High-level input actions for menus & gameplay **/
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

/** Switch terminal into raw, non‐blocking mode **/
void input_init(void);

/** Restore terminal state at exit **/
void input_restore(void);

/** Poll for a single high‐level action **/
InputAction input_poll(void);

/** Prompt the user for a line of text (blocking, with echo) **/
void input_get_line(const char *prompt, char *buf, size_t buflen);

#endif // INPUT_H
