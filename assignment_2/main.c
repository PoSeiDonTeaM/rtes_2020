/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                   *
 * @file         : main.c                                            *
 *                                                                   *
 * @brief        : The main function that implements a timer that    * 
 *                 assigns tasks to producers and consumers.         *
 *                                                                   *
 * @author       : Giannis Kotsakiachidis                            *
 *                                                                   *
 * @date         : 20 August 2020                                    *
 *                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "timer.h"
#include <sys/time.h>
#include <math.h>
#include <pthread.h>


#define PI 3.141592654

#define con_threads 33


int inputDuration;
int jobsExecuted;
int lostJobs;

// Parameter constructor

void paramInit(queueSize);
void fileInit();
void memoryInit(runtime, period[], periodSelection);
void totalJobsCalc(runtime, period[], periodSelection);
void timCreation(period[], runtime, inputDuration, totalDrift, periodDrift_1, periodDrift_2, periodDrift_3, timMut);
void storeData(int N, FILE *file, int *data);

typedef struct {
    Queue *queue;
    int totalJobs;
    int *jobAliveTime;
    int *jobsExecuted;
    int *jobExecutionTime;
    int *flag;
    pthread_mutex_t *timMut;
} ArgC;

typedef struct {
  void * (*work)(void *);
  void * arg;
  struct timeval stopwatch;
}workFunction;

int main() {
    
    int queueSize       = 0;
    int runtime         = 0;
    int periodSelection = 0;
    const int period[3] = {1000, 100, 10};
    
    // Period selection
    
    do{
        printf("Select the desired period (in milliseconds). Your choices are:\n 1. Period of 1000 milliseconds. \n 2. Period of 100 milliseconds. \n 3. Period of 10 milliseconds. \n 4. Combine all the above periods together in one simulation.\n\n");
        scanf("%d", &periodSelection);
        
        // Error message
        if(periodSelection > 4 || periodSelection < 1)
        {
            printf("The period you selected is invalid. Please select one of the choices above.\n");
        }
    }while(periodSelection > 4 || periodSelection < 1);
        

    // Runtime definition by user
    
    do{
        printf("Please insert the desired runtime.\n");
        scanf("%d", &runtime);
        
        // Error message
        if(runtime <= 0)
        {
            printf("The runtime you selected seems to be smaller than expected. Consider using a runtime with a positive value.\n\n");
        }
    }while(runtime <= 0);
    
    
    // Queue Size definition by user
    
    do{
        printf("Please insert the queue size.\n");
        scanf("%d", &queueSize);
        
        // Error message
        if(queueSize <= 0)
        {
            printf("The size of queue shall be a positive value.\n\n");
        }
    }while(queueSize <= 0);
    

// Calculate total jobs 
totalJobs = totalJobsCalc(runtime, period[], periodSelection);

// Initialize files to store values
fileInit();

for(int i = 0; i < conNum; i++)
{
    // Initialize parameters
    paramInit();
    
    // Allocate memory to store the values later
    memoryInit(runtime, period[], periodSelection);
    
    ArgC *argC = (ArgC*)malloc(sizeof(ArgC));
    argC->totalJobs         = totalJobs;
    argC->jobAliveTime      = jobAliveTime;
    argC->jobExecutionTime  = jobExecutionTime;
    argC->timMut            = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(argC->timMut, NULL);
    
    // Creating consumer threads
    for (int x=0; x<con_threads; x++)
            pthread_create (&con[x], &attr, consumer, fifo);
    
    // Creating required timers
    void timCreation(period[], runtime, inputDuration, totalDrift, periodDrift_1, periodDrift_2, periodDrift_3, timMut);
    
    // Waiting for timers to conclude operations
    if (periodSelection == 4)
    {
        for(int x = 0; x < 3; x++)
            pthread_join(timer[x].tid,NULL);
    } else
        pthread_join(timer->tid, NULL);
    
    
    pthread_cond_broadcast(fifo->notEmpty);
    
    for (int x=0; x<con_threads; x++)
            pthread_join (con[x], NULL);
    
    
    // Store data to file for later computations 
    
    storeData(jobsExecuted, jobAliveTimeFile, jobAliveTime);
    storeData(jobsExecuted, executionTimeFile, executionTime);
    storeData(jobsExecuted, inputDurationFile, inputDuration);
    
    if(periodSelection == 4){
        storeData(runtime * (int)1e3 / period[0] - 1, periodDrift_1_File, periodDrift_1);
        storeData(runtime * (int)1e3 / period[1] - 1, periodDrift_2_File, periodDrift_2);
        storeData(runtime * (int)1e3 / period[2] - 1, periodDrift_3_File, periodDrift_3);
    }else
    {
        storeData(inputDuration - 1, totalDriftFile, totalDrift_mem);
    }
    
    
    //! Cleans up.
    free(jobAliveTime);
    free(inputDuration);
    free(executionTime);
    if (periodSelection == 4) {
        free(periodDrift_1_mem);
        free(periodDrift_2_mem);
        free(periodDrift_3_mem);
    }
    else
        free(totalDrift_mem);
    free(tim);

    // Deletes Queue.
    queueDelete(fifo);

    pthread_mutex_destroy (argC->timMut);
    free(argC->timMut);
    free(argC);
    pthread_mutex_destroy (timMut);
    free(timMut);

    // Leaving some time for the system to rest, till next iteration.
    usleep(100000);
    
}
    
    // Closes files.
    fclose(jobAliveTimeFile);
    fclose(inputDurationFile);
    fclose(executionTimeFile);
    if (periodSelection == 4) {
        fclose(periodDrift_1_File);
        fclose(periodDrift_2_File);
        fclose(periodDrift_3_File);
    }
    else
        fclose(totalDriftFile);
        

    return 0;

}

