#include <vector>
#include <ncurses.h>

#ifndef SNAKEGAME_H
#define SNAKEGAME_H

struct Position
{
    int x, y;
    // constructor
    Position();
    Position(int row, int col);
};

class SnakeGame
{
private:
    int speed;
    int maxWidth, maxHeight;
    char snakeChar;
    char direction; // direction of snake head
    std::vector<Position> snake;

private:
    void init();
    void drawSnake();
    
    void snakeMove();
    bool gameOver();

public:
    SnakeGame(int level);   // constructor
    virtual ~SnakeGame();   // destructor
    void startGame();
};

#endif