#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <unistd.h>
#include "input.h"
#include "snake.h"
#include "snake_map.h"
#include <utility>
#include "macros.h"

using namespace std;

Snake snake;
SnakeMap snake_map(&snake);

void initialize()
{
    input_init();
    input_enter_off();
}

bool is_game_end()
{
    bool result = false;
    pair<int, int> snake_head = snake.snake_head;
    if (snake_head.first < 0 || snake_head.first >= MAP_HEIGHT || snake_head.second < 0 || snake_head.second >= MAP_WIDTH)
    {
        result = true;
    }
    if (snake.is_dead)
    {
        result = true;
    }
    return result;
}

void game_over()
{
    // Clear screen
    for (int i = 0; i < MAP_END; i++) {
        cout << endl;
    }
    
    // Game over screen - cleaner design
    cout << "========================================" << endl;
    cout << "            GAME OVER!" << endl;
    cout << "                                      " << endl;
    cout << "        Final Score: " << snake.length << endl;
    cout << "                                      " << endl;
    cout << "        Press 'R' to Restart          " << endl;
    cout << "        Press 'Q' to Quit             " << endl;
    cout << "========================================" << endl;
    cout << endl;
}

void start_game()
{
    while (true)
    {
        snake.update_movement();
        if (is_game_end())
        {
            game_over();
            
            // Game over loop - wait for restart or quit
            while (true) {
                char special_key = get_last_special_key();
                if (special_key == 'r') {
                    // Restart game
                    snake_map.restart_game();
                    break;  // Exit game over loop, continue main game
                } else if (special_key == 'q') {
                    // Quit game
                    return;  // Exit entire function
                }
                usleep(100000);  // Small delay to prevent busy waiting
            }
            continue;  // Continue main game loop after restart
        }
        
        snake_map.redraw();
        usleep(snake_map.get_current_pause_length());
        snake.validate_direction();
        
        // Toggle system - press 'h' for help, 'm' for difficulty
        static bool toggle_pressed = false;
        static bool last_was_error = false;
        
        bool current_is_error = snake.was_last_input_error();
        
        // Detect when error state changes from false to true (key pressed)
        if (current_is_error && !last_was_error && !toggle_pressed) {
            // A key was pressed that resulted in error - could be 'h' or 'm'
            toggle_pressed = true;
            char special_key = get_last_special_key();
            if (special_key == 'h') {
                snake_map.toggle_help_mode();
            } else if (special_key == 'm') {
                snake_map.cycle_difficulty_mode();
            }
        } else if (!current_is_error) {
            toggle_pressed = false;
        }
        
        last_was_error = current_is_error;
    }
}

void show_welcome_screen()
{
    // Clear screen
    for (int i = 0; i < MAP_END; i++) {
        cout << endl;
    }
    
    // Welcome screen - cleaner design
    cout << "========================================" << endl;
    cout << "            SNAKE GAME                " << endl;
    cout << "                                      " << endl;
    cout << "        Controls:                     " << endl;
    cout << "        W A S D - Move                " << endl;
    cout << "        H - Toggle Help               " << endl;
    cout << "        M - Change Difficulty         " << endl;
    cout << "        R - Restart (when game over)  " << endl;
    cout << "        Q - Quit (when game over)     " << endl;
    cout << "                                      " << endl;
    cout << "        Game starts automatically...  " << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    // Countdown timer - 3 seconds with live countdown
    cout << "Starting game in: ";
    for (int i = 3; i > 0; i--) {
        cout << i << "... ";
        cout.flush();  // Force output to display immediately
        usleep(1000000);  // 1 second delay
    }
    cout << "GO!" << endl;
}

int main()
{
    initialize();
    show_welcome_screen();
    start_game();
    return 0;
}
