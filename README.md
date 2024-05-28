## SnakeMap.h
스네이크 게임의 맵을 다루는 코드입니다. 이 코드는 스네이크 게임의 맵을 입력된 레벨에 따라 불러 오고, SnakeGame.cpp에서 맵을 표시, 다른 아이템과 스네이크간에 상호 작용에 필요한 기능을 제공합니다. 

SnakeMap.h에서는 ./map에서 txt파일을 불러와 게임의 맵을 만듭니다. txt파일은 다음 규칙을 따릅니다.  
1. 첫 줄에 전체 맵의 크기가 있습니다. 세로줄Y, 가로줄 X 순서로 공백 문자로 구분되어 있습니다.
2. 다음 줄부터 한 줄씩 Y개의 데이터가 있습니다.각 데이터는 X개의 문자가 공백 구분없이 있습니다. 
3. 각 문자는 '2', '1', '0' 중 하나로 Immune Wall, Wall, Empty를 나타냅니다.
<hr>

게이트에 관한 정보는 Gate struct으로 저장 됩니다. Gate는 다음의 변수는 다음과 같습니다. 
|이름|자료형|설명
|---|----|---|
|y|int |게이트의 y좌표를 나타냅니다.|
|x|int |게이트의 y좌표를 나타냅니다.|
|out|Gate*|게이트의 출구 게이트를 가리키는 포인터 변수입니다.|

기능은 MapHandler class를 통해 제공되며 설명은 다음과 같습니다.  
1. 맴버 변수 

|이름|자료형|설명|관련 기능|
|---|---|---|----|
|map|char**|동적 할당된 2차원 배열 데이터를 가리키는 이중 포인터 변수 입니다. 배열의 값은 해당 위치에 데이터(Wall, Immune Wall, Empty 등)를 나타냅니다.|importLevel(int), getMap()|
|maxHeight|int|map의 높이를 나타내는 변수입니다.|getMaxHeight()|
|maxWidth|int|map의 넓이를 나타내는 변수입니다.|getMaxWidth()|
||

2. 맴버 함수 

|접근|이름|리턴|설명|관련 기능|
|---|----|-----|------|-------|
|public|importLevel(int)|void|매개변수에 따라 ./map주소에 txt파일을 불러와 map 변수에 할당합니다. 할당된 값은 char을 가지는 이차원 배열입니다. txt파일 불러오기에 실패시 21x21크기의 배열을 만들어 냅니다.|createMap(), loadMap(), resetMap()|
|public|putItem(int, int, char)|void|매개 변수에 따라 map의 특정 위치에 char값을 변경합니다. **입력되는 위치는 Y, X 순서입니다**||
|public|makeGates()|void|무작위 위치에 게이트를 만들어 냅니다. 이미 게이트가 있다면 해당 게이트를 없애고 다시 생성합니다.|getRandomGate()|
|public|isWall(int, int)|bool|매개변수에 따라 map의 해당 위치가 Wall이면 true를 리턴합니다. 맵밖으로 나간 경우도 Wall로 판단합니다. **입력되는 위치는 Y, X 순서입니다**||
|public|getMap()|char**|map 변수를 리턴합니다.||
|public|getMaxHeight()|int|maxHeight를 리턴합니다.||
|public|getMaxWidth()|int|maxWidth를 리턴합니다.||
|public|getItem(int, int)|char|매개 변수에 따라 map의 특정 위치의 char 값을 리턴합니다. 맵 밖으로 나간 경우는 Wall로 판단합니다. **입력되는 위치는 Y, X 순서입니다.**||
|public|getGate(int, int)|Gate|매개 변수에 따라 map의 특정 위치의 Gate을 리턴합니다. 해당 위치에 게이트가 없는 경우 0인 Gate를 리턴합니다.**입력되는 위치는 Y, X 순서입니다.**||
||
|private|resetMap()|void|map에 할당된 배열을 삭제합니다.||
|private|loadMap()|void|importLevel()에 의해서 호출되어 txt파일을 map에 저장합니다.|importLevel(int)|
|private|createMap()|void|importLevel()에서 txt파일 불러오기가 실패할 경우, 호출되는 함수 입니다. 21x21 크기의 맵을 만듭니다.|importLevel(int)|
|private|Gate getRandomGate()|Gate|무작위 위치를 가진 게이트를 만들어 리턴합니다. 만들어지는 게이트는 게임 룰을 따릅니다.||
||



