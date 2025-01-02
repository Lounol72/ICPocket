all:
	gcc -Isrc/Include -o main main.c boutons.c inputs.c menu.c utils.c -lSDL2 -lSDL2_image -lSDL2_mixer
