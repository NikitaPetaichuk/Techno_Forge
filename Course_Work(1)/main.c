#include "Interface.h"

static const char *opt_string = "r:f:c:nbh?";
static const struct option long_opts[] = {
  {"rewrite", required_argument, NULL, 'r'},
  {"filter", required_argument, NULL, 'f'},
  {"cut", required_argument, NULL, 'c'},
  {"negative", no_argument, NULL, 'n'},
  {"blackwhite", no_argument, NULL, 'b'},
  {"help", no_argument, NULL, 'h'},
  {NULL, no_argument, NULL, 0}
};

int main(int argc, char **argv) {
  configs inst = {NULL, NULL, NULL, NULL};
  bmp_picture *picture;
  int opt, long_index = 0, count = 0;

  inst.queue = calloc(QUEUE_SIZE, sizeof(char));
  opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
  while (opt != -1) {
    switch (opt) {
      case 'r':
        inst.queue[count++] = opt;
        inst.rewrite_fs = optarg;
        break;
      case 'f':
        inst.queue[count++] = opt;
        inst.filter_fs = optarg;
        break;
      case 'c':
        inst.queue[count++] = opt;
        inst.cutting_fs = optarg;
        break;
      case 'n':
        inst.queue[count++] = opt;
        break;
      case 'b':
        inst.queue[count++] = opt;
        break;
      case 'h':
      case '?':
        printHelpMessage();
	return 0;
    }
    opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
  }
  if (strstr(argv[argc - 1], ".bmp") == NULL) {
    printf("Trying to operate with wrong file (or with nothing).\n");
    free(inst.queue);
    return 0;
  }
  if (!(picture = readPicture(argv[argc - 1]))) {
    free(inst.queue);
    return 0;
  }
  for (int i = 0; i < count; i++) {
    switch(inst.queue[i]) {
      case 'r':
        if (!rewriteInterface(*picture, inst)) {
          freePicture(picture->bitmap, picture->bih.biHeight);
          free(picture);
          free(inst.queue);
          return 0;
        }
        break;
      case 'f':
        if (!filterInterface(*picture, inst)) {
          freePicture(picture->bitmap, picture->bih.biHeight);
          free(picture);
          free(inst.queue);
          return 0;
        }
        break;
      case 'c':
        if (!cutInterface(*picture, inst)) {
          freePicture(picture->bitmap, picture->bih.biHeight);
          free(picture);
          free(inst.queue);
          return 0;
        }
        break;
      case 'n':
        NegativeFilter(*picture);
        break;
      case 'b':
        BlackWhiteFilter(*picture);
        break;
    }
  }
  if (strstr(inst.queue, "r") != NULL ||
      strstr(inst.queue, "f") != NULL ||
      strstr(inst.queue, "n") != NULL ||
      strstr(inst.queue, "b") != NULL)
    writeIntoFile(*picture, "result.bmp");
  freePicture(picture->bitmap, picture->bih.biHeight);
  free(picture);
  free(inst.queue);
  return 0;
}
