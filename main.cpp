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
    nodelay(screen, TRUE); // probably can be remove
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
        case 0: // UP
            snake.push_front({snake.front().x, snake.front().y -1});
            break;
        case 1: // RIGHT
            snake.push_front({snake.front().x + 1, snake.front().y});
            break;
        case 2: // DOWN
            snake.push_front({snake.front().x, snake.front().y +1});
            break;
        case 3: // LEFT
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
    if (getch() == 'a') {
        snakeDirection++;
        if (snakeDirection == 4) snakeDirection = 0;
    }
    else if (getch() == 'd') {
        snakeDirection--;
        if (snakeDirection == -1) snakeDirection = 3;
    }
}

void screenUpdate() {
    while(snakeAlive) {
        wrefresh(screen);

        std::thread user_input(userInput);
        std::thread game(gameLogic);

        // The intention here is to keep the game running
        // without waiting for user input.
        // Although the new problem is that the snake can turn very late or not at all
        // not sure what could be the solution at the moment.
        // Windows.h library has GetAsyncKeyState which does what I'm looking for
        // although I'm using OSX and I don't think there's a easy way for this.

        if (user_input.joinable()) {
            user_input.detach();
        }

        if (game.joinable()) {
            game.join();
        }
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
