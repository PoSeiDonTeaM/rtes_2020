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
#include <queue>
#include <math.h>
#include <sys/time.h>
#include <string.h>

#define PI 3.141592654

#define QUEUESIZE 50
#define pro_threads 1
#define con_threads 96
#define MAX_LOOPS 2000

// Function declaration of all required functions
void *producer (void *args);
void *consumer (void *args);

struct timeval start_program, end_program;

double long elapsedTime[MAX_LOOPS*pro_threads];
int consumerCount = 0;

double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3};

typedef struct {
  void * (*work)(void *);
  void * arg;
  double t1;
}workFunction;

typedef struct {
  workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, workFunction *in);
void queueDel (queue *q, workFunction *out);

void *work_execution(void *arg)
 {
     
    double output = cos(*(int*)arg);
    
    printf("Value of Output is: %f\n", output);
    
 }

void createCSV (char *filename, double long elapsedTime[])
{
 
    printf("\n Creating %s.csv file",filename);
 
    FILE *fp;
    
    filename = strcat(filename,".csv");
    
    fp=fopen(filename,"w+");
    
    for (int i=0; i<MAX_LOOPS*pro_threads; i++)
        fprintf(fp, "\n%d, %Lf", i, elapsedTime[i]);
    
    fclose(fp);
    
    printf("\n %sfile created",filename);
    
}
 
int main ()
{
  
    queue *fifo;
    
    double total_elapsed_time = 0;  
    gettimeofday(&start_program, NULL);
    
    double mean_elapsed_time = 0;
    
    char filename[24] = "Outputs_v1";
    
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
            pthread_create (&pro[x], &attr, producer, fifo);
            
    for (int x=0; x<con_threads; x++)
            pthread_create (&con[x], &attr, consumer, fifo);
            
    for (int x=0; x<pro_threads; x++)
            pthread_join (pro[x], NULL);

    for (int x=0; x<con_threads; x++)
            pthread_join (con[x], NULL);
    
    queueDelete (fifo);
    
    printf("\nWow, you made it to the end sailor! Congrats.\n\n");
    
    gettimeofday(&end_program, NULL);
    total_elapsed_time =  (end_program.tv_sec - start_program.tv_sec)*1000;
    total_elapsed_time += (end_program.tv_usec - start_program.tv_usec)/1000;
    
    
    
    for (int x=0; x<MAX_LOOPS*pro_threads; x++)
    {
        printf("Elapsed Time #%d is: %Lf us\n", x, elapsedTime[x]);
        
        mean_elapsed_time += elapsedTime[x];
    }
    
    mean_elapsed_time = mean_elapsed_time/(MAX_LOOPS*pro_threads);
    
    printf("Total program elapsed time is: %f ms\n\n", total_elapsed_time);
    
    printf("\n\nThe mean value of the elapsed time between a producer thread and a consumer one is: %f us.\n\n\n", mean_elapsed_time);
    
    createCSV(filename, elapsedTime);
  
    return 0;
}

void *producer (void *q)
{
  queue *fifo;
  int i;
  
  struct timeval t1;

  fifo = (queue *)q;

  for (i = 0; i < MAX_LOOPS; i++)
  {
      
    
    pthread_mutex_lock (fifo->mut);
    
    while (fifo->full) {
      printf ("Producer: Hold on there cowboy, another thread is executing...\n\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
    
    workFunction *producerExecution;
    
    double *x;
    
    x = (double *)malloc(sizeof(double));
    producerExecution = (workFunction *)malloc(sizeof(workFunction));
    
    x = &angles[rand()%9];
    
    producerExecution->arg  = (void *) x;  // Passing the address of x to arg with typecast void
    
    producerExecution->work = &work_execution; // Passing the address of the function work_execution
    
    // Starting the timer
    gettimeofday(&t1, NULL);
    
    producerExecution->t1  = t1.tv_sec*1000000 + t1.tv_usec;
    
    
    // Adding the struct producerExecution to queue
    queueAdd (fifo,producerExecution);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
  }
  
    pthread_cond_broadcast (fifo->notEmpty);
    return (NULL);
}

void *consumer (void *q)
{
    
  queue *fifo;
  int i;
  
  struct timeval t2;
  
  workFunction functionExecuter;

  fifo = (queue *)q;

  while(1) {
      
    pthread_mutex_lock (fifo->mut);
    
    if(fifo->empty!=1 && consumerCount < MAX_LOOPS*pro_threads)
    {
        queueDel (fifo, &functionExecuter);
        
        pthread_cond_signal (fifo->notFull);
    
        //pthread_mutex_unlock (fifo->mut);
        //pthread_cond_signal (fifo->notFull);
    
        printf("ConsumerCount is: %d",consumerCount);
        
        //pthread_mutex_lock(fifo -> mut);
    
        // Stopping the timer
        gettimeofday(&t2, NULL);
        
        //Saving the elapsedTime in an array
        elapsedTime[consumerCount]    =  (t2.tv_sec*1000000 + t2.tv_usec - functionExecuter.t1); // sec to us
        
        printf("Hey, I'm consumer with ID %lu. I'm calculating the cosine of angle: %f\n\n", pthread_self(), *(double*)functionExecuter.arg);

        // Running the workFunction!
        functionExecuter.work(functionExecuter.arg);
        
        consumerCount++;
        
        
    }
    else if(consumerCount == MAX_LOOPS*pro_threads)
    {
        pthread_mutex_unlock(fifo -> mut);
        break;
    }
    
    else if (fifo->empty) 
        {
            printf ("consumer: queue EMPTY.\n\n");
            pthread_cond_wait (fifo->notEmpty, fifo->mut);
        }
    
    
    // Unlocking the mutex
    pthread_mutex_unlock(fifo -> mut);
    //usleep(200000);
  }
  
  
  return (NULL);
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
