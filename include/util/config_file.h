#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <stdbool.h>

/**
 * Load persisted config values (FPS, etc.) from `path`.
 * Calls the appropriate settings_set_*() for each recognized key.
 * Returns true if the file existed and was parsed (even if some lines failed).
 */
bool config_load(const char *path);

/**
 * Save current config values (FPS, etc.) to `path`.
 * Writes a "Key=Value" line for each item.
 * Returns true on success.
 */
bool config_save(const char *path);

#endif // CONFIG_FILE_H
