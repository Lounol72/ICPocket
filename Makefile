# Nom de l'exécutable
all: main duel

# Dossiers
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Fichiers source et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g `sdl2-config --cflags` -I/usr/include/SDL2
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Règle pour créer l'exécutable
main: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Compilation terminée"

# Règle temporaire pour créer duel
duel: $(OBJ_DIR)/mainDuel.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/interDuel.o $(OBJ_DIR)/save.o
	$(CC) $^ -o $(SRC_DIR)/bin/$@

save: $(OBJ_DIR)/save.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/ministdlib.o
	$(CC) $^ -o $(SRC_DIR)/bin/$@

# Règle pour créer le dossier obj
$(OBJ_DIR):
	mkdir -p $@

# Règles pour compiler les fichiers source en objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Tester le projet
testDuel: duel
	./$(SRC_DIR)/bin/duel

testMain: main
	./main

testValgrind:
	valgrind --leak-check=full ./main

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -rf $(OBJ_DIR) main
	@echo "Nettoyage terminé"

.PHONY: all clean