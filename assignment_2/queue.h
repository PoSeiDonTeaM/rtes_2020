#ifndef QUEUE_H_INCLUDED    
#define QUEUE_H_INCLUDED


#include <stdlib.h>
#include <pthread.h>


typedef struct {
  void * (*work)(void *);
  void * arg;
  struct timeval stopwatch;
}workFunction;

typedef struct {
    workFunction *buf;
    long size, head, tail;
    int full, empty;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull, *notEmpty;
}Queue;

Queue *queueInit(int size);

void queueDelete(Queue *q);

void queueDel(Queue *q, workFunction *out);

void queueAdd(Queue *q, workFunction in);


#endif
