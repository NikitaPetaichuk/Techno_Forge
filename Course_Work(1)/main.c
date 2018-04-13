#include "BMP_Picture.h"

int main() {
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  FILE *file;
  bmp_pixel **picture;

  //Проверка корректного вызова программы и ввода bmp-заголовочников
  if (!(file = fopen("original.bmp", "rb"))) {
    return 0;
  }
  if (fread(&bfh, sizeof(bfh), 1, file) != 1) {
    fclose(file);
    return 0;
  }
  if (fread(&bih, sizeof(bih), 1, file) != 1) {
    fclose(file);
    return 0;
  }
  //Выделение памяти под картинку
  if (!(picture = createPictureMemory(bih.biHeight, bih.biWidth))) {
    fclose(file);
    return 0;
  }
  //Чтение картинки
  for (int i = bih.biHeight - 1; i >= 0; i--) {
    if (fread(picture[i], sizeof(bmp_pixel), bih.biWidth, file) != bih.biWidth) {
      freePicture(picture, bih.biHeight);
      fclose(file);
      return 0;
    }
    fseek(file, (3 * bih.biWidth) % 4, SEEK_CUR);
  }
  //Работа третьей функции
  cutIntoPieces(bfh, bih, picture, 2, 2); //Некорректная работа
  //Копирование картинки в отдельный файл
  writeIntoFile(bfh, bih, picture, "result.bmp");
  freePicture(picture, bih.biHeight);
  fclose(file);
  return 0;
}
