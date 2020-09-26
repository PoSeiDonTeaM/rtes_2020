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
#include <pthread.h>
#include "timer.h"
#include <sys/time.h>
#include <math.h>
#include "queue.h"

#define PI 3.141592654
#define con_threads 3
#define pro_threads 1

#define q_Size 2
#define p_Selection 1
#define r_time 1000

double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 

int inputDuration;
int jobsExecuted;
int lostJobs;
int jobsAdded;
int totalJobs = 0;


volatile int flag = 0;

// Parameter constructor
int  totalJobsCalc(int runtime, int* period, int periodSelection);
void storeData(int N, FILE *file, int *data);
void *producer(void *args);
void *consumer(void *args);
void *work_execution(void *arg);
void *stop(void *arg);
void *error();

typedef struct {
    Queue *queue;
    int totalJobs;
    int *jobAliveTime;
    int *jobsExecuted;
    int *jobExecutionTime;
    int *flag;
    pthread_mutex_t *timMut;
} ArgC;

int main() {
    
    int queueSize       = q_Size;
    int runtime         = r_time;
    int periodSelection = p_Selection;
    int period[3] = {1000, 100, 10};
    
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
 
totalJobs = totalJobsCalc(runtime, period, periodSelection);


// Initialize files to store values
FILE *executionTimeFile = fopen("executionTime.csv", "w");
    
    FILE *totalDriftFile, *periodDrift_1_File, *periodDrift_2_File, *periodDrift_3_File;
    
    if (periodSelection == 4) {
        periodDrift_1_File = fopen("periodDrift_1.csv", "w");
        periodDrift_2_File = fopen("periodDrift_2.csv", "w");
        periodDrift_3_File = fopen("periodDrift_3.csv", "w");
    }else
    {
        totalDriftFile = fopen("totalDrift.csv", "w");
    }
    
    FILE *jobAliveTimeFile = fopen("jobAliveTime.csv", "w");
    
    FILE *inputDurationFile = fopen("inputDuration.csv", "w");

    
    
for(int i = 0; i < con_threads; i++)
{

    
    // Initializes Queue.
    Queue *fifo;
    fifo = queueInit(queueSize);
    if (fifo == NULL) {
        fprintf(stderr, "main: Queue Init failed.\n");
        exit(1);
    }
    
    // Allocate memory to store the values later
    int *jobAliveTime   = (int *)malloc(totalJobs * sizeof(int));
    int *inputDuration  = (int *)malloc(totalJobs * sizeof(int));
    int *executionTime  = (int *)malloc(totalJobs * sizeof(int));
    int *totalDrift_mem, *periodDrift_1_mem, *periodDrift_2_mem, *periodDrift_3_mem;
    
    if(periodSelection == 4)
    {
        
        periodDrift_1_mem  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
        periodDrift_1_mem  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
        periodDrift_1_mem  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
    }
    else
    {
        totalDrift_mem = (int *)malloc(totalJobs * sizeof(int));
    }
    
    ArgC *argC = (ArgC*)malloc(sizeof(ArgC));
    argC->totalJobs         = totalJobs;
    argC->jobAliveTime      = jobAliveTime;
    argC->jobExecutionTime  = executionTime;
    argC->timMut            = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    argC->queue             = fifo;
    
    pthread_mutex_init(argC->timMut, NULL);
    
    pthread_t con[con_threads];
    pthread_t pro[pro_threads];
    
    // Creating consumer threads
    for (int x=0; x<con_threads; x++)
            pthread_create (&con[x], NULL, consumer, argC);
    

    // TIMER CREATION STARTS HERE
    
    Timer *timer;
        
    pthread_mutex_t *timMut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(timMut, NULL);
    
    if (periodSelection == 1) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[0], runtime * (int)1e3 / period[0], stop, work_execution, error, fifo, producer, inputDuration, totalDrift_mem, timMut);
        start(timer);
    }
    else if (periodSelection == 2) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[1], runtime * (int)1e3 / period[1], stop,
                work_execution, error, fifo, producer, inputDuration, totalDrift_mem, timMut);
        start(timer);
    }
    else if (periodSelection == 3) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[2], runtime * (int)1e3 / period[2], stop,
                work_execution, error, fifo, producer, inputDuration, totalDrift_mem, timMut);
        start(timer);
    }
    else if (periodSelection == 4) {
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
    
    // TIMER CREATION ENDS HERE
    
    // Waiting for timers to conclude operations
    if (periodSelection == 4)
    {
        for(int x = 0; x < con_threads; x++)
            pthread_join(timer[x].tid,NULL);
    } else
        pthread_join(timer->tid, NULL);
    
    flag = 1;



    
    pthread_cond_broadcast(fifo->notEmpty);
    
    for (int x=0; x < con_threads; x++)
            pthread_join (con[x], NULL);
    
    
    // Store data to file for later computations
    storeData(jobsExecuted, jobAliveTimeFile,  argC->jobAliveTime);
    storeData(jobsExecuted, executionTimeFile, argC->jobExecutionTime);
    storeData(jobsExecuted, inputDurationFile, inputDuration);
    
    if(periodSelection == 4){
        storeData(runtime * (int)1e3 / period[0] - 1, periodDrift_1_File, periodDrift_1_mem);
        storeData(runtime * (int)1e3 / period[1] - 1, periodDrift_2_File, periodDrift_2_mem);
        storeData(runtime * (int)1e3 / period[2] - 1, periodDrift_3_File, periodDrift_3_mem);
    }else
    {
        storeData(runtime * (int)1e3 / period[periodSelection] - 1, totalDriftFile, totalDrift_mem);
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
    //free(timer);
    
    
    // Deletes Queue.
    queueDelete(fifo);

    pthread_mutex_destroy (argC->timMut);
    //free(argC->timMut);
    //free(argC);
    pthread_mutex_destroy (timMut);
    //free(timMut);

    // Leaving some time for the system to rest, till next iteration.
    usleep(100000);
    
}
    printf("hello");
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


int totalJobsCalc(int runtime, int* period, int periodSelection)
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


void storeData(int N, FILE *file, int *data) {
    
    for (int i=0; i<N; i++)
        fprintf(file, "%d,", data[i]);
    
    fprintf(file, "\n");
} 

// TODO: Modify cons, work functions and integrate them to the current file.

void *producer (void *q)
{
  
    Timer *timer = (Timer *)q;
  
    sleep(timer->startDelay);
        
    int i;

    struct timeval jobStart, jobEnd, executionStart, executionEnd, temp;
    
    int drifting    = 0;
    int driftCount  = 0;

    for (i = 0; i < timer->tasksToExecute; i++)
    {
        
        // Acquiring timestamps to calculate time drifting
        gettimeofday(&temp, NULL);
        executionStart = executionEnd;
        executionEnd = temp;

        gettimeofday(&jobStart, NULL);
        
        double *x = &angles[rand()%9];

        
        // Creating a new workFunction Element on each iteration
        workFunction prod;
        prod.work = timer->timerFcn; 
        prod.arg  = (void *) x;  // Passing the address of x to arg with typecast void

        
        // Locking mutex of queue to start adding elements.
        pthread_mutex_lock(timer->queue->mut);
        
        // Queue Full -> Send error signal
        if(timer->queue->full)
        {
            pthread_mutex_unlock(timer->queue->mut);
            pthread_cond_signal(timer->queue->notEmpty);
            
            // Unlock mutex to start the error function
            pthread_mutex_lock(timer->timMut);
            timer->errorFcn();
            pthread_mutex_unlock(timer->timMut);
        } 
        
        // Queue not full -> Adding elements to queue
        else 
        {
            gettimeofday(&prod.stopwatch, NULL);
            queueAdd(timer->queue, prod);
            gettimeofday(&jobEnd, NULL);


            pthread_mutex_unlock(timer->queue->mut);
            pthread_cond_signal(timer->queue->notEmpty);
            pthread_mutex_lock(timer->timMut);
        
            int inputDuration = (jobEnd.tv_sec - jobStart.tv_sec)*1000000 + (jobEnd.tv_usec - jobStart.tv_usec);
            
            
            timer->input[jobsAdded] = inputDuration;
            jobsAdded++;
            
            pthread_mutex_unlock(timer->timMut);
        }
        
        if (i==0)
        {
            usleep(timer->period*1000);
            continue;
        }
        
        int drift = (executionEnd.tv_sec - executionStart.tv_sec)*1000000 + executionEnd.tv_usec - executionStart.tv_usec - timer->period*1000;
        
        drifting = drifting + drift;
        
        if(drifting > timer->period*1000)
            drift = timer->period*1000;
        else
            drift = drifting;
        
        timer->timDrift[driftCount] = drifting;
        driftCount++;
        
        usleep(timer->period*1000 - drift);
    }
        
    

    timer->stopFcn((void *) &timer->period); // Stop timer function starts here
    
    return (NULL);
}

void *consumer (void *q)
{
   
  ArgC *argC = (ArgC *)q;  

  int i;
  
  struct timeval jobAliveEnd, jobExecStart, jobExecEnd;
  
  workFunction result;


  while(1) {
      
    pthread_mutex_lock (argC->queue->mut);
    
    // TODO: Check if needs a while, instead of an if statement only.
    while(argC->queue->empty)
    {
        pthread_cond_wait(argC->queue->notEmpty, argC->queue->mut);
        
        
         if (flag) {
                pthread_mutex_unlock(argC->queue->mut);
                return NULL;
            }
    }
    

    queueDel(argC->queue, &result);
    gettimeofday(&jobAliveEnd, NULL);
    
    pthread_mutex_unlock(argC->queue->mut);
    pthread_cond_signal(argC->queue->notFull);


    // Task Execution starts
    
    gettimeofday(&jobExecStart, NULL); // Calculation Task duration.
    result.work(result.arg); // Running the workFunction!
    gettimeofday(&jobExecEnd, NULL);   // Getting the timestamp after task completion to calculate duration
    
    // Task Execution ends
    
    // Freeing arguments
    //free(result.arg);
    
    pthread_mutex_lock(argC->timMut);
    
    // TODO: Maybe add mutex between timestamps.
    // Calculating how long a task is alive (Creation -> Execution duration) 
    int jobAliveTime = (jobAliveEnd.tv_sec - result.stopwatch.tv_sec)*1000000 + jobAliveEnd.tv_usec - result.stopwatch.tv_usec;
    
    argC->jobAliveTime[jobsExecuted] = jobAliveTime;
    
    // Calculating how long does the task need to be executed 
    int executionTime = (jobExecEnd.tv_sec - jobExecStart.tv_sec)*1000000 + jobExecEnd.tv_usec - jobExecStart.tv_usec;
    
    argC->jobExecutionTime[jobsExecuted] = executionTime;
    
    jobsExecuted++;
    
    if(jobsExecuted == totalJobs)
        break;

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
