#ifndef GAMEPLAY_PLAY_H
#define GAMEPLAY_PLAY_H

#include "game/game.h"  // GameContext, GameState
#include "ui/ui.h"      // UI*, InputAction

/**
 * Perform one “move” in the game:
 *  - update elapsed time  
 *  - draw maze  
 *  - read a single input action  
 *  - advance player, monsters, and projectiles  
 *  - check collisions/end‐of‐level  
 *  - sleep for frame_delay_ms  
 *
 * Returns STATE_PLAY_LEVEL to keep playing, or 
 * one of the other states (MENU, TRANSITION, EXIT).
 */
GameState play(GameContext *g, UI *ui);

#endif /* GAMEPLAY_PLAY_H */
