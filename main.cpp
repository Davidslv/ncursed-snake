#include <ncurses.h>
#include <iostream>
#include <list>
#include <chrono>
#include <thread>

using namespace std;

WINDOW *screen;
int screenWidth = 120;
int screenHeight = 40;
int key;

int foodX = 10;
int foodY = 10;
int score = 0;
bool    snakeDead   = false,
        rightKey    = false,
        leftKey     = false,
        rightKeyOld = false,
        leftKeyOld  = false;

/* 
    0 = north
    1 = east
    2 = south 
    3 = west
*/
int snakeDirection = 3;

struct SnakeSegment {
    int x;
    int y;
    
};

list <SnakeSegment> snake = { 
    { 60, 20 }, 
    { 61, 20 }, 
    { 62, 20 }, 
    { 63, 20 }, 
    { 64, 20 } 
};

void updateScreen(const char *info, int x, int y) {
    mvprintw(y, x, info);
    wrefresh(screen);
    refresh();
}

void screenSetup() {
    initscr();
    
    screen = newwin(screenHeight, screenWidth, 0, 0);
    noecho();
    refresh();

    box(screen, 0, 0);

    updateScreen("[ SNAKE ]", (screenWidth/2), 0);
}

int main() {
    screenSetup();

    while(((key = getch()) != 'q') && !snakeDead) {
        wrefresh(screen);

        // Timing and Input
        this_thread::sleep_for(std::chrono::milliseconds(200));

        rightKey = getch() == KEY_RIGHT;
        leftKey = getch() == KEY_LEFT;

        if (rightKey && !rightKeyOld) {
            snakeDirection++;
            if (snakeDirection == 4) snakeDirection = 0;
        }

        if (leftKey && !leftKeyOld) {
            snakeDirection--;
            if (snakeDirection == -1) snakeDirection = 3;
        }

        rightKeyOld = rightKey;
        leftKeyOld  = leftKey;

        // -------------------------------
        // Game Logic
        switch(snakeDirection) {
            case 0: // UP
                updateScreen("[ !SNAKE IS UP! ]", (screenWidth/2), 1);
                snake.push_front({snake.front().x, snake.front().y -1});
                break;
            case 1: // RIGHT
                updateScreen("[ !SNAKE IS RIGHT! ]", (screenWidth/2), 1);
                snake.push_front({snake.front().x + 1, snake.front().y});
                break;
        case 2: // DOWN
            updateScreen("[ !SNAKE IS DOWN! ]", (screenWidth/2), 1);
            snake.push_front({snake.front().x, snake.front().y +1});
            break;
        case 3: // LEFT
            updateScreen("[ !SNAKE IS LEFT! ]", (screenWidth/2), 1);
            snake.push_front({snake.front().x -1, snake.front().y});
            break;
        }

        snake.pop_back();

        // -------------------------------
        // Presentation
        for (auto s : snake) {
            // Snake body
            updateScreen((snakeDead ? "+" : "O"), s.x, s.y);
            
            // Snake head
            updateScreen((snakeDead ? "X" : "@"), snake.front().x , snake.front().y);

            // Fruit
            updateScreen("F", foodX, foodY);
        }
    }

    delwin(screen);
    endwin();
    return 0;
}
