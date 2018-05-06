#include "BMP_Picture.h"
#include <unistd.h>
#include <getopt.h>
#include <regex.h>

#define QUEUE_SIZE 15

typedef struct Configs {
  char *queue;
  char *rewrite_fs;
  char *filter_fs;
  char *cutting_fs;
  char *res_name;
} configs;

//Проверка на корректность бита
int isCorrectByte(int byte);

//Проверка на корректность координаты
int isCorrectCoordinate(int coord, int max);

//Проверка на корректность формата аргумента 
int isCorrectFormat(char *string, char *reg_exp);

//Справка
void printHelpMessage();

//Информация о картинке
void printInfo(bmp_picture picture, char *name);

//Функция-оболочка для переписывания пикселей
int rewriteInterface(bmp_picture picture, configs inst);

//Функция-оболочка для фильтрации
int filterInterface(bmp_picture picture, configs inst);

//Функция-оболочка для деления на части
int cutInterface(bmp_picture picture, configs inst);
