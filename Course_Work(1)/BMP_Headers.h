#include <stdint.h>

//Заголовочник №1
#pragma pack(push, 2)
typedef struct tagBITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

//Заголовочник №2
#pragma pack(push, 2)
typedef struct tagBITMAPINFOHEADER {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
    uint32_t biRedChBitmask;
    uint32_t biGreenChBitmask;
    uint32_t biBlueChBitmask;
    uint32_t biAlphaChBitmask;
    uint32_t biClrSpcType;
    uint32_t biClrSpcEndpoints;
    uint32_t biRedChGamma;
    uint32_t biGreenChGamma;
    uint32_t biBlueChGamma;
    uint32_t biIntent;
    uint32_t biICCProfData;
    uint32_t biICCProfSize;
    uint32_t Reserved;
} BITMAPINFOHEADER;
#pragma pack(pop)
