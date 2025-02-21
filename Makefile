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
SRCS      = main.c $(filter-out $(SRC_DIR)/mainDuel.c $(SRC_DIR)/ministdlib.c, $(wildcard $(SRC_DIR)/*.c))

# Fichiers objets généraux
OBJS      = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Fichiers objets spécifiques pour duel et save
DUEL_OBJS = $(OBJ_DIR)/mainDuel.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/interDuel.o $(OBJ_DIR)/save.o
SAVE_OBJS = $(OBJ_DIR)/save.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/ministdlib.o

# Compilateur et options
CC        = gcc
CFLAGS    = -Wall -Wextra -Werror -std=c11 -g `sdl2-config --cflags` -I/usr/include/SDL2

# -Wall : affiche tous les warnings
# -Wextra : affiche des warnings supplémentaires
# -Werror : transforme les warnings en erreurs
# -std=c11 : utilise le standard C11
# -g : ajoute des informations de débogage
# `sdl2-config --cflags` : ajoute les flags de compilation pour SDL2
# -I/usr/include/SDL2 : ajoute le répertoire d'inclusion de SDL2

# Bibliothèques à lier
LIBS      = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Cibles principales
all: $(MAIN_EXE) $(DUEL_EXE) 

$(MAIN_EXE): $(OBJ_DIR)/main.o $(OBJS)
	@$(CC) -o $(BIN_DIR)/$@ $^ $(LIBS)
	@echo "✅ Compilation terminée"

$(DUEL_EXE): $(DUEL_OBJS)
	@$(CC) $^ -o $(BIN_DIR)/$@
	@echo "✅ Compilation duel terminée"

$(SAVE_EXE): $(SAVE_OBJS)
	@$(CC) $^ -o $(BIN_DIR)/$@
	@echo "✅ Compilation save terminée"

# Création des répertoires
$(OBJ_DIR):
	mkdir -p $@
	mkdir -p $(BIN_DIR)

# Règles de compilation génériques pour les sources situés dans SRC_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Règle spécifique pour main.c
$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	@echo "🛠️ Compilation en cours..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Cibles de tests
testDuel: $(DUEL_EXE)
	./$(BIN_DIR)/$(DUEL_EXE)

testMain: $(MAIN_EXE)
	./$(BIN_DIR)/$(MAIN_EXE)

testValgrind: $(MAIN_EXE)
	valgrind --leak-check=full ./$(MAIN_EXE)

# Cible de nettoyage
clean:
	@echo "🧹 Nettoyage en cours..."
	@rm -f $(OBJ_DIR)/*.o
	@rm -f $(BIN_DIR)/*.o
	@echo "✅ Nettoyage terminé"

.PHONY: all clean