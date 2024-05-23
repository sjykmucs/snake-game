#include "SnakeMap.h"
#include <iostream>

MapHandler::MapHandler() {
    map = {nullptr};
}

//map 디렉토리에서 해당하는 레벨의 파일을 가져온다.
void MapHandler::importLevel(int level) {
    std::ifstream levelIn;
    switch (level)
    {
    case 1:
        levelIn.open("./map/level_1.txt");
        break;
    case 2:
        levelIn.open("./map/level_2.txt");
        break;
    case 3:
        levelIn.open("./map/level_3.txt");
        break;
    }
    if (levelIn.fail()) return createMap();
    
    loadMap(levelIn);
    levelIn.close();
}

//레벨 파일을 기반으로 맵을 만든다.
void MapHandler::loadMap(std::ifstream &levelIn) {
    resetMap();
    levelIn >> maxHeight >> maxWidth;
    map = new char *[maxHeight];
    for (int y{}; y < maxHeight; y++) 
        map[y] = new char [maxWidth];

    char data;
    for (int y{}; y < maxHeight; y++) {
    for (int x{}; x < maxWidth ; x++) {
        data = levelIn.get();

        if (data == '\n') x--;
        else map[y][x] = data;
        }
    }
}

//기존 맵을 삭제한다.
void MapHandler::resetMap() {
    if (map) {
        for (int y{}; y < maxHeight; y++) {
            delete[] map[y];
        }
        delete[] map;
    }
}

//레벨 파일이 없는 경우 기본 맵을 만든다.
void MapHandler::createMap() {
    resetMap();

    maxHeight = 21; maxWidth = 21;
    map = new char *[maxHeight];
    
    map[0] = new char[maxWidth]{'2','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','2'};
    map[20] = new char[maxWidth]{'2','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','2'};
    for (int y{1}; y < 20; y++)
        map[y] = new char[maxWidth]{'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'};
}

bool MapHandler::isWall(const int Y, const int X) const {
    return (map[Y][X] == '1');
}

void MapHandler::putItem(const int Y, const int X,const char ITEM) {
    if (Y > 0 && Y < maxHeight && X > 0 && X < maxWidth) {
        map[Y][X] = ITEM;
    }  
}

//Getter Fn
char **MapHandler::getMap() const {
    return map;
}
int MapHandler::getMaxHeight() const {
    return maxHeight;
}
int MapHandler::getMaxWidth() const {
    return maxWidth;
}




