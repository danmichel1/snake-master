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

  // --- Help Me Win: cached path to draw
  std::vector<std::pair<int,int>> helper_path;

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