#include "BMP_Picsel.h"

//Создание нового пикселя
bmp_picsel *createNewPicsel(uint8_t blue, uint8_t green, uint8_t red) {
  bmp_picsel *newOne = malloc(sizeof(bmp_picsel));
  newOne->blue = blue;
  newOne->green = green;
  newOne->red = red;
  return newOne;
}

//Перезапись пикселя другим пикселем
void rewritePicsel(bmp_picsel *old, bmp_picsel *new) {
  old->blue = new->blue;
  old->green = new->green;
  old->red = new->red;
}

//Проверка идентичности пикселей
int picselsAreEqual(bmp_picsel one, bmp_picsel two) {
  if (one.red == two.red)
    if (one.green == two.green)
      if (one.blue == two.blue)
        return 1;
  return 0;
}

//Замена определённого бита
int changeComponent(bmp_picsel *picsel, char *colour, uint8_t intensive) {
  char *colours[] = { "blue", "green", "red" };
  int flag = -1;

  for (int i = 0; i < 3; i++)
    if (strcmp(colours[i], colour) == 0)
      flag = i;
  if (flag == -1)
    return 0;
  switch (flag) {
    case 0: {
      picsel->blue = intensive;
      break;
    }
    case 1: {
      picsel->green = intensive;
      break;
    }
    case 2: {
      picsel->red = intensive;
      break;
    }
  }
  return 1;
}
