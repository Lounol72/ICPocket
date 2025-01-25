# Nom de l'exécutable
all: main duel

# Dossiers
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
OBJ_DIR = $(SRC_DIR)/obj

# Fichiers source et objets
SRCS = main.c $(SRC_DIR)/GameEngine.c $(SRC_DIR)/Buttons.c
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g `sdl2-config --cflags` -I/usr/include/SDL2

# Bibliothèques à lier
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Règle pour créer l'exécutable
main: $(OBJ_DIR)/main.o $(OBJS)
	$(CC) -o $@ $^ $(LIBS)
	@echo "Compilation terminée"

# Règle temporaire pour créer duel
duel: $(OBJ_DIR)/duel.o
	$(CC) $^ -o $(SRC_DIR)/bin/$@

# Règle pour créer le dossier obj
$(OBJ_DIR):
	mkdir -p $@

# Règles pour compiler les fichiers source en objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Tester le projet

test: main
	./main

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ_DIR)/main.o main $(OBJ_DIR)/*.o
	@echo "Nettoyage terminé"

.PHONY: all clean