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

int isCorrectByte(int byte);

int isCorrectCoordinate(int coord, int max);

int isCorrectFormat(char *string, char *reg_exp);

int startInterface(int argc, char **argv, configs *inst);

int rewriteInterface(bmp_picture picture, configs inst);

int filterInterface(bmp_picture picture, configs inst);

int cutInterface(bmp_picture picture, configs inst);
