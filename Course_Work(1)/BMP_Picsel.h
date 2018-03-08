#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//Структура пикселя
typedef struct BMP_Picsel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} bmp_picsel;

bmp_picsel* createNewPicsel(uint8_t blue, uint8_t green, uint8_t red);

void rewritePicsel(bmp_picsel* old, bmp_picsel* new);

int picselsAreEqual(bmp_picsel one, bmp_picsel two);

int changeComponent(bmp_picsel *picsel, char *colour, uint8_t intensive);
