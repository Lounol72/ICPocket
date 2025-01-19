# Nom de l'exécutable
all: main

# Dossiers
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
OBJ_DIR = $(SRC_DIR)/obj

# Fichiers source et objets
SRCS = main.c $(SRC_DIR)/boutons.c $(SRC_DIR)/inputs.c $(SRC_DIR)/menu.c $(SRC_DIR)/utils.c $(SRC_DIR)/parametre.c $(SRC_DIR)/state.c
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Compilateur et options de compilation
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra -std=c11 `sdl2-config --cflags` -I/usr/include/SDL2

# Bibliothèques à lier
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Règle pour créer l'exécutable
main: $(OBJ_DIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
	@echo "Compilation terminée"

# Règle pour créer le dossier obj
$(OBJ_DIR):
	mkdir -p $@

# Règles pour compiler les fichiers source en objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -rf $(OBJ_DIR) main
	@echo "Nettoyage terminé"

.PHONY: all clean
