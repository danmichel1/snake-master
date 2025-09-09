#include "snake_map.h"
#include <pthread.h>
#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include "macros.h"
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

SnakeMap::SnakeMap(Snake *snake)
{
    this->snake = snake;
    this->help_mode_enabled = true;  // Start with help mode ON
    this->difficulty_mode = 0;       // Start with Easy mode (0=Easy, 1=Medium, 2=Hard)
    clear_map(this->map_array);
    srand(time(NULL));
    update_snake_food(true);
}

bool SnakeMap::bfs_find_path(std::pair<int,int> start,
                             std::pair<int,int> goal,
                             std::vector<std::pair<int,int>>& outPath)
{
    const int R = MAP_HEIGHT, C = MAP_WIDTH;

    // Block snake body cells
    std::vector<std::vector<bool>> blocked(R, std::vector<bool>(C, false));
    for (auto &p : snake->snake_parts) {
        int i = p.first, j = p.second;
        if (i >= 0 && i < R && j >= 0 && j < C) blocked[i][j] = true;
    }
    // allow the head cell as start
    blocked[start.first][start.second] = false;

    static int di[4] = {-1, 1, 0, 0};
    static int dj[4] = {0, 0, -1, 1};

    std::queue<std::pair<int,int>> q;
    std::vector<std::vector<bool>> vis(R, std::vector<bool>(C, false));
    std::map<std::pair<int,int>, std::pair<int,int>> parent;

    q.push(start);
    vis[start.first][start.second] = true;

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        if (cur == goal) {
            std::vector<std::pair<int,int>> path;
            for (auto v = goal; v != start; v = parent[v]) path.push_back(v);
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            outPath.swap(path);
            return true;
        }
        for (int k = 0; k < 4; ++k) {
            int ni = cur.first + di[k], nj = cur.second + dj[k];
            if (ni < 0 || ni >= R || nj < 0 || nj >= C) continue;
            if (vis[ni][nj]) continue;
            if (blocked[ni][nj]) continue;
            vis[ni][nj] = true;
            parent[{ni,nj}] = cur;
            q.push({ni,nj});
        }
    }
    return false;
}

void SnakeMap::compute_helper_path()
{
    helper_path.clear();

    // Don't compute path if help mode is disabled
    if (!help_mode_enabled) return;

    // Limit to first N apples eaten
    int apples_eaten = snake->length - INITIAL_SNAKE_LENGTH;
    if (apples_eaten > HELP_MAX_N) return;

    std::vector<std::pair<int,int>> path;
    if (bfs_find_path(snake->snake_head, snake_food, path)) {
        helper_path = path; // keep full path; drawing will avoid overwriting snake/food
    }
}

void SnakeMap::redraw(void)
{
    clear_map(this->map_array);
    for (int i = 0; i < MAP_END; i++)
    {
        cout << endl;
    }
    update_score();
    vector<pair<int, int>> snake_parts = snake->snake_parts;
    for (int i = 0; i < snake_parts.size(); i++)
    {
        pair<int, int> tmp = snake_parts[i];
        map_array[tmp.first][tmp.second] = SNAKE_CHAR;
    }
    update_snake_head(map_array, snake);
    update_snake_food(false);
    map_array[snake_food.first][snake_food.second] = SNAKE_FOOD_CHAR;
    
    // draw helper path as 'o' without overwriting snake body/head or food
    if (help_mode_enabled) {
        for (auto &p : helper_path) {
            int i = p.first, j = p.second;
            if (i < 0 || i >= MAP_HEIGHT || j < 0 || j >= MAP_WIDTH) continue;
            if (map_array[i][j] == MAP_CHAR) {
                map_array[i][j] = HELP_PATH_CHAR;
            }
        }
    }
    
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (map_array[i][j] == HELP_PATH_CHAR) {
                cout << "\033[32m" << map_array[i][j] << "\033[0m ";  // Green
            } else if (map_array[i][j] == SNAKE_FOOD_CHAR) {
                cout << "\033[31m" << map_array[i][j] << "\033[0m ";  // Red
            } else {
                cout << map_array[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void SnakeMap::update_snake_food(bool force_update)
{
    if (snake->food_eaten || force_update)
    {
        while (true)
        {
            int random_i = rand() % MAP_WIDTH;
            int random_j = rand() % MAP_HEIGHT;
            if (map_array[random_i][random_j] == MAP_CHAR)
            {
                snake_food = make_pair(random_i, random_j);
                snake->set_snake_food(snake_food);
                snake->food_eaten = false;
                compute_helper_path();
                break;
            }
        }
    }
}

void clear_map(char map_array[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            map_array[i][j] = MAP_CHAR;
        }
    }
}

void update_snake_head(char map_array[MAP_HEIGHT][MAP_WIDTH], Snake *snake)
{
    char snake_head_char = SNAKE_CHAR;
    enum Direction direction = snake->get_direction();
    switch (direction)
    {
    case West:
        snake_head_char = SNAKE_HEAD_WEST;
        break;
    case North:
        snake_head_char = SNAKE_HEAD_NORTH;
        break;
    case East:
        snake_head_char = SNAKE_HEAD_EAST;
        break;
    case South:
        snake_head_char = SNAKE_HEAD_SOUTH;
        break;
    case Error: break;
    }
    pair<int, int> snake_head = snake->snake_head;
    map_array[snake_head.first][snake_head.second] = snake_head_char;
}

void SnakeMap::update_score(void)
{
    cout << "Score:" << snake->length;
    if (help_mode_enabled) {
        cout << " [HELP ON]";
    } else {
        cout << " [HELP OFF]";
    }
    
    // Show difficulty mode
    switch (difficulty_mode) {
        case 0: cout << " [EASY]"; break;
        case 1: cout << " [MEDIUM]"; break;
        case 2: cout << " [HARD]"; break;
    }
    cout << endl;
}

void SnakeMap::toggle_help_mode()
{
    help_mode_enabled = !help_mode_enabled;
    if (!help_mode_enabled) {
        helper_path.clear();  // Clear any existing path when turning off
    } else {
        // When turning help mode back on, recompute the path
        compute_helper_path();
    }
}

void SnakeMap::cycle_difficulty_mode()
{
    difficulty_mode = (difficulty_mode + 1) % 3;  // Cycle through 0, 1, 2
}

int SnakeMap::get_current_pause_length()
{
    switch (difficulty_mode) {
        case 0: return PAUSE_EASY;    // 200ms
        case 1: return PAUSE_MEDIUM;  // 150ms
        case 2: return PAUSE_HARD;    // 100ms
        default: return PAUSE_EASY;
    }
}

void SnakeMap::restart_game()
{
    // Reset map state
    clear_map(this->map_array);
    helper_path.clear();
    
    // Reset snake
    snake->restart_game();
    
    // Generate new food
    update_snake_food(true);
}