all:
	gcc -Isrc/Include -Lsrc/lib -o main main.c  -lSDL2
