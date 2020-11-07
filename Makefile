all: bf2c

bf2c: main.c
	gcc -O2 -o bf2c main.c

test: test.c
	gcc -O2 -o test test.c

test.c: bf2c test.bf
	cat test.bf | ./bf2c > test.c
