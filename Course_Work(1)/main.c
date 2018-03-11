#include "BMP_Picture.h"

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
  /* bmp_picsel *newPicsel = createNewPicsel(255, 0, 0);
  changeColour(bih, picture, picture[300][200], *newPicsel);
  bmp_picsel *newPicsel1 = createNewPicsel(0, 255, 0);
  changeColour(bih, picture, picture[300][300], *newPicsel1); */
  //Работа второй функции
  if (colourFilter(bih, picture, "green", 0) == 0) {
    printf("FilterError\n");
    freePicture(picture, bih.biHeight);
    fclose(file);
    return 0;
  }
  if (colourFilter(bih, picture, "blue", 0) == 0) {
    printf("FilterError\n");
    freePicture(picture, bih.biHeight);
    fclose(file);
    return 0;
  }
  //Работа третьей функции
  //cutIntoPieces(bfh, bih, picture, 2, 2); //Некорректная работа
  //Копирование картинки в отдельный файл
  writeIntoFile(bfh, bih, picture, "result.bmp");
  printf("Making new picture: done\n");
  freePicture(picture, bih.biHeight);
  fclose(file);
  return 0;
}
