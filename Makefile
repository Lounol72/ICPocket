# Nom de l'exécutable
all: main

# Dossiers
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
OBJ_DIR = $(SRC_DIR)/obj

# Fichiers source et objets
SRCS = main.c $(SRC_DIR)/boutons.c $(SRC_DIR)/inputs.c $(SRC_DIR)/menu.c $(SRC_DIR)/utils.c $(SRC_DIR)/parametre.c
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/boutons.o $(OBJ_DIR)/inputs.o $(OBJ_DIR)/menu.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/parametre.o

# Compilateur et options de compilation
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -std=c11 `sdl2-config --cflags` -I/usr/include/SDL2

# Bibliothèques à lier
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Règle pour créer l'exécutable
main: $(OBJ_DIR) $(OBJS)
	$(CC) -o main $(OBJS) $(LIBS)

# Règle pour créer le dossier obj
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Règles pour compiler les fichiers source en objets
$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/boutons.o: $(SRC_DIR)/boutons.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/boutons.c -o $(OBJ_DIR)/boutons.o

$(OBJ_DIR)/inputs.o: $(SRC_DIR)/inputs.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/inputs.c -o $(OBJ_DIR)/inputs.o

$(OBJ_DIR)/menu.o: $(SRC_DIR)/menu.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/menu.c -o $(OBJ_DIR)/menu.o

$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/utils.c -o $(OBJ_DIR)/utils.o

$(OBJ_DIR)/parametre.o: $(SRC_DIR)/parametre.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/parametre.c -o $(OBJ_DIR)/parametre.o

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f main $(OBJ_DIR)/*.o

.PHONY: all clean
