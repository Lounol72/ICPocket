# Répertoires
SRC_DIR      = src
OBJ_DIR      = obj
BIN_DIR      = bin
INCLUDE_DIR  = include
TEST_DIR     = test
TEST_BIN_DIR = test/bin
TEST_OBJ_DIR = test/obj

# Noms des exécutables
MAIN_EXE     = main
DUEL_EXE     = duel
SAVE_EXE     = save

# Fichiers source et objets
SRCS         = main.c $(filter-out $(SRC_DIR)/mainDuel.c $(SRC_DIR)/ministdlib.c, $(wildcard $(SRC_DIR)/*.c))
OBJS         = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))
DUEL_OBJS    = $(OBJ_DIR)/mainDuel.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/interDuel.o $(OBJ_DIR)/save.o
SAVE_OBJS    = $(OBJ_DIR)/save.o $(OBJ_DIR)/structPoke.o $(OBJ_DIR)/duel.o $(OBJ_DIR)/trainerAI.o $(OBJ_DIR)/ministdlib.o

# Tests source et objets
TEST_SRCS    = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS    = $(patsubst $(TEST_DIR)/%.c,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))
TEST_BINS    = $(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%,$(TEST_SRCS))

# Compilateur et options
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -std=c11 -g `sdl2-config --cflags` -I/usr/include/SDL2 -D_POSIX_C_SOURCE=200809L
LIBS        = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread

# Trouver les drapeaux de compilation pour Check
CHECK_FLAGS := $(shell pkg-config --cflags check 2>/dev/null || echo "-I/usr/include")
CHECK_LIBS  := $(shell pkg-config --libs check 2>/dev/null || echo "-lcheck -lsubunit -lm -lpthread -lrt")

TEST_CFLAGS = $(CFLAGS) $(CHECK_FLAGS)
TEST_LIBS   = $(LIBS) $(CHECK_LIBS)

# Définition des cibles .PHONY
.PHONY: all clean rebuild run testDuel test test-memory unit-tests package-linux doxygen check-deps

# Cibles principales
all: check-deps $(BIN_DIR) $(TEST_BIN_DIR) $(TEST_OBJ_DIR) $(MAIN_EXE) $(DUEL_EXE) $(TEST_BINS)

# Vérification des dépendances
check-deps:
	@echo "🔍 Vérification des dépendances..."
	@if ! pkg-config --exists check; then \
		echo "⚠️ La bibliothèque check n'est pas installée. Installation..."; \
		if [ -x "$(command -v apt-get)" ]; then \
			sudo apt-get update && sudo apt-get install -y check; \
		elif [ -x "$(command -v dnf)" ]; then \
			sudo dnf install -y check check-devel; \
		elif [ -x "$(command -v yum)" ]; then \
			sudo yum install -y check check-devel; \
		else \
			echo "❌ Impossible d'installer check automatiquement. Veuillez l'installer manuellement."; \
			exit 1; \
		fi; \
	fi

$(BIN_DIR) $(TEST_BIN_DIR) $(TEST_OBJ_DIR):
	@mkdir -p $@

$(MAIN_EXE): $(OBJS) | $(BIN_DIR)
	@echo "🛠️ Compilation Linux en cours..."
	@$(CC) -o $(BIN_DIR)/$@ $^ $(LIBS)
	@echo "✅ Compilation terminée"

$(DUEL_EXE): $(DUEL_OBJS) | $(BIN_DIR)
	@$(CC) $^ -o $(BIN_DIR)/$@
	@echo "✅ Compilation duel terminée"

$(SAVE_EXE): $(SAVE_OBJS) | $(BIN_DIR)
	@$(CC) $^ -o $(BIN_DIR)/$@
	@echo "✅ Compilation save terminée"

# Règles de compilation
$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Règles de compilation des tests
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(TEST_OBJ_DIR)
	@echo "🧪 Compilation du test $<..."
	@$(CC) $(TEST_CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(TEST_BIN_DIR)/%: $(TEST_OBJ_DIR)/%.o $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) | $(TEST_BIN_DIR)
	@echo "🔗 Liaison du test $@..."
	@$(CC) -o $@ $^ $(TEST_LIBS)
	@echo "✅ Compilation de $@ terminée"

# Cibles de tests
test: unit-tests

unit-tests: $(TEST_BINS)
	@echo "🧪 Exécution des tests unitaires..."
	@for test in $(TEST_BINS); do \
		echo "▶️ Exécution de $$test"; \
		$$test; \
	done

test-memory: $(TEST_BINS)
	@echo "🧪 Test mémoire avec Valgrind..."
	@valgrind --leak-check=full $(BIN_DIR)/$(MAIN_EXE) -test-mode

testDuel: $(DUEL_EXE)
	@echo "🚀 Lancement du programme duel..."
	@./$(BIN_DIR)/$(DUEL_EXE)

run: $(MAIN_EXE)
	@echo "🚀 Lancement du programme main..."
	@./$(BIN_DIR)/$(MAIN_EXE)

rebuild: clean all

# Nettoyage
clean:
	@echo "🧹 Nettoyage en cours..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(TEST_BIN_DIR)
	@echo "✅ Nettoyage terminé"

# Packaging
package-linux: $(MAIN_EXE)
	@echo "📦 Création du package linux..."
	@mkdir -p $(BIN_DIR)/libs
	@ldd $(BIN_DIR)/$(MAIN_EXE) | awk '{print $$3}' | grep -v "ld-linux" | grep -v "linux-vdso.so" | xargs -I{} cp -v {} $(BIN_DIR)/libs/ 2>/dev/null || true
	@echo '#!/bin/bash' > run.sh
	@echo 'cd "$(dirname "$$0")"' >> run.sh
	@echo 'export LD_LIBRARY_PATH=$$(pwd)/$(BIN_DIR)/libs' >> run.sh
	@echo "./$(BIN_DIR)/$(MAIN_EXE) \"$$@\"" >> run.sh
	@chmod +x run.sh $(BIN_DIR)/$(MAIN_EXE)
	@chmod -R 775 $(BIN_DIR) assets
	@chmod -R 777 data
	@tar -czf ICPocket.tar.gz run.sh $(BIN_DIR) assets data README.md
	@echo "✅ Package linux créé : ICPocket.tar.gz"

# Documentation
doxygen:
	@echo "🧹 Nettoyage de la documentation..."
	@rm -rf docs/html/*
	@doxygen Doxyfile 
	@echo "✅ Documentation générée dans docs"

latex:
	@echo "🧹 Nettoyage en cours..."
	@find rapport -type f ! -name "*.tex" ! -name "*.png" -delete
	@cd rapport && pdflatex modele.tex
	@cd rapport && makeglossaries modele
	@cd rapport && pdflatex modele.tex
	@echo "✅ Documentation générée dans docs/tpLatex/modele.pdf" 