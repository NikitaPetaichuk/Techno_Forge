#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <regex.h>

#define STRING_SIZE 200
#define RESULT_FILE "result.txt"
#define START_DIR "root"

typedef struct FileData {
  long long int number;
  char string[STRING_SIZE];
} filedata;

int isCorrectFile(char *filename) {
  char *regular = "^.+\\.txt$";
  regex_t regExp;

  if (regcomp(&regExp, regular, REG_EXTENDED))
    return 0;
  return regexec(&regExp, filename, 0, NULL, 0) == 0;
}

int compFileData(const void *a, const void *b) {
  filedata first = *((filedata *)a);
  filedata second = *((filedata *)b);
  if ((first.number) > (second.number))
    return 1;
  else if ((first.number) == (second.number))
    return 0;
  else
    return -1;
}

void readData(char *filename, filedata **array, int *pcount, int *pmax) {
  char digit[STRING_SIZE] = {0};
  FILE *file = fopen(filename, "r");
  int i;

  if (!file) return;
  *array = realloc(*array, sizeof(filedata) * ((*pcount) + 1));
  fgets((*array)[*pcount].string, STRING_SIZE, file);
  for (i = 0; (*array)[*pcount].string[i] != ' '; i++)
    digit[i] = (*array)[*pcount].string[i];
  (*array)[*pcount].number = atoll(digit);
  (*pcount)++;
  fclose(file);
  return;
}

void goThroughDirs(char *root, filedata **array, int *pcount, int *pmax) {
  char *next = calloc(STRING_SIZE, sizeof(char));
  strncpy(next, root, STRING_SIZE);
  DIR *dir = opendir(root);

  if(!dir) return;
  struct dirent *now = readdir(dir);
  while(now) {
    if(now->d_type == DT_DIR && strcmp(now->d_name, ".") != 0 &&
       strcmp(now->d_name, "..") != 0) {
      int len = strlen(next);
      strcat(next, "/");
      strcat(next, now->d_name);
      goThroughDirs(next, array, pcount, pmax);
      next[len] = '\0';
    }
    if(now->d_type == DT_REG && isCorrectFile(now->d_name)) {
      int len = strlen(next);
      strcat(next, "/");
      strcat(next, now->d_name);
      readData(next, array, pcount, pmax);
      next[len] = '\0';
    }
    now = readdir(dir);
  }
  closedir(dir);
  free(next);
  return;
}

int main() {
  filedata *array = (filedata *)malloc(sizeof(filedata));
  int count = 0, i;
  FILE *output;

  goThroughDirs(START_DIR, &array, &count);
  qsort(array, count, sizeof(filedata), compFileData);
  output = fopen(RESULT_FILE, "w");
  for (i = 0; i < count; i++)
    fwrite(array[i].string, sizeof(char), strlen(array[i].string), output);
  fclose(output);
  free(array);
  return 0;
}
