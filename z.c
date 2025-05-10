# Nom de l'exécutable
EXEC = cy_fighters

# Dossiers
SRC_DIR = .
OBJ_DIR = obj

# Trouve tous les fichiers .c
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Génère les fichiers .o correspondants dans obj/
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.

# Règle par défaut
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# Compilation des fichiers .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers compilés
clean:
	rm -rf $(OBJ_DIR) *.o $(EXEC)

# Nettoyage complet
fclean: clean

# Recompilation complète
re: fclean all
