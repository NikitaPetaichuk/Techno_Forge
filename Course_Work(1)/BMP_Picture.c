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
    uint32_t size = width * 3 + (3 * width) % 4;
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

void writeIntoFile(BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih, bmp_pixel **picture, char *name) {
  FILE *new_file;

  if (!(new_file = fopen(name, "wb+")))
    return;
  fwrite(&bfh, sizeof(bfh), 1, new_file);
  fwrite(&bih, sizeof(bih), 1, new_file);
  for (int i = bih.biHeight - 1; i >= 0; i--) {
    fwrite(picture[i], sizeof(bmp_pixel), bih.biWidth, new_file);
    fseek(new_file, (3 * bih.biWidth) % 4, SEEK_CUR);
  }
  fclose(new_file);
}

void changeColour(BITMAPINFOHEADER bih, bmp_pixel **picture, bmp_pixel oldColour, bmp_pixel newColour) {
  for (int i = 0; i < bih.biHeight; i++)
    for (int j = 0; j < bih.biWidth; j++)
      if (pixelsAreEqual(picture[i][j], oldColour))
        rewritePixel(&picture[i][j], &newColour);
}

int colourFilter(BITMAPINFOHEADER bih, bmp_pixel **picture, char *colour, uint8_t intensive) {
  for (int i = 0; i < bih.biHeight; i++)
    for (int j = 0; j < bih.biWidth; j++)
      if (changeComponent(&picture[i][j], colour, intensive) == 0)
        return 0;
  return 1;
}           

int cutIntoPieces(BITMAPFILEHEADER bfh, BITMAPINFOHEADER bih, bmp_pixel **picture, uint32_t x_cut, uint32_t y_cut) {
  BITMAPFILEHEADER *bfh_array;
  BITMAPINFOHEADER *bih_array;
  uint32_t *x_pieces, *y_pieces;
  bmp_pixel ***cutted_pictures;
  char **names;
  uint32_t count = x_cut * y_cut, i = 0;

  bfh_array = calloc(count, sizeof(BITMAPFILEHEADER));
  bih_array = calloc(count, sizeof(BITMAPINFOHEADER));
  x_pieces = getPieces(bih.biWidth, x_cut);
  y_pieces = getPieces(bih.biHeight, y_cut);
  cutted_pictures = calloc(count, sizeof(bmp_pixel **));
  names = getPictureNames(count);
  if (!(bfh_array && bih_array && x_pieces && y_pieces && cutted_pictures && names)) {
    printf("NotEnoughMemoryError\n");
    return 0;
  }
  for (uint32_t j = 0; j < y_cut; j++)
    for (uint32_t k = 0; k < x_cut; k++)
      if (!(cutted_pictures[i++] = createPictureMemory(y_pieces[j], x_pieces[k]))) {
	printf("NotEnoughMemoryError\n");
	return 0;
      }
  i = 0;
  for (uint32_t j = 0; j < count; j++) {
    bfh_array[i] = bfh;
    bfh_array[i].bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
                          x_pieces[j % x_cut] * y_pieces[j / y_cut] * sizeof(bmp_pixel);
  }
  for (uint32_t j = 0; j < count; j++) {
    bih_array[j] = bih;
    bih_array[j].biWidth = x_pieces[j % x_cut];
    bih_array[j].biHeight = y_pieces[j / y_cut];
  }
  for (uint32_t j = 0; j < y_cut; j++)
    for (uint32_t k = 0; k < x_cut; k++)
      copyPicturePiece(picture, cutted_pictures[i++], k * (bih.biWidth / x_cut),
                       j * (bih.biHeight / y_cut), x_pieces[k], y_pieces[j]);
  for (uint32_t j = 0; j < count; j++)
    writeIntoFile(bfh_array[j], bih_array[j], cutted_pictures[j], names[j]);
  for (uint32_t j = 0; j < count; j++)
    free(names[j]);
  free(names);
  for (uint32_t j = 0; j < count; j++)
    freePicture(cutted_pictures[j], y_pieces[j / y_cut]);
  free(cutted_pictures);
  free(bfh_array);
  free(bih_array);
  free(x_pieces);
  free(y_pieces);
  return 1;
}
