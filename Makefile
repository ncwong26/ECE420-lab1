all: clean runDemo 

matrixgen:
	gcc matrixgen.c -o matrixgen

gen10by10: cleanMatrixgen matrixgen
	./matrixgen -s 10 -b 10 -p

runDemo: demo
	./demo

demo: gen10by10 cleanDemo
	gcc -g -Wall demo.c lab1_IO.c -o demo -lpthread

cleanDemo:
	rm -rf demo

cleanMatrixgen:
	rm -rf matrixgen
	rm -rf data_*

clean: cleanDemo cleanMatrixgen
	rm -rf *.dSYM
	clear