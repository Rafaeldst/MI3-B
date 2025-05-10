# Nom de l'exécutable
EXEC = projet

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Dossiers
BUILD_DIR = build

# Fichiers sources
SRC = projet.c combat.c affichage.c effet.c

# Fichiers objets (dans build/)
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

# Règle principale
all: $(EXEC)

# Construction de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des .c en .o dans build/
$(BUILD_DIR)/%.o: %.c projet.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Créer le dossier build s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Nettoyer les fichiers objets et l'exécutable
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Cibles spéciales
.PHONY: all clean
