#include "BMP_Picture.h"
#include <unistd.h>
#include <getopt.h>

typedef struct Request {
  int need_to_rewrite;
  int need_to_filter;
  char *which_component;
  int need_to_cut;
} request;

static const char *optString = "rf:c";

static const struct option long_opts[] = {
  {"rewrite", no_argument, NULL, 'r'},
  {"filter", required_argument, NULL, 'f'},
  {"cut", no_argument, NULL, 'c'},
  {NULL, no_argument, NULL, 0}
};
  

int main(int argc, char **argv) {
  FILE *file;
  bmp_picture picture;
  request instruction = {0, 0, NULL, 0};
  int option, longIndex;

  //Обработка входных аргументов
  if (strstr(argv[argc - 1], ".bmp") == NULL)
    return 0;
  while ((option = getopt_long(argc, argv, optString, long_opts, &longIndex)) != -1) {
    switch (option) {
      case 'r': {
        instruction.need_to_rewrite = 1;
        break;
      }
      case 'f': {
        instruction.need_to_filter = 1;
        instruction.which_component = optarg;
        break;
      }
      case 'c': {
        instruction.need_to_cut = 1;
        break;
      }
      default: {
        return 0;
      }
    }
  }
  if (instruction.need_to_filter) {
    if (strcmp(instruction.which_component, "red") != 0 &&
        strcmp(instruction.which_component, "green") != 0 &&
        strcmp(instruction.which_component, "blue") != 0) {
      return 0;
    }
  }
  puts("3");
  //Открытие файла и ввод bmp-заголовочников
  if (!(file = fopen(argv[argc - 1], "rb"))) {
    return 0;
  }
  if (fread(&picture.bfh, sizeof(picture.bfh), 1, file) != 1) {
    fclose(file);
    return 0;
  }
  if (fread(&picture.bih, sizeof(picture.bih), 1, file) != 1) {
    fclose(file);
    return 0;
  }
  //Выделение памяти под картинку
  if (!(picture.bitmap = createPictureMemory(picture.bih.biHeight, picture.bih.biWidth))) {
    fclose(file);
    return 0;
  }
  //Чтение картинки
  for (int i = picture.bih.biHeight - 1; i >= 0; i--) {
    if (fread(picture.bitmap[i], sizeof(bmp_pixel), picture.bih.biWidth, file) != picture.bih.biWidth) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    fseek(file, (4 - (3 * picture.bih.biWidth) % 4) % 4, SEEK_CUR);
  }
  //Основная работа программы
  if (instruction.need_to_rewrite) {
    bmp_pixel *old, *new;
    int red, green, blue, count;
    printf("Write components of the pixel which you'd like to change:\n");
    count = scanf("%d %d %d", &red, &green, &blue);
    if (count != 3) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    if (red < 0 || red > 255) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    else if (green < 0 || green > 255) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    else if (blue < 0 || blue > 255) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    old = createNewPixel((uint8_t) blue, (uint8_t) green, (uint8_t) red);
    printf("Write components of the new pixel:\n");
    count = scanf("%d %d %d", &red, &green, &blue);
    if (count != 3) {
      free(old);
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    if (red < 0 || red > 255) {
      free(old);
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    else if (green < 0 || green > 255) {
      free(old);
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    else if (blue < 0 || blue > 255) {
      free(old);
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    new = createNewPixel((uint8_t) blue, (uint8_t) green, (uint8_t) red);
    changeColour(picture, *old, *new);
    free(old);
    free(new);
  }
  if (instruction.need_to_filter) {
    int intensive, check;
    printf("Write the intensive of your colour filter:\n");
    check = scanf("%d", &intensive);
    if (check != 1) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    if (intensive < 0 || intensive > 255) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    colourFilter(picture, instruction.which_component, (uint8_t) intensive);
  }
  if (instruction.need_to_cut) {
    int perX, perY, count;
    printf("Write number of pieces per X coordinate axis and per Y coordinate axis:\n");
    count = scanf("%d %d", &perX, &perY);
    if (count != 2)  {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    if (perX <= 0 || perY <= 0) {
      freePicture(picture.bitmap, picture.bih.biHeight);
      fclose(file);
      return 0;
    }
    cutIntoPieces(picture, (uint32_t) perX, (uint32_t) perY);
  }
  //Копирование картинки в отдельный файл
  writeIntoFile(picture, "result.bmp");
  freePicture(picture.bitmap, picture.bih.biHeight);
  fclose(file);
  return 0;
}
