#include "Interface.h"

int isCorrectByte(int byte) {
  return (byte >= 0) && (byte <= 255);
}

int isCorrectCoordinate(int coord, int max) {
  return (coord >= 0) && (coord < max);
}

int isCorrectFormat(char *string, char *reg_exp) {
  regex_t reg_comp;
  if (regcomp(&reg_comp, reg_exp, REG_EXTENDED)) {
    return 0;
  }
  return regexec(&reg_comp, string, 0, NULL, 0);
}

int startInterface(int argc, char **argv, configs *inst) {
  int option, opt_index, count = 0;
  char *opt_string = "r:f:c:";
  struct option long_opts[] = {
    {"rewrite", required_argument, NULL, 'r'},
    {"filter", required_argument, NULL, 'f'},
    {"cut", required_argument, NULL, 'c'},
    {NULL, 0, NULL, 0}
  };

  inst->queue = calloc(QUEUE_SIZE, sizeof(char));
  while ((option = getopt_long(argc, argv, opt_string, long_opts, &opt_index)) != -1) {
    switch (option) {
      case 'r':
        inst->queue[count++] = option;
        inst->rewrite_fs = optarg;
        break;
      case 'f':
        inst->queue[count++] = option;
        inst->filter_fs = optarg;
        break;
      case 'c':
        inst->queue[count++] = option;
        inst->cutting_fs = optarg;
        break;
    }
  }
  return count;
}

int rewriteInterface(bmp_picture picture, configs inst) {
  bmp_pixel *old, *new;
  int args[5];
  int size[2] = {(int) picture.bih.biWidth, (int) picture.bih.biHeight};

  if(isCorrectFormat(inst.rewrite_fs, "[[:digit:]]+-[[:digit:]]+:[[:digit:]]+-[[:digit:]]+-[[:digit:]]+")) {
    printf("wrong\n");
    return 0;
  }
  args[0] = atoi(strtok(inst.rewrite_fs, "-:"));
  for (int i = 1; i < 5; i++)
    args[i] = atoi(strtok(NULL, "-:"));
  for (int i = 0; i < 5; i++) {
    if (i < 2) {
      if (!isCorrectCoordinate(args[i], size[i]))
        return 0;
    }
    else {
      if (!isCorrectByte(args[i]))
        return 0;
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

  if(isCorrectFormat(inst.filter_fs, "[[:alpha:]]+:[[:digit:]]+")) {
    printf("wrong\n");
    return 0;
  }
  colour = strtok(inst.filter_fs, ":");
  intensity = atoi(strtok(NULL, ":"));
  if (strcmp(colour, "red") != 0 &&
      strcmp(colour, "green") != 0 &&
      strcmp(colour, "blue") != 0) 
    return 0;
  if (!isCorrectByte(intensity))
    return 0;
  colourFilter(picture, colour, (uint8_t) intensity);
  return 1;
}

int cutInterface(bmp_picture picture, configs inst) {
  int perX, perY;

  if(isCorrectFormat(inst.cutting_fs, "[[:digit:]]+:[[:digit:]]+")) {
    printf("wrong\n");
    return 0;
  }
  perX = atoi(strtok(inst.cutting_fs, ":"));
  perY = atoi(strtok(NULL, ":"));
  if (perX <= 0 || perY <= 0)
    return 0;
  else if (perX > picture.bih.biWidth || perY > picture.bih.biHeight)
    return 0;
  cutIntoPieces(picture, (uint32_t) perX, (uint32_t) perY);
  return 1;
}
