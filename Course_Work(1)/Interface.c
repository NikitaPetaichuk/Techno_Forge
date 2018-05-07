#include "Interface.h"

int isCorrectByte(int byte) {
  return byte <= 255;
}

int isCorrectCoordinate(int coord, int max) {
  return coord < max;
}

int isCorrectFormat(char *string, char *reg_exp) {
  regex_t reg_comp;
  if (regcomp(&reg_comp, reg_exp, REG_EXTENDED)) {
    return 0;
  }
  return regexec(&reg_comp, string, 0, NULL, 0);
}

void printHelpMessage() {
  printf("\033[1m*** BMPSHOP ***\n\n");
  printf("USAGE:\n\n");
  printf("\t\033[0m./bmpshop flags_with_arguments <filename>.bmp\n\n");
  printf("\033[1mFLAGS:\n\n");
  printf("\t\033[0m1) -r (--rewrite); argument format: \"x_y:r_g_b\" - change colour, which belongs to the pixel with coordinates (x, y) from\n"); 
  printf("\t                                                  the upper left corner, to the new one with red component 'r', green\n");
  printf("\t                                                  component 'g' and blue component 'b' (each one from 0 to 255);\n\n");
  printf("\t2) -f (--filter); argument format: \"comp:inten\" - change every pixel's component 'comp' (\"red\", \"blue\" or \"green\") to the new\n");
  printf("\t                                                  value 'inten' (from 0 t0 255)\n\n");
  printf("\t3) -c (--cut); argument format: \"x:y\" - cut picture into x * y pieces (x per picture width, y per picture height).\n");
  printf("\t                                        Evevy piece goes to the new file with the name \"piece_#N.bmp\"\n\n");
  printf("\t4) -s (--standart); need argument - Use standart filter (\"negative\", \"blackwhite\" or \"sepia\") to the picture.\n\n");
  printf("\t5) -n (--name); need argument - Choose name for the file with changed picture (default - \"result.bmp\").\n\n");
  printf("\t6) -i (--info); no argument - Show some information about the picture.\n\n");
  printf("\033[1mRemember!\033[0m The sequence of flags is important (first flag instruction will be done first).\n");
  printf("Changed picture will be written into a new file (unless you only cut picture into pieces).\n");
  printf("When you choose a name for a new file, it's nessesary to use the format \"<filename>.bmp\".\n");
}

void printInfo(bmp_picture picture, char *name) {
  printf("\033[1mINFORMATION ABOUT BMP-FILE\033[0m\n\n");
  printf("\033[1mName\033[0m: %s\n", name);
  printf("\033[1mSize\033[0m: %.2fK\n", (float) picture.bfh.bfSize / 1024);
  printf("\033[1mWidth\033[0m: %u\n", picture.bih.biWidth);
  printf("\033[1mHeight\033[0m: %u\n", picture.bih.biHeight);
}

int rewriteInterface(bmp_picture picture, configs inst) {
  bmp_pixel *old, *new;
  int args[5];
  int size[2] = {(int) picture.bih.biWidth, (int) picture.bih.biHeight};

  if(isCorrectFormat(inst.rewrite_fs, "^[[:digit:]]+_[[:digit:]]+:[[:digit:]]+_[[:digit:]]+_[[:digit:]]+$")) {
    printf("Wrong argument format for rewriting (or you write invalid numbers, for example negative of fractional).\n");
    return 0;
  }
  args[0] = atoi(strtok(inst.rewrite_fs, "_:"));
  for (int i = 1; i < 5; i++)
    args[i] = atoi(strtok(NULL, "_:"));
  for (int i = 0; i < 5; i++) {
    if (i < 2) {
      if (!isCorrectCoordinate(args[i], size[i])) {
        printf("Incorrect coordinate: must be not bigger than %d.\n", size[i] - 1);
        return 0;
      }
    }
    else {
      if (!isCorrectByte(args[i])) {
        printf("Incorrect intensity: must be not bigger than 255.\n");
        return 0;
      }
    }
  }
  old = &picture.bitmap[args[1]][args[0]];
  new = createNewPixel((uint8_t) args[4], (uint8_t) args[3], (uint8_t) args[2]);
  changeColour(picture, *old, *new);
  free(new);
  return 1;
}

int filterInterface(bmp_picture picture, configs inst) {
  int intensity;
  char *colour;

  if(isCorrectFormat(inst.filter_fs, "^[[:alpha:]]+:[[:digit:]]+$")) {
    printf("Wrong argument format for filtrating (or you write invalid numbers, for example negative of fractional).\n");
    return 0;
  }
  colour = strtok(inst.filter_fs, ":");
  intensity = atoi(strtok(NULL, ":"));
  if (strcmp(colour, "red") != 0 &&
      strcmp(colour, "green") != 0 &&
      strcmp(colour, "blue") != 0) {
    printf("Unknown colour component.\n");
    return 0;
  }
  if (!isCorrectByte(intensity)) {
    printf("Incorrect intensity: must be not bigger than 255.\n");
    return 0;
  }
  colourFilter(picture, colour, (uint8_t) intensity);
  return 1;
}

int cutInterface(bmp_picture picture, configs inst) {
  int perX, perY;

  if(isCorrectFormat(inst.cutting_fs, "^[[:digit:]]+:[[:digit:]]+$")) {
    printf("Wrong argument format for cutting (or you write invalid numbers, for example negative of fractional).\n");
    return 0;
  }
  perX = atoi(strtok(inst.cutting_fs, ":"));
  perY = atoi(strtok(NULL, ":"));
  if (perX == 0 || perY == 0) {
    printf("Incorrect division: division by zero.\n");
    return 0;
  }
  else if (perX > picture.bih.biWidth || perY > picture.bih.biHeight) {
    printf("Incorrect division: number of pixels is not enough for the such division.\n"); 
    return 0;
  }
  if (!cutIntoPieces(picture, (uint32_t) perX, (uint32_t) perY))
    return 0;
  return 1;
}

int stdFilterInterface(bmp_picture picture, configs inst) {
  if (strcmp(inst.std_filter, "negative") == 0) {
    NegativeFilter(picture);
  }
  else if (strcmp(inst.std_filter, "blackwhite") == 0) {
    BlackWhiteFilter(picture);
  }
  else if (strcmp(inst.std_filter, "sepia") == 0) {
    SepiaFilter(picture);
  }
  else {
    printf("Unknown standart filter.\n");
    return 0;
  }
  return 1;
}
