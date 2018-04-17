#include "BMP_Picture.h"

uint32_t *getPieces(uint32_t length, unsigned int number) {
  if (length < number)
    return NULL;
  uint32_t len = length;
  uint32_t *pieces = calloc(number, sizeof(uint32_t));
  if (pieces) {
    for (int i = 0; i < number - 1; i++) {
      pieces[i] = length / number;
      len -= pieces[i];
    }
    pieces[number - 1] = len;
  }
  return pieces;
}

void itoa(int n, char *s) {
  int i, sign, j;
  char c;

  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
  }
}

char **getPictureNames(uint32_t number) {
  char *prefix = "piece_#";
  char *postfix = ".bmp";
  int *indexes = calloc(number, sizeof(int));
  char **names = calloc(number, sizeof(char *));

  if (!indexes)
    return NULL;
  for (uint32_t i = 0; i < number; i++)
    indexes[i] = i + 1;
  if (names) {
    char *index_buffer = calloc(BUFFER_SIZE, sizeof(char));
    for (uint32_t i = 0; i < number; i++) {
      if (names[i] = calloc(BUFFER_SIZE + 15, sizeof(char))) {
        strncat(names[i], prefix, 8);
        itoa(indexes[i], index_buffer);
        strncat(names[i], index_buffer, BUFFER_SIZE);
        strncat(names[i], postfix, 5);
      }
      else {
        for (uint32_t j = 0; j < i; j++)
          free(names[i]);
        break;
      }
    }
    free(index_buffer);
  }
  free(indexes);
  return names;
}

bmp_pixel **createPictureMemory(uint32_t height, uint32_t width) {
  bmp_pixel **new_picture;
  if (!(new_picture = calloc(height, sizeof(bmp_pixel*))))
    return NULL;
  for (uint32_t i = 0; i < height; i++) {
    uint32_t size = width * 3 + (4 - (3 * width) % 4) % 4;
    if (!(new_picture[i] = calloc(size, sizeof(bmp_pixel)))) {
      freePicture(new_picture, i);
      return NULL;
    }
  }
  return new_picture;
}

void freePicture(bmp_pixel **array, int lineNumber) {
  for (int i = 0; i < lineNumber; i++)
    free(array[i]);
  free(array);
}

void copyPicturePiece(bmp_pixel **from, bmp_pixel **destination, uint32_t x_from, uint32_t y_from,
                      uint32_t x_length, uint32_t y_length) {
  uint32_t x = 0, y = 0;
  for (uint32_t i = y_from; i < y_length; i++) {
    for (uint32_t j = x_from; j < x_length; j++)
      rewritePixel(&destination[y][x++], &from[i][j]);
    x = 0;
    y++;
  }
}

void writeIntoFile(bmp_picture picture, char *name) {
  FILE *new_file;

  if (!(new_file = fopen(name, "wb+")))
    return;
  fwrite(&picture.bfh, sizeof(picture.bfh), 1, new_file);
  fwrite(&picture.bih, sizeof(picture.bih), 1, new_file);
  for (int i = picture.bih.biHeight - 1; i >= 0; i--) {
    fwrite(picture.bitmap[i], sizeof(bmp_pixel), picture.bih.biWidth, new_file);
    fseek(new_file, (4 - (3 * picture.bih.biWidth) % 4) % 4, SEEK_CUR);
  }
  fclose(new_file);
}

void changeColour(bmp_picture picture, bmp_pixel oldColour, bmp_pixel newColour) {
  for (int i = 0; i < picture.bih.biHeight; i++)
    for (int j = 0; j < picture.bih.biWidth; j++)
      if (pixelsAreEqual(picture.bitmap[i][j], oldColour))
        rewritePixel(&picture.bitmap[i][j], &newColour);
}

int colourFilter(bmp_picture picture, char *colour, uint8_t intensive) {
  for (int i = 0; i < picture.bih.biHeight; i++)
    for (int j = 0; j < picture.bih.biWidth; j++)
      if (changeComponent(&picture.bitmap[i][j], colour, intensive) == 0)
        return 0;
  return 1;
}           

int cutIntoPieces(bmp_picture picture, uint32_t x_cut, uint32_t y_cut) {
  bmp_picture *cutted_pictures;
  uint32_t *x_pieces, *y_pieces;
  char **names;
  uint32_t count = x_cut * y_cut;

  x_pieces = getPieces(picture.bih.biWidth, x_cut);
  y_pieces = getPieces(picture.bih.biHeight, y_cut);
  cutted_pictures = calloc(count, sizeof(bmp_picture));
  names = getPictureNames(count);
  for (uint32_t i = 0; i < count; i++)
    if (!(cutted_pictures[i].bitmap = createPictureMemory(y_pieces[i / y_cut], x_pieces[i % x_cut]))) {
      printf("NotEnoughMemoryError\n");
      return 0;
    }
  for (uint32_t i = 0; i < count; i++) {
    cutted_pictures[i].bfh = picture.bfh;
    cutted_pictures[i].bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
                          x_pieces[i % x_cut] * y_pieces[i / y_cut] * sizeof(bmp_pixel);
    cutted_pictures[i].bih = picture.bih;
    cutted_pictures[i].bih.biWidth = x_pieces[i % x_cut];
    cutted_pictures[i].bih.biHeight = y_pieces[i / y_cut];
  }
  for (uint32_t i = 0; i < y_cut; i++)
    for (uint32_t j = 0; j < x_cut; j++)
      copyPicturePiece(picture.bitmap, cutted_pictures[i++].bitmap, j * (picture.bih.biWidth / x_cut),
                       i * (picture.bih.biHeight / y_cut), x_pieces[j], y_pieces[i]);
  for (uint32_t i = 0; i < count; i++)
    writeIntoFile(cutted_pictures[i], names[i]);
  for (uint32_t i = 0; i < count; i++)
    freePicture(cutted_pictures[i].bitmap, y_pieces[i / y_cut]);
  for (uint32_t i = 0; i < count; i++)
    free(names[i]);
  free(cutted_pictures);
  free(names);
  free(x_pieces);
  free(y_pieces);
  return 1;
}
