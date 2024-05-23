run:
	g++ -std=c++11 -o snake src/SnakeGame.cpp src/main.cpp src/SnakeMap.cpp -lncurses
	./snake