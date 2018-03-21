#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 10
#define ERROR_DEQ 0

typedef char type;
typedef struct Queue {
  type *array;
  int deqloc, enqloc, isFull;
} queue;

void initQueue(queue **q) {
  *q = malloc(sizeof(queue));
  (*q)->array = calloc(QUEUE_SIZE + 1, sizeof(type));
  (*q)->deqloc = 0;
  (*q)->enqloc = 0;
}

void enq(queue *q, type elem) {
  if ((q->enqloc + 1 == q->deqloc) ||
      (q->enqloc == QUEUE_SIZE && q->deqloc == 0)) {
    printf("Queue is overflown, sorry.\n");
    return;
  }
  q->enqloc = (q->enqloc == QUEUE_SIZE) ? 0 : q->enqloc + 1;
  q->array[q->enqloc] = elem;
}

type deq(queue *q) {
  if (q->enqloc == q->deqloc) {
    printf("Queue is empty, sorry.\n");
    return ERROR_DEQ;
  }
  q->deqloc = (q->deqloc == QUEUE_SIZE) ? 0 : q->deqloc + 1;
  return q->array[q->deqloc];
}

type top(queue *q) {
  if (q->enqloc == q->deqloc) {
    printf("Queue is empty.\n");
    return ERROR_DEQ;
  }
  int index = (q->deqloc == QUEUE_SIZE) ? 0 : q->deqloc + 1;
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

int main() {
  queue *qu;

  initQueue(&qu);
  printf("Working with 5 elements\n");
  for (int i = 1; i <= 5; i++) {
    printf("%d)I'm trying to put %c into queue.\n", i, 'a' + i - 1);
    enq(qu, 'a' + i - 1);
    printf("Number of elements: %d\n", count(qu));
  }
  printf("--------------------\n");
  for (int i = 1; i <= 5; i++) {
    printf("%d)I'm trying to get element from queue: ", i);
    type now = deq(qu);
    if (now != 0)
      printf("OK\n");
    printf("Number of elements: %d\n", count(qu));
  }
/*  printf("--------------------\n"); 
  printf("Working with ten elements\n");
  for (int i = 1; i < QUEUE_SIZE + 2; i++) {
    printf("%d)I'm trying to put %c into queue.\n", i, 'a' + i - 1);
    enq(qu, 'a' + i - 1);
    printf("Number of elements: %d\n", count(qu));
  }
  printf("--------------------\n");
  for (int i = 1; i < QUEUE_SIZE + 2; i++) {
    printf("%d)I'm trying to get element from queue: ", i);
    type now = deq(qu);
    if (now != 0)
      printf("OK\n");
    printf("Number of elements: %d\n", count(qu));
  } */
  if (isEmpty)
    printf("Function \"isEmpty\" feels good.\n");
  finalize(qu);
  return 0;
}
