# ======================
# Opcions d'optimització
# ======================
OPT=-g -Wall -std=c99
#OPT=-O3 -Wall -std=c99

# =========
# Utilitats
# =========
main : main.o minimax.o 4enratlla.o
	gcc -o main $(OPT) main.o minimax.o 4enratlla.o
main.o : main.c
	gcc -c $(OPT) main.c
test : test.o minimax.o 4enratlla.o
	gcc -o test $(OPT) test.o minimax.o 4enratlla.o
test.o : test.c
	gcc -c $(OPT) test.c

# ============
# Biblioteques
# ============
minimax.o : minimax.c
	gcc -c $(OPT) minimax.c
4enratlla.o : 4enratlla.c
	gcc -c $(OPT) 4enratlla.c

# ======
# Neteja
# ======
clean :
	rm -f *.o
realclean : clean
	rm -f main test


