#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_SIZE 10
#define ERROR_DEQ 0

typedef char type;
typedef struct Queue {
  type *array;
  int deqloc, enqloc;
} queue;

void initQueue(queue **q) {
  *q = malloc(sizeof(queue));
  (*q)->array = calloc(QUEUE_SIZE + 1, sizeof(type));
  (*q)->deqloc = 0;
  (*q)->enqloc = 0;
}

int enq(queue *q, type elem) {
  if ((q->enqloc + 1 == q->deqloc) ||
      (q->enqloc == QUEUE_SIZE && q->deqloc == 0))
    return 0;
  q->enqloc = (q->enqloc + 1) % (QUEUE_SIZE + 1);
  q->array[q->enqloc] = elem;
  return 1;
}

type deq(queue *q) {
  if (q->enqloc == q->deqloc)
    return ERROR_DEQ;
  q->deqloc = (q->deqloc + 1) % (QUEUE_SIZE + 1);
  return q->array[q->deqloc];
}

type top(queue *q) {
  if (q->enqloc == q->deqloc)
    return ERROR_DEQ;
  int index = (q->deqloc + 1) % (QUEUE_SIZE + 1);
  return q->array[index];
}

int isEmpty(queue *q) {
  if (q->deqloc == q->enqloc)
    return 1;
  return 0;
}

int count(queue *q) {
  int result = 0;
  for (int i = q->enqloc; i != q->deqloc; i--) {
    if (i == -1) i = QUEUE_SIZE;
    result++;
  }
  return result;
}

void finalize(queue *q) {
  free(q->array);
  free(q);
}

void showInfo(queue *q) {
  printf("\n----Queue Information----\n");
  printf("Number of elements: %d.\n", count(q));
  printf("Top element: '%c'.\n", top(q));
  printf("-------------------------\n\n");
}

void intro(queue **q, char **string) {
  printf("Data Tester is happy to see you!\n");
  printf("Data Tester is the programm which is used to test various data structures.\n");
  printf("This version is specialized on the cyclical array-based queue, which size is %d.\n\n", QUEUE_SIZE);

  initQueue(q);
  *string = malloc(QUEUE_SIZE * sizeof(char)); 
}

void instruction_1(queue *q, char *string) {
  int count = 0;

  printf("Write a string and press Enter.\nFirst %d symbols (except '\\n') will be tried to push into the queue.\n", QUEUE_SIZE);
  fgets(string, QUEUE_SIZE + 1, stdin);
  if (strlen(string) >= QUEUE_SIZE)
    while(getchar() != '\n');
  if (strstr(string, "\n"))
    string[strlen(string) - 1] = '\0';
  for (int i = 0; string[i]; i++) {
    if (!enq(q, string[i]))
      break;
    count++;
  }
  printf("You added %d elements.\n", count);
  showInfo(q);
}

void instruction_2(queue *q) {
  int number, count = 0;

  printf("Write the number of elements you want do delete: ");
  scanf("%d", &number);
  for (int i = 0; i < number; i++) {
    if (deq(q) == ERROR_DEQ)
      break;
    count++;
  }
  printf("You deleted %d elements.\n", count);
  showInfo(q);
}

int main() {
  queue *qu;
  char *string;

  intro(&qu, &string);
  instruction_1(qu, string);
  instruction_2(qu);
  printf("Bye-bye!\n");
  finalize(qu);
  free(string);
  return 0;
}
