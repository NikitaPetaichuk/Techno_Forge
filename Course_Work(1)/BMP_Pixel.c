#include "BMP_Pixel.h"

bmp_pixel *createNewPixel(uint8_t blue, uint8_t green, uint8_t red) {
  bmp_pixel *newOne = malloc(sizeof(bmp_pixel));
  newOne->blue = blue;
  newOne->green = green;
  newOne->red = red;
  return newOne;
}

void rewritePixel(bmp_pixel *old, bmp_pixel *new) {
  old->blue = new->blue;
  old->green = new->green;
  old->red = new->red;
}

int pixelsAreEqual(bmp_pixel one, bmp_pixel two) {
  if (one.red == two.red)
    if (one.green == two.green)
      if (one.blue == two.blue)
        return 1;
  return 0;
}

void changeComponent(bmp_pixel *pixel, char *colour, uint8_t intensity) {
  char *colours[] = { "blue", "green", "red" };
  int flag = -1;

  for (int i = 0; i < 3; i++)
    if (strcmp(colours[i], colour) == 0)
      flag = i;
  switch (flag) {
    case 0: {
      pixel->blue = intensity;
      break;
    }
    case 1: {
      pixel->green = intensity;
      break;
    }
    case 2: {
      pixel->red = intensity;
      break;
    }
  }
  return;
}

void setToNegative(bmp_pixel *pixel) {
  pixel->blue = 255 - pixel->blue;
  pixel->green = 255 - pixel->green;
  pixel->red = 255 - pixel->red;
}
