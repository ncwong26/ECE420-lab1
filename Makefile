CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-lpthread -lm

all: clean run

matrixgen:
	$(CC) $(CFLAGS) matrixgen.c -o matrixgen -lm

gen10by10: cleanMatrixgen matrixgen
	./matrixgen -s 1000 -b 1000 -p

# Build the required executable name for the lab
main: gen10by10
	$(CC) $(CFLAGS) demo.c lab1_IO.c -o main $(LDFLAGS)

# Run with a default thread count (change 4 -> 1/9/16 if you want)
run: main
	./main 1

cleanMain:
	rm -f main

cleanMatrixgen:
	rm -f matrixgen
	rm -f data_*

clean: cleanMain cleanMatrixgen
	rm -rf *.dSYM
