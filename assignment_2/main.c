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

int jobsAdded;
int jobsExecuted;
int lostJobs;

// Parameter constructor

void paramInit(){
    
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
    
    FILE *executionTime = fopen("executionTime.csv", "w");
    
    FILE *totalDrift, *periodDrift_1, *periodDrift_2, *periodDrift_3;
    
    if (periodSelection == 4) {
        periodDrift_1 = fopen("periodDrift_1.csv", "w");
        periodDrift_2 = fopen("periodDrift_2.csv", "w");
        periodDrift_3 = fopen("periodDrift_3.csv", "w");
    }else
    {
        totalDrift = fopen("totalDrift.csv", "w");
    }
    
    FILE *jobAliveTime = fopen("jobAliveTime.csv", "w");
    
    FILE *inputTime = fopen("inputTime.csv", "w");
    
}

int totalJobsCalc(runtime, period[], periodSelection)
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
    
}
    

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
    
    
    
}

}
    
    
    
    
    
    
    
