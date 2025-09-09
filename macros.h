#ifndef _macros_h
#define _macros_h

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define MAP_CHAR '.'
#define MAP_END 30
#define SNAKE_CHAR 'O'
#define SNAKE_HEAD_WEST '>'
#define SNAKE_HEAD_NORTH 'v'
#define SNAKE_HEAD_EAST '<'
#define SNAKE_HEAD_SOUTH '^'
#define SNAKE_FOOD_CHAR '*'

#define INITIAL_SNAKE_LENGTH 3

#define PAUSE_LENGTH 1000 * 200 // 200 milliseconds

// --- Help Me Win mode settings ---
#define HELP_PATH_CHAR 'o'
#define HELP_MAX_N 15   // show helper path for first N apples eaten
#define HELP_TOGGLE_KEY 'h'  // press 'h' to toggle help mode on/off

// --- Difficulty mode settings ---
#define DIFFICULTY_TOGGLE_KEY 'm'  // press 'm' to cycle through difficulty modes
#define PAUSE_EASY 1000 * 200      // 200ms (current speed)
#define PAUSE_MEDIUM 1000 * 150    // 150ms (faster)
#define PAUSE_HARD 1000 * 100      // 100ms (much faster)

#endif