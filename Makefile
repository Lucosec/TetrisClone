all:
	g++ *.cpp -o bin/windows/tetris.exe -O2 -Wall -Wno-missing-braces -Iinclude -Llib -lraylib

