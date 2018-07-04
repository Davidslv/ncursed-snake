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
bool    snakeAlive  = true,
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
    curs_set(0); // remove cursor from screen
    
    screen = newwin(screenHeight, screenWidth, 0, 0);
    noecho();
    cbreak();
    refresh();

    box(screen, 0, 0);

    updateScreen("[ SNAKE ]", (screenWidth/2), 0);
}

int main() {
    screenSetup();

    while(((key = getch()) != 'q') && snakeAlive) {
        wrefresh(screen);

        // Timing and Input
        this_thread::sleep_for(std::chrono::milliseconds(200));

        switch(key) {
            case KEY_RIGHT:
                snakeDirection++;
                if (snakeDirection == 4) snakeDirection = 0;
            break;
            case KEY_LEFT:
                snakeDirection--;
                if (snakeDirection == -1) snakeDirection = 3;
            break;
        }

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
            updateScreen((snakeAlive ? "O" : "+"), s.x, s.y);
            
            // Snake head
            updateScreen((snakeAlive ? "@" : "X"), snake.front().x , snake.front().y);

            // Fruit
            updateScreen("F", foodX, foodY);
        }
    }

    delwin(screen);
    endwin();
    return 0;
}
