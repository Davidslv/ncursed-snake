#include <ncurses.h>
#include <iostream>
#include <list>
#include <chrono>
#include <time.h>


// this is necessary to slow down the game 200ms
#include <thread>

using namespace std;

WINDOW *screen;

int screenWidth  = 120,
    screenHeight = 40,
    foodX = 40,
    foodY = 20,
    score = 0;

bool snakeAlive  = true,
     rightKey    = false,
     leftKey     = false,
     rightKeyOld = false,
     leftKeyOld  = false,
     foodEaten   = false;


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
    { 64, 20 },
    { 65, 20 }
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
    updateScreen("[ SNAKE ]", (screenWidth/2) - 5, 0);
}

void updateScore() {
    mvprintw(0, 2, "[SCORE: %i ]", score);
}

void snakeUpdate() {
    // -------------------------------
    // Presentation
    for (auto s : snake) {
        // Snake body
        updateScreen((snakeAlive ? "O" : "+"), s.x, s.y);

        // Snake head
        updateScreen((snakeAlive ? "@" : "X"), snake.front().x , snake.front().y);

        // Remove snake last tail position.
        // this works together with snake.pop_back();
        // although this prevents the last segment of the snake to be shown.
        // meaning that when the snake 5 segments it only shows 4.
        updateScreen(" ", snake.back().x, snake.back().y);

        // Fruit
        updateScreen("F", foodX, foodY);
    }
}

void gameLogic() {
    // Timing and Input
    this_thread::sleep_for(std::chrono::milliseconds(190));

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

    // -------------------------------
    // Collision detection

    // World boundaries
    if (snake.front().x < 1 || snake.front().x >= (screenWidth -1))
        snakeAlive = false;

    if (snake.front().y < 1 || snake.front().y >= (screenHeight -1))
        snakeAlive = false;

    // Fruit collision
    if (snake.front().x == foodX && snake.front().y == foodY) {
        updateScreen(" ", foodX, foodY);
        score++;
        foodEaten = true;

        while(foodEaten) {
            foodX = rand() % (screenWidth - 1);
            foodY = rand() % (screenHeight - 1);

            if ((mvinch(foodY, foodX) == ' '))
                foodEaten = false;
        }
    }
}

void screenUpdate() {
    while(snakeAlive) {
        wrefresh(screen);

        gameLogic();
        userInput();
        updateScore();
    }
}

void clearScreen() {
    delwin(screen);
    endwin();
}

int main() {
    srand(time(NULL));

    screenSetup();
    screenUpdate();
    clearScreen();

    return 0;
}
