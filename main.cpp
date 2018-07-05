#include <ncurses.h>
#include <iostream>
#include <list>
#include <chrono>
#include <thread>
// #include <pthread/pthread.h>

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


enum SnakeDirection { UP, RIGHT, DOWN, LEFT };
SnakeDirection snakeDirection = LEFT;

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

void snakeUpdate() {
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

void gameLogic() {
    // Timing and Input
    this_thread::sleep_for(std::chrono::milliseconds(200));

    // -------------------------------
    // Game Logic
    switch(snakeDirection) {
        case UP:
            snake.push_front({snake.front().x, snake.front().y -1});
            break;
        case RIGHT:
            snake.push_front({snake.front().x + 1, snake.front().y});
            break;
        case DOWN:
            snake.push_front({snake.front().x, snake.front().y +1});
            break;
        case LEFT:
            snake.push_front({snake.front().x -1, snake.front().y});
            break;
    }

    // doesn't seem to be working
    // the intention is to remove the tail of the snake
    // since the snake has moved forward.
    snake.pop_back();

    snakeUpdate();
}

void userInput() {
    // this function is here to guarantee that the
    // getch() function reads user input in a non-blocking way
    // so that the snake keeps moving.
    nodelay(stdscr, TRUE);

    // Avoid user input if the input goes in the oposite direction
    // that the snake is going. This avoids the snake going over herself.
    switch(getch()) {
        case 'w':
            if (snakeDirection != DOWN) {
                snakeDirection = UP;
            }
            break;
        case 'd':
            if (snakeDirection != LEFT) {
                snakeDirection = RIGHT;
            }
            break;
        case 's':
            if (snakeDirection != UP) {
                snakeDirection = DOWN;
            }
            break;
        case 'a':
            if (snakeDirection != RIGHT) {
                snakeDirection = LEFT;
            }
            break;
    }
}

void screenUpdate() {
    while(snakeAlive) {
        wrefresh(screen);

        gameLogic();
        userInput();
    }
}

void clearScreen() {
    delwin(screen);
    endwin();
}

int main() {
    screenSetup();
    screenUpdate();
    clearScreen();

    return 0;
}
