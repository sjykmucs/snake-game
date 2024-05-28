#ifndef _SNAKE_MAP_
#define _SNAKE_MAP_

#include <fstream>
#include <ncurses.h>
#include <random>

struct Gate 
{
    int y, x;
    Gate*out;
};

class MapHandler 
{
public:
    MapHandler();
    virtual ~MapHandler();

public:
    //Getter Fn
    char **getMap() const;
    int getMaxHeight() const;
    int getMaxWidth()  const;

    //Member Fn
    void importLevel(int level);
    void putItem(const int Y, const int X, const char ITEM);
    char getItem(const int Y, const int X);
    bool isWall(const int Y, const int X) const;

    void makeGates();
    Gate getGate(const int Y, const int X);

private:
    char **map;
    int maxHeight;
    int maxWidth;
    Gate g1, g2;
    
private:
    void resetMap();
    void loadMap(std::ifstream &levelIn);
    void createMap();
    Gate getRandomGate();
};

#endif 