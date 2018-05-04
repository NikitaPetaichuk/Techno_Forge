#include "BMP_Picture.h"

uint32_t *getPieces(uint32_t length, unsigned int number) {
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

void freePicture(bmp_pixel **array, int lineNumber) {
  for (int i = 0; i < lineNumber; i++)
    free(array[i]);
  free(array);
}

bmp_pixel **createPictureMemory(uint32_t height, uint32_t width) {
  bmp_pixel **new_picture;
  if (!(new_picture = calloc(height, sizeof(bmp_pixel*))))
    return NULL;
  for (uint32_t i = 0; i < height; i++) {
    if (!(new_picture[i] = calloc(width, sizeof(bmp_pixel)))) {
      printf("Not enough memory for the picture.\n");
      freePicture(new_picture, i);
      return NULL;
    }
  }
  return new_picture;
}

void copyPicturePiece(bmp_pixel **from, bmp_pixel **destination, uint32_t x_from, uint32_t y_from,
                      uint32_t x_length, uint32_t y_length) {
  uint32_t x = 0, y = 0;
  for (uint32_t i = y_from; i < y_from + y_length; i++) {
    for (uint32_t j = x_from; j < x_from + x_length; j++) {
      rewritePixel(&destination[y][x++], &from[i][j]);
    }
    x = 0;
    y++;
  }
}

void writeIntoFile(bmp_picture picture, char *name) {
  FILE *new_file;

  if (!(new_file = fopen(name, "wb+"))) {
    printf("File opening error (file - \"%s\").\n", name);
    return;
  }
  if (fwrite(&picture.bfh, sizeof(picture.bfh), 1, new_file) != 1) {
    printf("File writing error (file - \"%s\").\n", name);
    fclose(new_file);
    return;
  }
  if (fwrite(&picture.bih, sizeof(picture.bih), 1, new_file) != 1) {
    printf("File writing error (file - \"%s\").\n", name);
    fclose(new_file);
    return;
  }
  for (int i = picture.bih.biHeight - 1; i >= 0; i--) {
    for (int j = 0; j < picture.bih.biWidth; j++) {
      if (fwrite(&picture.bitmap[i][j], sizeof(bmp_pixel), 1, new_file) != 1) {
        printf("File writing error (file - \"%s\").\n", name);
        fclose(new_file);
        return;
      }
    }
    int buff = 0;
    if (fwrite(&buff, 1, picture.bih.biWidth % 4, new_file) != picture.bih.biWidth % 4) {
      printf("File writing error (file - \"%s\").\n", name);
      fclose(new_file);
      return;
    }
  }
  fclose(new_file);
}

bmp_picture *readPicture(char *filename) {
  FILE *file = fopen(filename, "rb");
  bmp_picture *new = malloc(sizeof(bmp_picture));

  if (!file) {
    printf("File opening error (file - \"%s\").\n", filename);
    free(new);
    return NULL;
  }
  if (fread(&(new->bfh), sizeof(new->bfh), 1, file) != 1) {
    printf("File reading error (file - \"%s\").\n", filename);
    fclose(file);
    free(new);
    return NULL;
  }
  if (fread(&(new->bih), sizeof(new->bih), 1, file) != 1) {
    printf("File reading error (file - \"%s\").\n", filename);
    fclose(file);
    free(new);
    return NULL;
  }
  if (!(new->bitmap = createPictureMemory(new->bih.biHeight, new->bih.biWidth))) {
    fclose(file);
    free(new);
    return NULL;
  }
  for (int i = new->bih.biHeight - 1; i >= 0; i--) {
    if (fread(new->bitmap[i], sizeof(bmp_pixel), new->bih.biWidth, file) != new->bih.biWidth) {
      printf("File reading error (file - \"%s\").\n", filename);
      fclose(file);
      freePicture(new->bitmap, new->bih.biHeight);
      free(new);
      return NULL;
    }
    fseek(file, new->bih.biWidth % 4, SEEK_CUR);
  }
  fclose(file);
  return new;
}

void changeColour(bmp_picture picture, bmp_pixel oldColour, bmp_pixel newColour) {
  for (int i = 0; i < picture.bih.biHeight; i++)
    for (int j = 0; j < picture.bih.biWidth; j++)
      if (pixelsAreEqual(picture.bitmap[i][j], oldColour))
        rewritePixel(&picture.bitmap[i][j], &newColour);
  return;
}

void colourFilter(bmp_picture picture, char *colour, uint8_t intensity) {
  for (int i = 0; i < picture.bih.biHeight; i++)
    for (int j = 0; j < picture.bih.biWidth; j++)
      changeComponent(&picture.bitmap[i][j], colour, intensity);
  return;
}

void NegativeFilter(bmp_picture picture) {
  for (int i = 0; i < picture.bih.biHeight; i++)
    for (int j = 0; j < picture.bih.biWidth; j++)
      setToNegative(&picture.bitmap[i][j]);
  return;
}       

int cutIntoPieces(bmp_picture picture, uint32_t x_cut, uint32_t y_cut) {
  bmp_picture *cutted_pieces;
  uint32_t *x_pieces, *y_pieces;
  char **names;
  uint32_t count = x_cut * y_cut, now_index = 0;

  if (!(x_pieces = getPieces(picture.bih.biWidth, x_cut))) {
    printf("Not enough memory for the pieces length storage (per width).\n"); 
    return 0;
  }
  if (!(y_pieces = getPieces(picture.bih.biHeight, y_cut))) {
    printf("Not enough memory for the pieces length storage (per height).\n");
    free(x_pieces);
    return 0;
  }
  if (!(names = getPictureNames(count))) {
    printf("Not enough memory for the file names storage.\n");
    free(x_pieces);
    free(y_pieces);
    return 0;
  }
  if (!(cutted_pieces = calloc(count, sizeof(bmp_picture)))) {
    printf("Not enuogh memory for the pieces pictures storage.\n");
    free(x_pieces);
    free(y_pieces);
    for (uint32_t t = 0; t < count; t++)
      free(names[t]);
    free(names);
    return 0;
  }
  for (uint32_t j = 0; j < y_cut; j++) {
    for (uint32_t k = 0; k < x_cut; k++) {
      if (!(cutted_pieces[now_index++].bitmap = createPictureMemory(y_pieces[j], x_pieces[k]))) {
        int t = 0;
        for (uint32_t j1 = 0; j1 < y_cut; j1++) {
          for (uint32_t k1 = 0; k1 < x_cut; k1++) {
            if (t = now_index - 1) {
              free(x_pieces);
              free(y_pieces);
              for (uint32_t t = 0; t < count; t++)
                free(names[t]);
              free(names);
              free(cutted_pieces);
              return 0;              
            }
            freePicture(cutted_pieces[t++].bitmap, y_pieces[j1]);
          }
        }
      }
    }
  }
  now_index = 0;
  uint32_t now_y = 0, now_x = 0;
  for (uint32_t j = 0; j < y_cut; j++) {
    for (uint32_t k = 0; k < x_cut; k++) {
      cutted_pieces[now_index].bfh = picture.bfh;
      cutted_pieces[now_index].bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
                          x_pieces[k] * y_pieces[j] * sizeof(bmp_pixel);
      cutted_pieces[now_index].bih = picture.bih;
      cutted_pieces[now_index].bih.biWidth = x_pieces[k];
      cutted_pieces[now_index].bih.biHeight = y_pieces[j];
      copyPicturePiece(picture.bitmap, cutted_pieces[now_index].bitmap, now_x, now_y,
                      x_pieces[k], y_pieces[j]);
      now_x += x_pieces[k];
      now_index++;
    }
    now_x = 0;
    now_y += y_pieces[j];
  }
  now_index = 0;
  for (uint32_t t = 0; t < count; t++)
    writeIntoFile(cutted_pieces[t], names[t]);
  for (uint32_t j = 0; j < y_cut; j++) {
    for (uint32_t k = 0; k < x_cut; k++) {
      freePicture(cutted_pieces[now_index++].bitmap, y_pieces[j]);
    }
  }
  free(cutted_pieces);
  for (uint32_t t = 0; t < count; t++)
    free(names[t]);
  free(names);
  free(x_pieces);
  free(y_pieces);
  return 1;
}
