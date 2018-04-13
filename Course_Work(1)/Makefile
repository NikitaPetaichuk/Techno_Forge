all: main.o BMP_Pixel.o BMP_Picture.o
	gcc main.o BMP_Pixel.o BMP_Picture.o -o bmpshop
main.o: main.c BMP_Picture.h BMP_Pixel.h BMP_Headers.h
	gcc -c main.c
BMP_Picture.o: BMP_Picture.c BMP_Picture.h BMP_Pixel.h BMP_Headers.h
	gcc -c BMP_Picture.c
BMP_Pixel.o: BMP_Pixel.c BMP_Pixel.h
	gcc -c BMP_Pixel.c
