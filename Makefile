all: clean matrixgen gen10by10 

matrixgen:
	gcc matrixgen.c -o matrixgen

gen10by10:
	./matrixgen -s 10 -b 10 -p

clean:
	rm -f matrixgen
	rm -f data_*