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

void setToBlackAndWhite(bmp_pixel *pixel) {
  unsigned int black_white = (pixel->blue + pixel->green + pixel->red) / 3;
  pixel->blue = black_white;
  pixel->green = black_white;
  pixel->red = black_white;
}

void setToSepia(bmp_pixel *pixel) {
  unsigned int new_red = (unsigned int)(0.393 * pixel->red + 0.769 * pixel->green + 0.189 * pixel->blue);
  unsigned int new_green = (unsigned int)(0.349 * pixel->red + 0.686 * pixel->green + 0.168 * pixel->blue);
  unsigned int new_blue = (unsigned int)(0.272 * pixel->red + 0.534 * pixel->green + 0.131 * pixel->blue);
  pixel->red = (new_red <= 255) ? new_red : 255;
  pixel->green = (new_green <= 255) ? new_green : 255;
  pixel->blue = (new_blue <= 255) ? new_blue : 255;
}
