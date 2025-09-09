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
#define HELP_PATH_CHAR '+'
#define HELP_MAX_N 5   // show helper path for first N apples eaten

#endif