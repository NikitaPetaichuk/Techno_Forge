all: main.o BMP_Pixel.o BMP_Picture.o Interface.o
	gcc main.o BMP_Pixel.o BMP_Picture.o Interface.o -o bmpshop
main.o: main.c Interface.h BMP_Picture.h BMP_Pixel.h BMP_Headers.h
	gcc -c main.c
Interface.o: Interface.c Interface.h BMP_Picture.h BMP_Pixel.h BMP_Headers.h
	gcc -c Interface.c
BMP_Picture.o: BMP_Picture.c BMP_Picture.h BMP_Pixel.h BMP_Headers.h
	gcc -c BMP_Picture.c
BMP_Pixel.o: BMP_Pixel.c BMP_Pixel.h
	gcc -c BMP_Pixel.c
