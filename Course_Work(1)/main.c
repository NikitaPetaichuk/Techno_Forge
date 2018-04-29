#include "Interface.h"

int main(int argc, char **argv) {
  configs instruction = {NULL, NULL, NULL, NULL};
  bmp_picture *picture;
  int opt_count;

  if (!(opt_count = startInterface(argc, argv, &instruction)))
    return 0;
  if (strstr(argv[argc - 1], ".bmp") == NULL) {
    free(instruction.queue);
    return 0;
  }
  if (!(picture = readPicture(argv[argc - 1]))) {
    free(instruction.queue);
    return 0;
  }
  for (int i = 0; i < opt_count; i++) {
    switch(instruction.queue[i]) {
      case 'r': {
        if (!rewriteInterface(*picture, instruction)) {
          freePicture(picture->bitmap, picture->bih.biHeight);
          free(picture);
          free(instruction.queue);
          return 0;
        }
        break;
      }
      case 'f': {
        if (!filterInterface(*picture, instruction)) {
          freePicture(picture->bitmap, picture->bih.biHeight);
          free(picture);
          free(instruction.queue);
          return 0;
        }
        break;
      }
      case 'c': {
        if (!cutInterface(*picture, instruction)) {
          freePicture(picture->bitmap, picture->bih.biHeight);
          free(picture);
          free(instruction.queue);
          return 0;
        }
        break;
      }
    }
  }
  writeIntoFile(*picture, "result.bmp");
  freePicture(picture->bitmap, picture->bih.biHeight);
  free(picture);
  free(instruction.queue);
  return 0;
}
