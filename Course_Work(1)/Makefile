all: main.o BMP_Picsel.o BMP_Picture.o
	gcc main.o BMP_Picsel.o BMP_Picture.o -o bmpshop
main.o: main.c BMP_Picture.h BMP_Picsel.h BMP_Headers.h
	gcc -c main.c
BMP_Picture.o: BMP_Picture.c BMP_Picture.h BMP_Picsel.h BMP_Headers.h
	gcc -c BMP_Picture.c
BMP_Picsel.o: BMP_Picsel.c BMP_Picsel.h
	gcc -c BMP_Picsel.c
