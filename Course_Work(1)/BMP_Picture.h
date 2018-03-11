#include <stdio.h>
#include "BMP_Headers.h"
#include "BMP_Picsel.h"

#define BUFFER_SIZE 20

//Получение размеров кусков
uint32_t *getPieces(uint32_t length, unsigned int number);

//Перевод числа в строку
void itoa(int n, char *s);

//Получение имён для картинок
char **getPictureNames(uint32_t number);

//Функция выделения памяти под двумерный массив пикселей
bmp_picsel **createPictureMemory(uint32_t height, uint32_t width);

//Функция освобождения памяти для двумерного массива пикселей
void freePicture(bmp_picsel **array, int lineNumber);

//Копирование области картинки
void copyPicturePiece(bmp_picsel **from, bmp_picsel **destination, uint32_t x_from, uint32_t y_from,
                      uint32_t x_length, uint32_t y_length);

//Запись картинки в отдельный файл
void writeIntoFile(BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih, bmp_picsel **picture, char *name);

//Замена цвета
void changeColour(BITMAPINFOHEADER bih, bmp_picsel **picture, bmp_picsel oldColour, bmp_picsel newColour);

//Фильтрация по определённому цвету
int colourFilter(BITMAPINFOHEADER bih, bmp_picsel **picture, char *colour, uint8_t intensive);

//Деление картинки на части и запись частей в отдельные файлы (необходима доработка)
int cutIntoPieces(BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih, bmp_picsel **picture, uint32_t x_cut, uint32_t y_cut);
