#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//Структура пикселя
typedef struct BMP_Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} bmp_pixel;

//Создание нового пикселя
bmp_pixel* createNewPixel(uint8_t blue, uint8_t green, uint8_t red);

//Перезапись пикселя другим пикселем
void rewritePixel(bmp_pixel* old, bmp_pixel* new);

//Проверка идентичности пикселей
int pixelsAreEqual(bmp_pixel one, bmp_pixel two);

//Замена определённого бита
void changeComponent(bmp_pixel *picsel, char *colour, uint8_t intensive);
