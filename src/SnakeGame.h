#include <vector>
#include <ncurses.h>

#include "SnakeMap.h"

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
    int item_timer;
    char snakeChar;
    char direction; // direction of snake head
    std::vector<Position> snake;

    bool gateOpen;
    MapHandler m;

private:
    void init();
    void drawSnake();
    void drawMap();
    void snakeMove();
    bool gameOver();

    void checkItem();
    void updateItem();
public:
    SnakeGame(int level);   // constructor
    virtual ~SnakeGame();   // destructor
    void startGame();
};

#endif