void paramInit(int queueSize){
    
int inputDuration   = 0;
int jobsExecuted    = 0;
int lostJobs        = 0;

// Initializes Queue.
        Queue *fifo;
        fifo = queueInit(queueSize);
        if (fifo == NULL) {
            fprintf(stderr, "main: Queue Init failed.\n");
            exit(1);
        }
        
}

// File Read/Write permissions constructor to store simulation results

void fileInit(){
    
    FILE *executionTimeFile = fopen("executionTime.csv", "w");
    
    FILE *totalDriftFile, *periodDrift_1_File, *periodDrift_2_File, *periodDrift_3_File;
    
    if (periodSelection == 4) {
        periodDrift_1_File = fopen("periodDrift_1.csv", "w");
        periodDrift_2_File = fopen("periodDrift_2.csv", "w");
        periodDrift_3 = fopen("periodDrift_3.csv", "w");
    }else
    {
        totalDrift = fopen("totalDrift.csv", "w");
    }
    
    FILE *jobAliveTimeFile = fopen("jobAliveTime.csv", "w");
    
    FILE *inputDurationFile = fopen("inputDuration.csv", "w");
    
}

void memoryInit(int runtime, int period[], int periodSelection){
    
    int *jobAliveTime   = (int *)malloc(totalJobs * sizeof(int));
    int *inputDuration      = (int *)malloc(totalJobs * sizeof(int));
    int *executionTime  = (int *)malloc(totalJobs * sizeof(int));
    
    if(periodSelection == 4)
    {
        
        int *periodDrift_1_mem  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
        int *periodDrift_1_mem  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
        int *periodDrift_1_mem  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
    }
    else
    {
        int *totalDrift_mem = (int *)malloc(totalJobs * sizeof(int));
    }
    
}

int totalJobsCalc(int untime, int period[], int periodSelection)
{
    
    int total = 0;
    
    if (periodSelection == 1)
        total = runtime * (int)1000/period[0];
    else if (periodSelection == 2)
        total = runtime * (int)1000/period[1];
    else if (periodSelection == 3)
        total = runtime * (int)1000/period[2];
    else
        total = runtime * (int)1000/period[0] + runtime * (int)1000/period[1] + runtime * (int)1000/period[2];
    
    return total;
}

void timCreation(int period[], int runtime, int inputDuration, int totalDrift_mem, int periodDrift_1, int periodDrift_2, int periodDrift_3, pthread_mutex_t *timMut){
    
    Timer *timer;
        
    pthread_mutex_t *timMut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(timMut, NULL);
    
    if (mode == 1) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[0], runtime * (int)1e3 / period[0], stop,
                work_execution, error, fifo, producer, inputDuration, totalDrift_mem, timMut);
        start(timer);
    }
    else if (mode == 2) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[1], runtime * (int)1e3 / period[1], stop,
                work_execution, error, fifo, producer, inputDuration, totalDrift_mem, timMut);
        start(timer);
    }
    else if (mode == 3) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[2], runtime * (int)1e3 / period[2], stop,
                work_execution, error, fifo, producer, inputDuration, totalDrift_mem, timMut);
        start(timer);
    }
    else if (mode == 4) {
        timer = (Timer *)malloc(3 * sizeof(Timer));
        timerInit(&timer[0], period[0], runtime * (int)1e3 / period[0],
                stop, work_execution, error, fifo, producer, inputDuration, periodDrift_1_mem, timMut);
        timerInit(&timer[1], period[1], runtime * (int)1e3 / period[1],
                stop, work_execution, error, fifo, producer, inputDuration, periodDrift_2_mem, timMut);
        timerInit(&timer[2], period[2], runtime * (int)1e3 / period[2],
                stop, work_execution, error, fifo, producer, inputDuration, periodDrift_1_mem, timMut);
        start(&timer[0]);
        start(&timer[1]);
        start(&timer[2]);
    }
}
 
void storeData(int N, FILE *file, int *data) {
    for (int i=0; i<N; i++)
        fprintf(file, "%d,", data[i]);
    fprintf(file, "\n");
} 

// TODO: Modify cons, work functions and integrate them to the current file.

