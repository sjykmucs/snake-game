#include "SnakeGame.h"
#include <unistd.h>

int maxHeight, maxWidth;
void clearScreen(float x, float y);
char ready();
char again(bool clear);
char gameClear();
char input();

int main()
{
    bool clear;
    int level = 1;

    if(ready() == 'y')
    {
        do
        {
            SnakeGame s(level++);
            s.startGame();
            clear = s.stageClear();
            
            if(!clear) {level = 1;}
            
        } while (again(clear) == 'y' && level <= 4);

        if(level == 5)
        {
            gameClear();
        }
    }
    return 0;
}

/* clear the screen */
void clearScreen(float x, float y)
{
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, maxHeight, maxWidth);
    move((maxHeight / y), (maxWidth / x));
}

/* print start screen */
char ready()
{
    clearScreen(3.8, 2.2);
    printw("Welcome to the game! Do you want to start the game? (y/n)");
    return input();
}

/* next stage or play again */
char again(bool clear)
{
    if (clear)
    {
        clearScreen(3.8, 2.2);
        printw("Stage Clear! Do you want to go to the next stage? (y/n)");
    }
    else
    {
        clearScreen(3.8, 2.2);
        printw("Game Over. Do you want to play again? (y/n)");
    }
    return input();
}

char gameClear()
{
    clearScreen(3.8, 2.2);
    printw("You cleared all the stage! Congratulation!");
    return input();
}

/* user input */
char input()
{
    char input = getch();
    refresh();
    endwin();
    clear();

    return input;
}
