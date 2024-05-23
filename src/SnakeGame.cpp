#include "SnakeGame.h"
#include <unistd.h>

int key_pressed;

Position::Position()
{
    x = 0;
    y = 0;
}

Position::Position(int col, int row)
{
    x = col;    // column
    y = row;    // row
}

/* initialize the window */
void SnakeGame::init()
{
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);   
    noecho();   // user input x
    curs_set(0);    // cursor x
    getmaxyx(stdscr, maxHeight, maxWidth);

    //Set colors
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE); //Wall, Immune Wall
    init_pair(2, COLOR_CYAN, COLOR_CYAN); //Snake Head, Body
    init_pair(3, COLOR_GREEN, COLOR_GREEN); //Growth
    init_pair(4, COLOR_RED, COLOR_RED); //Poison
    init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA); //Gate
}

/* draw a snake on the map */
void SnakeGame::drawSnake()
{
    for (int i = 0; i < 3; i++)
    {
        snake.push_back(Position(8 - i, 1));
    }

    for (int i = 0; i < snake.size(); i++)
    {
        move(snake[i].y, snake[i].x);
        addch(snakeChar);
    }
    return;
}

/* movement of the snake */
void SnakeGame::snakeMove()
{
    key_pressed = getch();
    switch(key_pressed)
    {
    case KEY_LEFT:
        if(direction != 'r')
            direction = 'l';
        else
            gameOver();
        break;
    case KEY_RIGHT:
        if(direction != 'l')
            direction = 'r';
        else
            gameOver();
        break;
    case KEY_UP:
        if(direction != 'd')
            direction = 'u';
        else
            gameOver();
        break;
    case KEY_DOWN:
        if(direction != 'u')
            direction = 'd';
        else
            gameOver();
        break;
    }
    
    //remove tail
    move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
    printw(" ");
    refresh();
    snake.pop_back();
    
    // add to head
    if (direction == 'l')
        snake.insert(snake.begin(), Position(snake[0].x - 1, snake[0].y));
    else if (direction == 'r')
        snake.insert(snake.begin(), Position(snake[0].x + 1, snake[0].y));
    else if (direction == 'u')
        snake.insert(snake.begin(), Position(snake[0].x, snake[0].y - 1));
    else if (direction == 'd')
        snake.insert(snake.begin(), Position(snake[0].x, snake[0].y + 1));
    move(snake[0].y, snake[0].x);
    addch(snakeChar);
    refresh();
}

/* cases to fail the game */
bool SnakeGame::gameOver()
{
    /* press opposite key */
    if (direction == 'r' && key_pressed == KEY_LEFT)    { return true; }
    if (direction == 'l' && key_pressed == KEY_RIGHT)   { return true; }
    if (direction == 'u' && key_pressed == KEY_DOWN)    { return true; }
    if (direction == 'd' && key_pressed == KEY_UP)      { return true; }

    // snake head meets Wall
    if (m.isWall(snake[0].y, snake[0].x))               { return true; }

    return false;
}

SnakeGame::SnakeGame(int level)
{
    speed = 500000; // 0.5sec
    snakeChar = '#';
    direction = 'r'; // right
    
    m.importLevel(level);
    init();
    drawMap();
    drawSnake();
    refresh();
}

SnakeGame::~SnakeGame()
{
    nodelay(stdscr, false);
    getch();
    endwin();
}

void SnakeGame::startGame()
{
    while(true)
    {
        if(gameOver())
        {
            move((maxHeight - 2) / 2, (maxWidth - 22) / 2);
            printw("GAME OVER");
            endwin();
            break;
        }
        snakeMove();
        usleep(speed); // delay
    }
}

//draw game map
void SnakeGame::drawMap() {
    char **map{m.getMap()};
    for (int y{}; y < m.getMaxHeight(); y++) {
        for (int x{}; x < m.getMaxWidth(); x++) {
            move(y, x);
            int color;
            switch (map[y][x])
            {
            case '0':
                printw(" ");
                break;
            case '1': //Wall
                attron(COLOR_PAIR(1));
                printw("_");
                attroff(COLOR_PAIR(1));
                break;
            case '2': //Immune Wall
                attron(COLOR_PAIR(1));
                printw("_");
                attroff(COLOR_PAIR(1));
                break;
            case '3': //Growth
                attron(COLOR_PAIR(3));
                printw("_");
                attroff(COLOR_PAIR(3));
                break;
            case '4': //Poison
                attron(COLOR_PAIR(4));
                printw("_");
                attroff(COLOR_PAIR(4));
                break;
            case '5': //Gate
                attron(COLOR_PAIR(5));
                printw("_");
                attroff(COLOR_PAIR(5));
                break;
            }
        }
    }
}
