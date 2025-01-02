all:
	gcc -Isrc/Include -o main main.c boutons.c -lSDL2 -lSDL2_image
