#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <regex.h>

#define FILE_NUMBER 500
#define STRING_SIZE 150
#define NUMBER_SIZE 32
#define RESULT_FILE "result.txt"
#define START_DIR "root"

typedef struct FileData {
  int number;
  char *string;
} filedata;

int isCorrectFile(char *filename) {
  char *regular = ".+\\.txt$";
  regex_t regExp;

  if (regcomp(&regExp, regular, REG_EXTENDED))
    return 0;
  return regexec(&regExp, filename, 0, NULL, 0) == 0;
}

int compFileData(const void *a, const void *b) {
  filedata *first = (filedata *)a;
  filedata *second = (filedata *)b;
  return (first->number) - (second->number);
}

void readData(char *filename, filedata *array, int *pcount) {
  char *digit = calloc(NUMBER_SIZE, sizeof(char));
  FILE *file = fopen(filename, "r");
  char *buff = calloc(STRING_SIZE, sizeof(char));

  if (!file) return;
  array[*pcount].string = calloc(STRING_SIZE, sizeof(char));
  fgets(buff, STRING_SIZE, file);
  for (int i = 0; buff[i] != ' '; i++)
    digit[i] = buff[i];
  array[*pcount].number = atoi(digit);
  strncat(array[*pcount].string, buff, STRING_SIZE);
  (*pcount)++;
  free(digit);
  free(buff);
  fclose(file);
  return;
}

void goThroughDirs(char *root, filedata *array, int * pcount) {
  char *next = calloc(STRING_SIZE, sizeof(char));
  strncpy(next, root, 25);
  DIR *dir = opendir(root);
  if(!dir) return;
  struct dirent *now = readdir(dir);
  while(now) {
    if(now->d_type == DT_DIR && strcmp(now->d_name, ".") != 0 &&
       strcmp(now->d_name, "..") != 0) {
      printf("dir name: %s\n", now->d_name);
      int len = strlen(next);
      strcat(next, "/");
      strcat(next, now->d_name);
      goThroughDirs(next, array, pcount);
      next[len] = '\0';
    }
    if(now->d_type == DT_REG && isCorrectFile(now->d_name)) {
      printf("file name: %s\n", now->d_name);
      int len = strlen(next);
      strcat(next, "/");
      strcat(next, now->d_name);
      readData(next, array, pcount);
      next[len] = '\0';
    }
    now = readdir(dir);
  }
  free(next);
  closedir(dir);
  return;
}

int main() {
  filedata *array = calloc(FILE_NUMBER, sizeof(filedata));
  int count = 0;
  char *space = " ";
  FILE *output;

  goThroughDirs(START_DIR, array, &count);
  qsort(array, count, sizeof(filedata), compFileData);
  output = fopen(RESULT_FILE, "w");
  for (int i = 0; i < count; i++)
    fwrite(array[i].string, sizeof(char), strlen(array[i].string), output); 
  for (int i = 0; i < count; i++)
    free(array[i].string);
  free(array);
  fclose(output);
  return 0;
}
