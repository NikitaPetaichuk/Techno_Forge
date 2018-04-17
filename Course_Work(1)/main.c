#include "BMP_Picture.h"

int main() {
  FILE *file;
  bmp_picture picture;

  //Проверка корректного вызова программы и ввода bmp-заголовочников
  if (!(file = fopen("original.bmp", "rb"))) {
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
  //Работа третьей функции
  cutIntoPieces(picture, 3, 3); //Некорректная работа
  //Копирование картинки в отдельный файл
  writeIntoFile(picture, "result.bmp");
  freePicture(picture.bitmap, picture.bih.biHeight);
  fclose(file);
  return 0;
}
