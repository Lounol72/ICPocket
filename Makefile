# Répertoires
SRC_DIR      = src
OBJ_DIR      = obj
BIN_DIR      = bin
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
CFLAGS    = -Wall -Wextra -Werror -std=c11 -g `sdl2-config --cflags` -I/usr/include/SDL2 -D_POSIX_C_SOURCE=200809L

# -Wall : affiche tous les warnings
# -Wextra : affiche des warnings supplémentaires
# -Werror : transforme les warnings en erreurs
# -std=c11 : utilise le standard C11
# -g : ajoute des informations de débogage
# `sdl2-config --cflags` : ajoute les flags de compilation pour SDL2
# -I/usr/include/SDL2 : ajoute le répertoire d'inclusion de SDL2

# Bibliothèques à lier
LIBS      = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread

# Compilateur et options Windows
WINCC     = x86_64-w64-mingw32-gcc
WINCFLAGS = -Wall -Wextra -std=c11 -g \
           -I$(INCLUDE_DIR) \
           -I/usr/x86_64-w64-mingw32/include \
           -I/usr/x86_64-w64-mingw32/include/SDL2 \
           -D_REENTRANT \
           -DSDL_MAIN_HANDLED

WINLIBS   = -L/usr/x86_64-w64-mingw32/lib \
            -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread

# Fichiers objets Windows
WIN_OBJS  = $(patsubst %.c,$(OBJ_DIR)/%_win.o,$(notdir $(SRCS)))

# Cibles principales
all: 
	@clear
	@mkdir -p $(BIN_DIR)
	@$(MAKE) $(MAIN_EXE) $(DUEL_EXE) 

$(MAIN_EXE): $(OBJS)
	@echo "🛠️ Compilation Linux en cours..."
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
	@mkdir -p $@
	@mkdir -p $(BIN_DIR)

# Règles de compilation Linux
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	@echo "🛠️ Compilation en cours..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Règles de compilation Windows
$(OBJ_DIR)/%_win.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(WINCC) $(WINCFLAGS) -c $< -o $@

$(OBJ_DIR)/main_win.o: main.c | $(OBJ_DIR)
	@$(WINCC) $(WINCFLAGS) -c $< -o $@

# Cibles de tests
testDuel: $(DUEL_EXE)
	@echo "🚀 Lancement du programme duel..."
	@./$(BIN_DIR)/$(DUEL_EXE)

run: $(MAIN_EXE)
	@echo "🚀 Lancement du programme main..."
	@./$(BIN_DIR)/$(MAIN_EXE)
	@echo "✅ Main terminé"

testValgrind: $(MAIN_EXE)
	@echo "🚀 Lancement de Valgrind..."
	@valgrind --leak-check=full ./$(BIN_DIR)/$(MAIN_EXE)

testMain-debug: $(MAIN_EXE)
	@echo "🚀 Lancement de Valgrind..."
	@./$(BIN_DIR)/$(MAIN_EXE) -debug

rebuild: clean main
	@clear
	@$(MAKE) $(MAIN_EXE) $(DUEL_EXE) 

# Cible de nettoyage
clean:
	@echo "🧹 Nettoyage en cours..."
	@rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*
	@echo "✅ Nettoyage terminé"

package-linux:
	@echo "📦 Création du package linux..."
	@mkdir -p $(BIN_DIR)/libs >/dev/null 2>&1
	@ldd $(BIN_DIR)/$(MAIN_EXE) | awk '{print $$3}' | grep -v "ld-linux" | grep -v "linux-vdso.so" | xargs -I{} cp -v {} $(BIN_DIR)/libs/ >/dev/null 2>&1 || true
	@echo '#!/bin/bash' > run.sh
	@echo 'cd "$(dirname "$$0")"' >> run.sh
	@echo 'export LD_LIBRARY_PATH=$$(pwd)/$(BIN_DIR)/libs' >> run.sh
	@echo "./$(BIN_DIR)/$(MAIN_EXE) \"$$@\"" >> run.sh
	@chmod -R 775 $(BIN_DIR) >/dev/null 2>&1
	@chmod -R 775 assets >/dev/null 2>&1
	@chmod -R 777 data >/dev/null 2>&1
	@chmod +x run.sh
	@chmod +x $(BIN_DIR)/$(MAIN_EXE)
	@tar -czf ICPocket.tar.gz run.sh $(BIN_DIR) assets data README.md >/dev/null 2>&1 || true
	@echo "✅ Package linux créé : ICPocket.tar.gz"

doxygen:
	@echo "🧹 Nettoyage en cours..."
	@rm -rf docs/html
	@doxygen Doxyfile 
	@echo "✅ Documentation générée dans docs"

latex:
	@echo "🧹 Nettoyage en cours..."
	@find rapport -type f ! -name "*.tex" ! -name "*.png" -delete
	@cd rapport && pdflatex modele.tex
	@cd rapport && makeglossaries modele
	@cd rapport && pdflatex modele.tex
	@echo "✅ Documentation générée dans docs/tpLatex/modele.pdf"


.PHONY: all clean windows package-windows 