void *producer (void *q)
{
  
    Timer *timer = (Timer *) arg;
  
    sleep(timer->startDelay);
        
    int i;

    struct timeval jobStart, jobEnd, executionStart, executionEnd, temp;
    
    int drifting    = 0;
    int driftCount  = -1;

    for (i = 0; i < timer->tasksToExecute; i++)
    {
        
        // Acquiring timestamps to calculate time drifting
        gettimeofday(&temp, NULL);
        executionStart = executionEnd;
        executionEnd = temp;

        gettimeofday(&jobStart, NULL);
        
        double *x;

        x = &angles[rand()%9];
        
        // Creating a new workFunction Element on each iteration
        workFunction prod;
        prod.work = tim->timerFcn; 
        prod.arg  = (void *) x;  // Passing the address of x to arg with typecast void

        // Locking mutex of queue to start adding elements.
        pthread_mutex_lock(tim->queue->mut);
        
        // Queue Full -> Send error signal
        if(tim->queue->full)
        {
            pthread_mutex_unlock(tim->queue->mut);
            pthread_cond_signal(tim->queue->notEmpty);
            
            // Unlock mutex to start the error function
            pthread_mutex_lock(tim->timMut);
            tim->errorFcn();
            pthread_mutex_unlock(tim->timMut);
        } 
        
        // Queue not full -> Adding elements to queue
        else 
        {
            gettimeofday(&prod.stopwatch, NULL);
            queueAdd(tim->queue, prod);
            gettimeofday(&jobEnd, NULL);


            pthread_mutex_unlock(timer->queue->mut);
            pthread_cond_signal(timer->queue->notEmpty);
            pthread_mutex_lock(timer->tMut);
        
            int inputDuration = (jobEnd.tv_sec - jobStart.tv_sec)*1000000 + (jobEnd.tv_usec - jobStart.tv_usec);
            
            tim->inputDuration[jobsAdded] = inputDuration;
            jobsAdded++;
            
            pthread_mutex_unlock(tim->timMut);
        }
        
        if (i==0)
        {
            usleep(tim->period*1000);
            continue;
        }
        
        totalDrift_mem = (executionEnd.tv_sec - executionStart.tv_sec)*1000000 + executionEnd.tv_usec - executionStart.tv_usec - tim->period*1000;
        
        drifting = totalDrift + totalDrift_mem;
        
        if(drifting > tim->period*1000)
            totalDrift_mem = tim->period*1000;
        else
            totalDrift_mem = drifting;
        
        tim->timDrift[driftCount] = drifting;
        driftCount++;
        
        usleep(tim->period*1000 - timDrift);
    }
        
    

    tim->stopFcn((void *) &tim->period); // Stop timer function starts here
    
    return (NULL);
}

void *consumer (void *q)
{
   
  ArgC *argC = (ArgC *)arg;  

  int i;
  
  struct timeval jobAliveEnd, jobExecStart, jobExecEnd;
  
  workFunction result;


  while(1) {
      
    pthread_mutex_lock (argC->queue-mut);
    
    // TODO: Check if needs a while, instead of an if statement only.
    if(argC->queue->empty)
    {
        pthread_cond_wait(argC->queue->notEmpty, argC->queue->mut);
    }
    
    
    queueDel(argC->queue, &result);
    gettimeofday(&jobAliveEnd, NULL);
    
    pthread_mutex_unlock(argC->queue-mut);
    pthread_cond_signal(argC->queue->notFull);


    // Task Execution starts
    
    gettimeofday(&jobExecStart, NULL); // Calculation Task duration.
    result.work(result.arg); // Running the workFunction!
    gettimeofday(&jobExecEnd, NULL);   // Getting the timestamp after task completion to calculate duration
    
    // Task Execution ends
    
    // Freeing arguments
    free(result.arg);
    
    pthread_mutex_lock(argC->timMut);
    
    
    // Calculating how long a task is alive (Creation -> Execution duration) 
    int jobAliveTime = (jobAliveEnd.tv_sec - result.stopwatch.tv_sec)*1000000 + jobAliveEnd.tv_usec - result.stopwatch.tv_usec;
    
    argC->jobAliveTime[jobsExecuted] = jobAliveTime;
    
    // Calculating how long does the task need to be executed 
    int executionTime = (jobExecEnd.tv_sec - jobExecStart.tv_sec)*1000000 + jobExecEnd.tv_usec - jobExecStart.tv_usec;
    
    argC->jobExecutionTime[jobsExecuted] = executionTime;
    
    jobsExecuted++;
    

    pthread_mutex_unlock(argC->timMut);

    //usleep(200000);
  }
  
  
  return (NULL);
}
    
void *work_execution(void *arg)
 {
     
    double output = cos(*(int*)arg);
    
    //printf("Value of Output is: %f\n", output);
    
 }

void *error()
{
    lostJobs++;
}
// TODO: Add stop, error functions
    
void *stop(void *arg) {
    int *period = (int *) arg;
    printf("Stop operation started by %d millisecond period timer.\n", *period);
}    

// Queue Functions implemented below.

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
