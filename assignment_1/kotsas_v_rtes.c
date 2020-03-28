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
#include <iostream> 
#include <queue>
#include <math.h>
#include <sys/time.h>

#define QUEUESIZE 50
#define pro_threads 100
#define con_threads 50
#define MAX_QUEUE_SIZE 100
#define MAX_LOOPS 80000

// Function declaration of all required functions
void *producer (void *args);
void *consumer (void *args);

struct timeval t1, t2, start_program, end_program;

int elapsedTime[MAX_LOOPS*pro_threads];

double *angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3};

typedef struct {
  void * (*work)(void *);
  void * arg;
}workFunction ;

typedef struct {
  workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  int num_prods,num_cons;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, workFunction *in);
void queueDel (queue *q, workFunction *out);
void queueReduceProducers(queue *q);
void queueReduceConsumers(queue *q);
void * my_function(void *arg); 

int main ()
{
  queue *fifo;
  
  double total_elapsed_time = 0;  
  gettimeofday(&start_program, NULL);
  
  double mean_elapsed_time = 0;
  
  struct timeval t1, t2, start_program, end_program;
  
  pthread_t pro[pro_threads], con[con_threads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr , PTHREAD_CREATE_JOINABLE);

  fifo = queueInit ();
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
  
  /*       Create producer and consumer threads      */
  
  
  for (int x=0; x<pro_threads; x++)
        pthread_create (&pro[x], NULL, producer, NULL);
        
  for (int x=0; x<con_threads; x++)
        pthread_create (&con[x], NULL, consumer, NULL);
        
  for (int x=0; x<pro_threads; x++)
  {
        pthread_join (pro[x], NULL);
  }  
      
  for (int x=0; x<con_threads; x++)
  {
        pthread_join (con[x], NULL);
  }
  
  queueDelete (fifo);
  
  printf("\nWow, you made it to the end sailor! Congrats.\n");
  
  gettimeofday(&end_program, NULL);
  total_elapsed_time =  (end_program.tv_sec - start_program.tv_sec)*1000;
  total_elapsed_time += (end_program.tv_usec - start_program.tv_usec)/1000;
  
  printf("Total program elapsed time is: %d ms", total_elapsed_time);
  
  for (int x=0; x<MAX_LOOPS*pro_threads; x++)
      mean_elapsed_time += elapsedTime[x];
  
  mean_elapsed_time = mean_elapsed_time/(MAX_LOOPS*pro_threads);
  
  printf("The mean value of the elapsed time between a producer thread and a consumer one is: %d", mean_elapsed_time);

  return 0;
}

void *producer (void *q)
{
  queue *fifo;
  int i;

  fifo = (queue *)q;

  for (i = 0; i < MAX_LOOPS; i++)
  {
    
    pthread_mutex_lock (fifo->mut);
    
    while (fifo->full) {
      printf ("Producer: Hold on there cowboy, another thread is executing..."\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
    
    workFunction *producerExecution;
    
    long int *x;
    x= (long int *)malloc(sizeof(long int));
    producerExecution = (workFunction *)malloc(sizeof(workFunction));
    
    producerExecution->arg  = (void *)angles[rand()%9];
    producerExecution->work = &work_execution;
    
    // Starting the timer
    gettimeofday(&t1, NULL);
    
    *x = tv.tv_usec+tv.tv_sec*1000000;
    producerExecution->arg = (void *) x;
    
    queueAdd (fifo,producerExecution);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
    //usleep (100000);
  }

  pthread_mutex_lock(fifo->mut);
  queueReduceProducers(fifo);
  pthread_mutex_unlock(fifo->mut);
  pthread_cond_broadcast (fifo->notEmpty);
  return (NULL);
}

void *consumer (void *q)
{
  queue *fifo;
  int i;
  workFunction d;

  fifo = (queue *)q;

  while(1) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty && fifo->num_prods>0) {
      printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
    if(fifo->empty!=1 )queueDel (fifo, &d);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
    if(fifo->num_prods==0 )break;
    printf ("consumer: recieved %ld.\n", *(long int*)d.arg);
    pthread_mutex_lock(fifo -> mut);
    struct timeval tv_con;
    gettimeofday(&tv_con , NULL);
    long int temp;
    temp = tv_con.tv_sec*1000000 + tv_con.tv_usec;
    *(long int*)d.arg = temp - *(long int *)d.arg;
    d.work(d.arg);
    pthread_mutex_unlock(fifo -> mut);
    //usleep(200000);
  }
  /*for (i = 0; i < MAX_LOOPS; i++) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
    queueDel (fifo, &d);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
    printf ("consumer: recieved %d.\n", d);
    usleep (50000);
  }*/
  pthread_mutex_lock(fifo->mut);
  queueReduceConsumers(fifo);
  pthread_mutex_unlock(fifo->mut);
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

queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->num_prods = PRODUCERS;
  q->num_cons  = CONSUMERS;
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

void queueAdd (queue *q, workFunction *in)
{
  q->buf[q->tail] = *in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueDel (queue *q, workFunction *out)
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
void queueReduceProducers(queue *q){
  q->num_prods =  q->num_prods - 1;
  printf("THERE ARE NOW  %d producers \n",q->num_prods);
}
void queueReduceConsumers(queue *q){
  q->num_cons =  q->num_cons - 1;
  printf("THERE ARE NOW %d consumers \n",q->num_cons);
}

void work_execution(void *arg)
 {
     
    double output = cos(*(int*)arg);
    
    printf("Value of Output is: %f\n", output);
    
 }

void * my_function(void *arg){
  long int k = *(long int *)arg;  
  int error;
  FILE *fp;
  printf("%ld\n",k);
  fp=fopen("/home/kostas/EMBEDDED/assignments/1/results.txt","a");
  fprintf(fp, "%ld\n", k);
  fclose(fp);
}
