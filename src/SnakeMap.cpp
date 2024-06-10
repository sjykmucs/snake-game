#include "SnakeMap.h"
#include <iostream>

MapHandler::MapHandler() {
    map = {nullptr};
    g1 = Gate();
    g2 = Gate();
    Growth = item();
    Poison = item();
    Speed = item();
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
    if (Y >= 0 && Y < maxHeight && X >= 0 && X < maxWidth) {
        return map[Y][X] == '1'|| map[Y][X] == '2';
    } 
    return true;
}

void MapHandler::putItem(const int Y, const int X,const char ITEM) {
    if (Y >= 0 && Y < maxHeight && X >= 0 && X < maxWidth) {
        map[Y][X] = ITEM;
    }  
}
char MapHandler::getItem(const int Y, const int X) {
    if (Y >= 0 && Y < maxHeight && X >= 0 && X < maxWidth) {
        return map[Y][X];
    }
    else return '1'; //Invaild Position
}

//무작위 게이트 생성
Gate MapHandler::getRandomGate() {
    std::random_device rd;
    std::mt19937 mt_gate(rd());

    std::uniform_int_distribution<int> gateYDist(0, maxHeight - 1);
    int y {gateYDist(mt_gate)};
    int i {0};
    for (int r{}; r < maxWidth; r++) {
        if (map[y][r] == '1') i++;
    }

    std::uniform_int_distribution<int> gateXDist(0, i - 1);
    int j {gateXDist(mt_gate)};
    int x {};
    while (j >= 0) {
        if (map[y][x] == '1') j--;
        x++;
    }
    x--;

    Gate g{y, x, nullptr};
    return g;
}

//
void MapHandler::makeGates() {
    if (g1.out != nullptr) {
        putItem(g1.y, g1.x, '1');
        putItem(g2.y, g2.x, '1');
    }

    g1 = getRandomGate();
    g2 = getRandomGate();
    while (g1.y == g2.y && g1.x == g2.x) g2 = getRandomGate();

    g1.out = &g2;
    g2.out = &g1;

    putItem(g1.y, g1.x, '5');
    putItem(g2.y, g2.x, '5');
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
//특정 위치의 게이트 반환
Gate MapHandler::getGate(const int Y, const int X) {
    if (Y == g1.y && X == g1.x) return g1;
    else if(Y == g2.y && X == g2.x) return g2;
    return Gate{0,0,nullptr}; //No such gate
}

MapHandler::~MapHandler() {
    resetMap();
}
// 무작위 아이템 위치 생성
item MapHandler::getRandomitems() {
    std::random_device rd;
    std::mt19937 mt_item(rd());

    std::uniform_int_distribution<int> itemY(1, maxHeight - 2);
    std::uniform_int_distribution<int> itemX(1, maxHeight - 2);
    
    while(true){
    int y {itemY(mt_item)};
    int x {itemX(mt_item)};
   if(map[y][x]=='0')  //check wall,item
   {
    return item{y,x};
   }
    }
}
// 아이템 배치 1->Growth 2-> Poison  3->Speed up
void MapHandler::makeitems(int a) {
    switch (a)
    {
    case 1:
        putItem(Growth.y, Growth.x, '0');
        Growth = getRandomitems();
        putItem(Growth.y, Growth.x, '3');
        break;
    case 2:
        putItem(Poison.y, Poison.x, '0');
        Poison = getRandomitems();
        putItem(Poison.y, Poison.x, '4');
        break;
    case 3:
        putItem(Speed.y, Speed.x, '0');
        Speed = getRandomitems();
        putItem(Speed.y, Speed.x, '6');
        break;
    }
}

