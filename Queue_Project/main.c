#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 50
#define QUEUE_SIZE 10
#define ERROR_DEQ 0

typedef char type;
typedef struct Queue {
  type *array;
  int deqloc, enqloc, now_size, max_size;
} queue;

void initQueue(queue **q, int size) {
  *q = malloc(sizeof(queue));
  (*q)->max_size = size;
  (*q)->array = calloc((*q)->max_size, sizeof(type));
  (*q)->deqloc = 0;
  (*q)->enqloc = 0;
  (*q)->now_size = 0;
}

type deq(queue **q) {
  if ((*q)->now_size == 0)
    return ERROR_DEQ;
  type elem = (*q)->array[(*q)->deqloc];
  (*q)->deqloc = ((*q)->deqloc + 1) % ((*q)->max_size);
  (*q)->now_size--;
  return elem;
}

void enq(queue **q, type elem) {
  if ((*q)->now_size == (*q)->max_size) 
    deq(q);
  (*q)->array[(*q)->enqloc] = elem;
  (*q)->enqloc = ((*q)->enqloc + 1) % ((*q)->max_size);
  (*q)->now_size++;
}

type top(queue *q) {
  if (q->now_size == 0)
    return ERROR_DEQ;
  return q->array[q->deqloc];
}

int isEmpty(queue *q) {
  return !q->now_size;
}

int count(queue *q) {
  return q->now_size;
}

void finalize(queue *q) {
  free(q->array);
  free(q);
}

void print_queue(queue *q) {
  for (int i = q->deqloc, j = 0; j < q->now_size; i = (i + 1) % q->max_size, j++)
    printf("%c", q->array[i]);
}

void showInfo(queue *q) {
  printf("\n----Queue Information----\n");
  if (isEmpty(q)) {
    printf("The queue is empty.\n");
  }
  else {
    printf("Number of elements: %d.\n", count(q));
    printf("Top element: '%c'.\n", top(q));
    printf("Elements: ");
    print_queue(q);
    printf("\n");
  }
  printf("-------------------------\n\n");
}

void intro(queue **q, char **string) {
  printf("Data Tester is happy to see you!\n");
  printf("Data Tester is the programm which is used to test various data structures.\n");
  printf("This version is specialized on the cyclical array-based queue, which size is %d.\n\n", QUEUE_SIZE);

  initQueue(q, QUEUE_SIZE);
  *string = malloc(STRING_SIZE * sizeof(char)); 
}

void instruction_1(queue **q, char *string) {
  int count = 0;

  printf("Write a string and press Enter (max size of the string is %d).\n", STRING_SIZE);
  fgets(string, STRING_SIZE, stdin);
  if (strstr(string, "\n"))
    string[strlen(string) - 1] = '\0';
  for (int i = 0; string[i]; i++) {
    enq(q, string[i]);
    count++;
  }
  printf("You've added %d elements.\n", count);
  showInfo(*q);
}

void instruction_2(queue **q) {
  int number, count = 0;

  printf("Write the number of elements you want do delete: ");
  scanf("%d", &number);
  printf("Removed elements: ");
  for (int i = 0; i < number; i++) {
    char now = deq(q);
    if (now == ERROR_DEQ)
      break;
    printf("%c", now);
    count++;
  }
  printf("\nYou've deleted %d elements.\n", count);
  showInfo(*q);
}

int main() {
  queue *qu;
  char *string;

  intro(&qu, &string);
  instruction_1(&qu, string);
  instruction_2(&qu);
  printf("Bye-bye!\n");
  finalize(qu);
  free(string);
  return 0;
}
