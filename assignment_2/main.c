/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                   *
 * @file         : main.c                                            *
 *                                                                   *
 * @brief        : The main function that uses the timer function    * 
 *                 to assign tasks to producers and consumers.       *
 *                                                                   *
 * @author       : Giannis Kotsakiachidis                            *
 *                                                                   *
 * @date         : 20 August 2020                                    *
 *                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include "queue.h"
#include "timer.h"

#define PI 3.141592654

#define con_threads 33


int jobsAdded;
int jobsExecuted;
int lostJobs;

// Parameter constructor

void paramInit(queueSize);
void fileInit();
void memoryInit(runtime, period[], periodSelection);
void totalJobsCalc(runtime, period[], periodSelection);
void timCreation(period[], runtime, jobsAdded, totalDrift, periodDrift_1, periodDrift_2, periodDrift_3, timMut);
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
    void timCreation(period[], runtime, jobsAdded, totalDrift, periodDrift_1, periodDrift_2, periodDrift_3, timMut);
    
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
    storeData(jobsExecuted, inputTimeFile, inputTime);
    
    if(periodSelection == 4){
        storeData(runtime * (int)1e3 / period[0] - 1, periodDrift_1_File, periodDrift_1);
        storeData(runtime * (int)1e3 / period[1] - 1, periodDrift_2_File, periodDrift_2);
        storeData(runtime * (int)1e3 / period[2] - 1, periodDrift_3_File, periodDrift_3);
    }else
    {
        storeData(jobsAdded - 1, totalDriftFile, totalDrift);
    }
    
    
}

}

void paramInit(int queueSize){
    
int jobsAdded       = 0;
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
    
    FILE *inputTimeFile = fopen("inputTime.csv", "w");
    
}

void memoryInit(int runtime, int period[], int periodSelection){
    
    int *jobAliveTime   = (int *)malloc(totalJobs * sizeof(int));
    int *inputTime      = (int *)malloc(totalJobs * sizeof(int));
    int *executionTime  = (int *)malloc(totalJobs * sizeof(int));
    
    if(periodSelection == 4)
    {
        
        int *periodDrift_1  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
        int *periodDrift_1  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
        int *periodDrift_1  = (int *)malloc(runtime * (int)1e3 / period[0] * sizeof(int));
        
    }
    else
    {
        totalDrift = (int *)malloc(totalJobs * sizeof(int));
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

void timCreation(int period[], int runtime, int jobsAdded, int totalDrift, int periodDrift_1, int periodDrift_2, int periodDrift_3, pthread_mutex_t *timMut){
    
    Timer *timer;
        
    pthread_mutex_t *timMut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(timMut, NULL);
    
    if (mode == 1) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[0], runtime * (int)1e3 / period[0], stop,
                work, error, fifo, producer, jobsAdded, totalDrift, timMut);
        start(timer);
    }
    else if (mode == 2) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[1], runtime * (int)1e3 / period[1], stop,
                work, error, fifo, producer, jobsAdded, totalDrift, timMut);
        start(timer);
    }
    else if (mode == 3) {
        timer = (Timer *)malloc(sizeof(Timer));
        timerInit(timer, period[2], runtime * (int)1e3 / period[2], stop,
                work, error, fifo, producer, jobsAdded, totalDrift, timMut);
        start(timer);
    }
    else if (mode == 4) {
        timer = (Timer *)malloc(3 * sizeof(Timer));
        timerInit(&timer[0], period[0], runtime * (int)1e3 / period[0],
                stop, work, error, fifo, producer, jobsAdded, periodDrift_1, timMut);
        timerInit(&timer[1], period[1], runtime * (int)1e3 / period[1],
                stop, work, error, fifo, producer, jobsAdded, periodDrift_2, timMut);
        timerInit(&timer[2], period[2], runtime * (int)1e3 / period[2],
                stop, work, error, fifo, producer, jobsAdded, periodDrift_1, timMut);
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

    
    
    
    
