#include "input.h"
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

struct termios t;
static char last_special_key = 0;

void input_enter_off()
{
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void input_enter_on()
{
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

enum Direction get_input()
{
    enum Direction result = East;
    char user_input = getchar();
    switch (user_input)
    {
    case 'a':
        result = West;
        break;
    case 'w':
        result = North;
        break;
    case 'd':
        result = East;
        break;
    case 's':
        result = South;
        break;
    case 'h':
        result = Error;  // Special case for help toggle
        last_special_key = 'h';
        cout << "Help toggle pressed!" << endl;
        break;
    case 'm':
        result = Error;  // Special case for difficulty toggle
        last_special_key = 'm';
        cout << "Difficulty toggle pressed!" << endl;
        break;
    case 'r':
        result = Error;  // Special case for restart
        last_special_key = 'r';
        cout << "Restart pressed!" << endl;
        break;
    case 'q':
        result = Error;  // Special case for quit
        last_special_key = 'q';
        cout << "Quit pressed!" << endl;
        break;
    default:
        result = Error;
        cout << "Incorrect button clicked(" << user_input << ")" << endl;
        break;
    }
    return result;
}

bool check_help_toggle()
{
    // Check if 'h' key was pressed (non-blocking)
    // This is a simplified implementation
    return false;  // Will be handled in main loop
}

char get_last_special_key()
{
    char key = last_special_key;
    last_special_key = 0;  // Reset after reading
    return key;
}

void input_init()
{
    
}