#include "BMP_Picture.h"
#include <unistd.h>
#include <getopt.h>
#include <regex.h>

#define QUEUE_SIZE 10

typedef struct Configs {
  char *queue;
  char *rewrite_fs;
  char *filter_fs;
  char *cutting_fs;
} configs;

//Проверка на корректность бита
int isCorrectByte(int byte);

//Проверка на корректность координаты
int isCorrectCoordinate(int coord, int max);

//Проверка на корректность формата аргумента 
int isCorrectFormat(char *string, char *reg_exp);

//Справка
void printHelpMessage();

//Функция-оболочка для переписывания пикселей
int rewriteInterface(bmp_picture picture, configs inst);

//Функция-оболочка для фильтрации
int filterInterface(bmp_picture picture, configs inst);

//Функция-оболочка для деления на части
int cutInterface(bmp_picture picture, configs inst);
