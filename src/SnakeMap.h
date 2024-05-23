#ifndef _SNAKE_MAP_
#define _SNAKE_MAP

#include <fstream>
#include <ncurses.h>

class MapHandler 
{
public:
    MapHandler();

public:
    //Getter Fn
    char **getMap() const;
    int getMaxHeight() const;
    int getMaxWidth()  const;

    //Member Fn
    void importLevel(int level);
    void putItem(const int Y, const int X, const char ITEM);
    bool isWall(const int Y, const int X) const;

private:
    char **map;
    int maxHeight;
    int maxWidth;

private:
    void resetMap();
    void loadMap(std::ifstream &levelIn);
    void createMap();
};

#endif 