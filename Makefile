# Fichier: Makefile. Il dit a mingw32-make comment compiler le projet.
# La cible all construit coloring, la cible clean supprime les .o et l exe.
# Appelant: l utilisateur avec mingw32-make ou mingw32-make clean.

# compilateur et options
CC = gcc
CFLAGS = -O2 -Wall -fopenmp -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# cible par defaut
all: coloring

# edition du lien final
coloring: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# compilation de chaque .c en .o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# choix de la commande de nettoyage selon Windows ou Linux
ifeq ($(OS),Windows_NT)
RM = del /Q /F
CLEANFILES = src\*.o coloring.exe
else
RM = rm -f
CLEANFILES = src/*.o coloring
endif

# supprime les objets et l executable
clean:
	-$(RM) $(CLEANFILES)
