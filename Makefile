# Répertoires
SRC_DIR      = src
OBJ_DIR      = obj
BIN_DIR      = $(SRC_DIR)/bin
INCLUDE_DIR  = include

# Noms des exécutables
MAIN_EXE  = main
DUEL_EXE  = duel
SAVE_EXE  = save

# Fichiers source
SRCS      = main.c $(SRC_DIR)/GameEngine.c $(SRC_DIR)/Buttons.c $(SRC_DIR)/structPoke.c $(SRC_DIR)/duel.c $(SRC_DIR)/trainerAI.c $(SRC_DIR)/save.c $(SRC_DIR)/interDuel.c $(SRC_DIR)/Log.c $(SRC_DIR)/Audio.c $(SRC_DIR)/Window.c $(SRC_DIR)/Text.c $(SRC_DIR)/Game.c $(SRC_DIR)/Events.c

# Fichiers objets généraux
OBJS      = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Fichiers objets spécifiques pour duel et save
DUEL_OBJS = $(OBJ_DIR)/mainDuel.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/interDuel.o $(OBJ_DIR)/save.o
SAVE_OBJS = $(OBJ_DIR)/save.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/ministdlib.o

# Compilateur et options
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g `sdl2-config --cflags` -I/usr/include/SDL2

# Bibliothèques à lier
LIBS    = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Cibles principales
all: $(MAIN_EXE) $(DUEL_EXE)

$(MAIN_EXE): $(OBJ_DIR)/main.o $(OBJS)
	$(CC) -o $@ $^ $(LIBS)
	@echo "Compilation terminée"

$(DUEL_EXE): $(DUEL_OBJS)
	$(CC) $^ -o $(BIN_DIR)/$@
	@echo "Compilation duel terminée"

$(SAVE_EXE): $(SAVE_OBJS)
	$(CC) $^ -o $(BIN_DIR)/$@
	@echo "Compilation save terminée"

# Création des répertoires
$(OBJ_DIR):
	mkdir -p $@
	mkdir -p $(BIN_DIR)

# Règles de compilation génériques pour les sources situés dans SRC_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle spécifique pour main.c
$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Cibles de tests
testDuel: $(DUEL_EXE)
	./$(BIN_DIR)/$(DUEL_EXE)

testMain: $(MAIN_EXE)
	./$(MAIN_EXE)

testValgrind: $(MAIN_EXE)
	valgrind --leak-check=full ./$(MAIN_EXE)

# Cible de nettoyage
clean:
	rm -f $(OBJ_DIR)/*.o $(MAIN_EXE)
	rm -f $(BIN_DIR)/$(DUEL_EXE) $(BIN_DIR)/$(SAVE_EXE)
	@echo "Nettoyage terminé"

.PHONY: all clean