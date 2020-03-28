/*
 *	File	: pc.c
 *
 *	Title	: Demo Producer/Consumer.
 *
 *	Short	: A solution to the producer consumer problem using
 *		pthreads.	
 *
 *	Long 	:
 *
 *	Author	: Andrae Muys
 *
 *	Date	: 18 September 1997
 *
 *	Revised	:
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define PI 3.141592654

#define QUEUESIZE 10
#define LOOP 5
#define pro_threads 10
#define con_threads 10

void *producer (void *args);
void *consumer (void *args);

typedef struct {
  int buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, int in);
void queueDel (queue *q, int *out);

int main ()
{
    
  queue *fifo;
  
  /* User may enter the number of producer and consumer threads */
  
  pthread_t pro[pro_threads];
  pthread_t con[con_threads];

  fifo = queueInit ();
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
  
  
  /*       Create producer and consumer threads      */
  
  
  for (int x=0; x<pro_threads; x++)
        pthread_create (&pro[x], NULL, producer, fifo);
  
  for (int x=0; x<con_threads; x++)
        pthread_create (&con[x], NULL, consumer, fifo);
  
  for (int x=0; x<pro_threads; x++)
        pthread_join (pro[x], NULL);
  
  for (int x=0; x<pro_threads; x++)
        pthread_join (con[x], NULL);
  
  queueDelete (fifo);

  return 0;
}

void *producer (void *q)
{
    
  queue *fifo;
  int i;

  fifo = (queue *)q;

  for (i = 0; i < LOOP; i++) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->full) {
      printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
    
    queueAdd (fifo, angles[rand()%9]);                    // Selecting a random angle from the matrix "angles"
    
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
    usleep (100000);
  }
  
  for (i = 0; i < LOOP; i++) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->full) {
      printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
    
    queueAdd (fifo, angles[rand()%9]);                    // Selecting a random angle from the matrix "angles"
    
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
    usleep (200000);
  }
  return (NULL);
}

void *consumer (void *q)
{
  queue *fifo;
  int i, d, output;

  fifo = (queue *)q;

  while(1){
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
    queueDel (fifo, &d);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
    
    /* Mathematics 101 */
    
    output = work(d);
    
    printf ("consumer: received %d.\n", d);
    
    
    /* Break the while when the producers stop */
    
    if(){
     
        break;
        
    }
    
  }
  
  return (NULL);
}


/*
  typedef struct {
  int buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
  } queue;
*/


struct workFunction {
  void * (*work)(void *);
  void * arg;
};

void (*work_ptr[])(int) = {work1, work2, work3, work4, work5, work6, work7, work8, work9, work10}' 

void work1(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of Output is: %f\n", output);
 }
 
 void work2(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of Output is: %f\n", output);
 }
 
 void work3(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of Output is: %f\n", output);
 }
 
 void work4(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of Output is: %f\n", output);
 }
 
 void work5(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of Output is: %f\n", output);
 }
 
 void work6(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of Output is: %f\n", output);
 }
 
 void work7(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of Output is: %f\n", output);
 }
 
 void work8(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work8 is: %f\n", output);
 }
 
 void work9(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work9 is: %f\n", output);
 }
 
 void work10(double arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work10 is: %f\n", output);
 }

queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);
	
  return (q);
}

void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);	
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
}

void queueAdd (queue *q, int in)
{
  q->buf[q->tail] = in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueDel (queue *q, int *out)
{
  *out = q->buf[q->head];

  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;

  return;
}
