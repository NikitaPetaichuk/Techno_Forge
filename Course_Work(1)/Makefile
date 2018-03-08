all: main.o BMP_Picsel.o
	gcc main.o BMP_Picsel.o -o bmpshop
main.o: main.c BMP_Picsel.h BMP_Headers.h
	gcc -c main.c
BMP_Picsel.o: BMP_Picsel.c BMP_Picsel.h
	gcc -c BMP_Picsel.c
