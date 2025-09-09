#ifndef _snake_map_h
#define _snake_map_h

#include "snake.h"
#include "macros.h"
#include <vector>
#include <utility>

class SnakeMap
{
public:
  SnakeMap(Snake *snake);
  void redraw();
  std::pair<int, int> snake_food;
  void update_snake_food(bool force_update);
  void update_score();
  void toggle_help_mode();
  void cycle_difficulty_mode();
  int get_current_pause_length();
  void restart_game();

  // --- Help Me Win: cached path to draw
  std::vector<std::pair<int,int>> helper_path;
  bool help_mode_enabled;
  
  // --- Difficulty mode settings
  int difficulty_mode;  // 0=Easy, 1=Medium, 2=Hard

private:
  char map_array[MAP_HEIGHT][MAP_WIDTH];
  Snake *snake;

  // --- Help Me Win: internals
  void compute_helper_path();
  bool bfs_find_path(std::pair<int,int> start,
                     std::pair<int,int> goal,
                     std::vector<std::pair<int,int>>& outPath);
};

void clear_map(char map_array[MAP_HEIGHT][MAP_WIDTH]);
void update_snake_head(char map_array[MAP_HEIGHT][MAP_WIDTH], Snake *snake);

#endif