#include <ncurses.h>
#include <iostream>
#include <list>

using namespace std;

WINDOW *screen;
int screenWidth = 120;
int screenHeight = 40;

int foodX = 10;
int foodY = 10;
int score = 0;
bool snakeDead = false;
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

void screenSetup() {
    initscr();
    
    screen = newwin(screenHeight, screenWidth, 0, 0);
    noecho();
    refresh();

    box(screen, 0, 0);
    wrefresh(screen);
}

void updateScreen(const char *info, int x, int y) {
    mvprintw(y, x, info);
    wrefresh(screen);
}

int main() {
    screenSetup();

    updateScreen("[ SNAKE ]", (screenWidth/2), 0);

    while(!snakeDead) {
      // Timing and Input

      // Game Logic

      // Display to player

        // for(std::list<SnakeSegment>::iterator it = snake.begin(); it != snake.end(); it ++) {
        //     it
        // }

        for (auto s : snake) {
            // Snake body
            updateScreen((snakeDead ? "+" : "O"), s.x, s.y);
            
            // Snake head
            updateScreen((snakeDead ? "X" : "@"), snake.front().x , snake.front().y);

            // Fruit
            updateScreen("F", foodX, foodY);
        }  

        getch();
        snakeDead = true;
    }

    delwin(screen);
    endwin();
    return 0;
}
