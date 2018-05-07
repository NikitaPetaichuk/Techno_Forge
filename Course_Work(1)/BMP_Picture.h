#include <stdio.h>
#include "BMP_Headers.h"
#include "BMP_Pixel.h"

#define BUFFER_SIZE 20

typedef struct BMP_Picture {
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  bmp_pixel **bitmap;
} bmp_picture;

//Получение размеров кусков
uint32_t *getPieces(uint32_t length, unsigned int number);

//Перевод числа в строку
void itoa(int n, char *s);

//Получение имён для картинок
char **getPictureNames(uint32_t number);

//Функция выделения памяти под двумерный массив пикселей
bmp_pixel **createPictureMemory(uint32_t height, uint32_t width);

//Функция освобождения памяти для двумерного массива пикселей
void freePicture(bmp_pixel **array, int lineNumber);

//Копирование области картинки
void copyPicturePiece(bmp_pixel **from, bmp_pixel **destination, uint32_t x_from, uint32_t y_from,
                      uint32_t x_length, uint32_t y_length);

//Запись картинки в отдельный файл
void writeIntoFile(bmp_picture picture, char *name);

//Функция чтения картинки
bmp_picture *readPicture(char *filename);

//Замена цвета
void changeColour(bmp_picture picture, bmp_pixel oldColour, bmp_pixel newColour);

//Фильтрация по определённому цвету
void colourFilter(bmp_picture picture, char *colour, uint8_t intensive);

//Негатив
void NegativeFilter(bmp_picture picture);

//Чёрно-белый фильтр
void BlackWhiteFilter(bmp_picture picture);

//Сепия
void SepiaFilter(bmp_picture picture);

//Деление картинки на части и запись частей в отдельные файлы (необходима доработка)
int cutIntoPieces(bmp_picture picture, uint32_t x_cut, uint32_t y_cut);
