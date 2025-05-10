# Nom de l'exécutable
EXEC = projet

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Dossiers
BUILD_DIR = build

# Fichiers sources
SRC = projet.c

# Headers (pas besoin de toucher les .h directement, ils sont inclus dans les règles)
HEADERS = projet.h combat.h affichage.h effet.h

# Objets compilés
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

# Règle principale
all: $(EXEC)

# Construction de l'exécutable à partir des objets
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation de chaque .c en .o dans le dossier build
$(BUILD_DIR)/%.o: %.c %.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Créer le dossier build si besoin
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Nettoyer
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Cibles spéciales
.PHONY: all clean
