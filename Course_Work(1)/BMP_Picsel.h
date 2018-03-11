#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//Структура пикселя
typedef struct BMP_Picsel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} bmp_picsel;

//Создание нового пикселя
bmp_picsel* createNewPicsel(uint8_t blue, uint8_t green, uint8_t red);

//Перезапись пикселя другим пикселем
void rewritePicsel(bmp_picsel* old, bmp_picsel* new);

//Проверка идентичности пикселей
int picselsAreEqual(bmp_picsel one, bmp_picsel two);

//Замена определённого бита
int changeComponent(bmp_picsel *picsel, char *colour, uint8_t intensive);
