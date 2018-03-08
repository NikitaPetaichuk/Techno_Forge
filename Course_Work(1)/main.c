#include <stdio.h>
#include "BMP_Headers.h"
#include "BMP_Picsel.h"

//Замена цвета
void changeColour(BITMAPINFOHEADER bih, bmp_picsel **picture, bmp_picsel oldColour, bmp_picsel newColour) {
  for (int i = 0; i < bih.biHeight; i++)
    for (int j = 0; j < bih.biWidth; j++)
      if (picselsAreEqual(picture[i][j], oldColour))
        rewritePicsel(&picture[i][j], &newColour);
}

//Фильтрация по определённому цвету
int colourFilter(BITMAPINFOHEADER bih, bmp_picsel **picture, char *colour, uint8_t intensive) {
  for (int i = 0; i < bih.biHeight; i++)
    for (int j = 0; j < bih.biWidth; j++)
      if (changeComponent(&picture[i][j], colour, intensive) == 0)
        return 0;
  return 1;
}

//Функция освобождения памяти для двумерного массива пикселей
void freePicture(bmp_picsel **array, int lineNumber) {
  for (int i = 0; i < lineNumber; i++)
    free(array[i]);
  free(array);
}

//Функция выделения памяти под двумерный массив пикселей
bmp_picsel **createPictureMemory(uint32_t height, uint32_t width) {
  bmp_picsel **new_picture;
  if (!(new_picture = calloc(height, sizeof(bmp_picsel*))))
    return NULL;
  for (int i = 0; i < height; i++) {
    int size = width * 3 + (3 * width) % 4;
    if (!(new_picture[i] = calloc(size, sizeof(bmp_picsel)))) {
      freePicture(new_picture, i);
      return NULL;
    }
  }
  return new_picture;
}

int main() {
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  FILE *file;
  bmp_picsel **picture;

  //Проверка корректного вызова программы и ввода bmp-заголовочников
  if (!(file = fopen("original.bmp", "rb"))) {
    printf("FileOpeningError\n");
    return 0;
  }
  if (fread(&bfh, sizeof(bfh), 1, file) != 1) {
    printf("FileReadingError\n");
    fclose(file);
    return 0;
  }
  if (fread(&bih, sizeof(bih), 1, file) != 1) {
    printf("FileReadingError\n");
    fclose(file);
    return 0;
  }
  printf("Checking arguments and bmp-headers input: done\n");
  //Выделение памяти под картинку
  if (!(picture = createPictureMemory(bih.biHeight, bih.biWidth))) {
    printf("CreatingMemoryError\n");
    fclose(file);
    return 0;
  }
  printf("Memory work: done\n");
  //Чтение картинки
  for (int i = bih.biHeight - 1; i >= 0; i--) {
    if (fread(picture[i], sizeof(bmp_picsel), bih.biWidth, file) != bih.biWidth) {
      printf("PictureReadingError\n");
      freePicture(picture, bih.biHeight);
      fclose(file);
      return 0;
    }
    fseek(file, (3 * bih.biWidth) % 4, SEEK_CUR);
  }
  printf("Reading picture: done\n");
  //Работа первой функции
  bmp_picsel *newPicsel = createNewPicsel(255, 0, 0);
  changeColour(bih, picture, picture[300][200], *newPicsel);
  bmp_picsel *newPicsel1 = createNewPicsel(0, 255, 0);
  changeColour(bih, picture, picture[300][300], *newPicsel1);
  //Работа второй функции
  if (colourFilter(bih, picture, "red", 0) == 0) {
    printf("FilterError\n");
    freePicture(picture, bih.biHeight);
    fclose(file);
    return 0;
  }
  if (colourFilter(bih, picture, "green", 0) == 0) {
    printf("FilterError\n");
    freePicture(picture, bih.biHeight);
    fclose(file);
    return 0;
  }
  //Копирование картинки
  FILE *file1;
  if (!(file1 = fopen("result.bmp", "wb+"))) {
    printf("NewFileOpeningError\n");
    freePicture(picture, bih.biHeight);
    fclose(file);
    return 0;
  }
  if (fwrite(&bfh, sizeof(bfh), 1, file1) != 1) {
    printf("NewFileWritingError\n");
    freePicture(picture, bih.biHeight);
    fclose(file1);
    fclose(file);
    return 0;
  }
  if (fwrite(&bih, sizeof(bih), 1, file1) != 1) {
    printf("NewFileWritingError\n");
    freePicture(picture, bih.biHeight);
    fclose(file1);
    fclose(file);
    return 0;
  }
  for (int i = bih.biHeight - 1; i >= 0; i--) {
    if (fwrite(picture[i], sizeof(bmp_picsel), bih.biWidth, file1) != bih.biWidth) {
      printf("NewFileWritingError\n");
      freePicture(picture, bih.biHeight);
      fclose(file1);
      fclose(file);
      return 0;
    }
    fseek(file1, (3 * bih.biWidth) % 4, SEEK_CUR);
  }
  printf("Making new picture: done\n");
  freePicture(picture, bih.biHeight);
  fclose(file1);
  fclose(file);
  return 0;
}
