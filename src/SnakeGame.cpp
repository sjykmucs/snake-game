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
    init_pair(5, COLOR_BLUE, COLOR_MAGENTA); //Gate
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
    speed = 200000 / level; // level up -> speed up //임시
    snakeChar = '#';
    direction = 'r'; // right
    item_timer = 0;
    gateOpen = false;

    max_len = 3;
    miss_len = 3 + level * 2;
    score_growth = 0;
    miss_growth = level * 3;
    score_poison = 0;
    miss_poison = level;
    score_gate = 0;
    miss_gate = level * 2;

    m.importLevel(level);
    init();
    drawMap();
    drawSnake();
    scoreBoard();
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
        checkItem();
        updateItem();
        drawMap();
        scoreBoard();

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
                break;
            case '1': //Wall
                attron(COLOR_PAIR(1)); attron(A_INVIS);
                addch('_');
                attroff(COLOR_PAIR(1)); attroff(A_INVIS);
                break;
            case '2': //Immune Wall
                attron(COLOR_PAIR(1)); attron(A_INVIS);
                addch('_');
                attroff(COLOR_PAIR(1)); attroff(A_INVIS);
                break;
            case '3': //Growth
                attron(COLOR_PAIR(3));
                addch('_');
                attroff(COLOR_PAIR(3));
                break;
            case '4': //Poison
                attron(COLOR_PAIR(4));
                addch('_');
                attroff(COLOR_PAIR(4));
                break;
            case '5': //Gate
                attron(COLOR_PAIR(5)); 
                addch('0');
                attroff(COLOR_PAIR(5));
                break;
            }
        }
    }
}
void SnakeGame::checkItem() {
    // check snake tail
    char item = m.getItem(snake.back().y, snake.back().x);
    if (item == '5') gateOpen = false;

    // check snake head
    item = m.getItem(snake[0].y, snake[0].x);
    switch (item)
    {
    case '3': //Growth
        break;
    case '4': //Poison
        break;
    case '5': //Gate
        score_gate++;

        Gate in = m.getGate(snake[0].y, snake[0].x);
        snake[0].y = in.out->y;
        snake[0].x = in.out->x;
        gateOpen = true;

        char UP = m.getItem(snake[0].y - 1, snake[0].x);
        char RI = m.getItem(snake[0].y, snake[0].x + 1);
        char DO = m.getItem(snake[0].y + 1, snake[0].x);
        char LE = m.getItem(snake[0].y, snake[0].x - 1);


        switch (direction)
        {
        case 'u':
            if (UP != '1' && UP != '5' && UP != '2') direction = 'u';
            else if (RI != '1' && RI != '5' && RI != '2') direction = 'r';
            else if (LE != '1' && LE != '5' && LE != '2') direction = 'l';
            else if (DO != '1' && DO != '5' && DO != '2') direction = 'd';
            break;
        case 'r':
            if (RI != '1' && RI != '5' && RI != '2') direction = 'r';
            else if (DO != '1' && DO != '5' && DO != '2') direction = 'd';
            else if (UP != '1' && UP != '5' && UP != '2') direction = 'u';
            else if (LE != '1' && LE != '5' && LE != '2') direction = 'l';
            break;
        case 'l':
            if (LE != '1' && LE != '5' && LE != '2') direction = 'l';
            else if (UP != '1' && UP != '5' && UP != '2') direction = 'u';
            else if (DO != '1' && DO != '5' && DO != '2') direction = 'd';
            else if (RI != '1' && RI != '5' && RI != '2') direction = 'r';
            break;
        case 'd':
            if (DO != '1' && DO != '5' && DO != '2') direction = 'd';
            else if (LE != '1' && LE != '5' && LE != '2') direction = 'l';
            else if (RI != '1' && RI != '5' && RI != '2') direction = 'r';
            else if (UP != '1' && UP != '5' && UP != '2') direction = 'u';
            break;
        }
        break;
    }
}
void SnakeGame::updateItem() {
    if (!(gateOpen) && (item_timer % 20) == 0) m.makeGates();


    item_timer++;

}

void SnakeGame::scoreBoard()
{   
    move(1, maxWidth - 20);
    printw("*------------------*");
    move(3, maxWidth - 20);
    printw(" Score Board");
    move(5, maxWidth - 20);
    printw(" B : %d / %d", snake.size(), max_len);
    move(6, maxWidth - 20);
    printw(" + : %d", score_growth);
    move(7, maxWidth - 20);
    printw(" - : %d", score_poison);
    move(8, maxWidth - 20);
    printw(" G : %d", score_gate);
    move(10, maxWidth - 20);
    printw("*------------------*");
    
    move(11, maxWidth - 20);
    printw("*------------------*");
    move(13, maxWidth - 20);
    printw(" Mission");

    move(15, maxWidth - 20);
    if(max_len >= miss_len)
        printw(" B : %d (v)", miss_len);
    else
        printw(" B : %d ( )", miss_len);
    
    move(16, maxWidth - 20);
    if(score_growth >= miss_growth)
        printw(" + : %d (v)", miss_growth);
    else
        printw(" + : %d ( )", miss_growth);

    move(17, maxWidth - 20);
    if(score_poison >= miss_poison)
        printw(" - : %d (v)", miss_poison);
    else
        printw(" - : %d ( )", miss_poison);

    move(18, maxWidth - 20);
    if(score_gate >= miss_gate)
        printw(" G : %d (v)", miss_gate);
    else
        printw(" G : %d ( )", miss_gate);

    move(20, maxWidth - 20);
    printw("*------------------*");
}
