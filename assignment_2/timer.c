/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                   *
 * @file         : timer.c                                           *
 *                                                                   *
 * @brief        : Function that creates timer structs to be used in *
 *                 main function                                     *
 *                                                                   *
 * @author       : Giannis Kotsakiachidis                            *
 *                                                                   *
 * @date         : 20 August 2020                                    *
 *                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "timer.h"

// Timer Initialization

void timerInit(Timer *tim, int period, int tasksToExecute, void *(*stopFcn)(void *arg), void *(*timerFcn)(void *arg), void *(*errorFcn)(), Queue *queue, void *(*prod)(void *arg), int *input, int *timDrift, pthread_mutex_t *timMut){
    
    tim-> period            = period;         
    tim-> tasksToExecute    = tasksToExecute;
    tim-> startDelay        = 0;
    tim-> startFcn          = NULL;
    tim-> stopFcn           = stopFcn;
    tim-> timerFcn          = timerFcn;
    tim-> errorFcn          = errorFcn;
    tim-> userData          = NULL;

    
    tim-> input             = input;
    tim-> timDrift          = timDrift;
    tim-> queue             = queue;
    tim-> prod              = prod;
    tim-> timMut            = timMut;
    
}

// Starting timer instantly

void start(Timer *tim) {
    pthread_create(&tim->tid, NULL, tim->prod, tim);
}

// Starting timer at a specific date/time

void startat(Timer *tim, int d, int m, int y, int h, int min, int sec)
{
    int delay = 0;
    
    // Initialization of timer struct to specify when the timer starts
    struct tm init;
    
    init.tm_mday = d;
    init.tm_mon  = m;
    init.tm_year = y;
    init.tm_hour = h;
    init.tm_min  = min;
    init.tm_sec  = sec;
    
    // Calculate the delay to compensate between current time and desired execution time
    
    time_t now = time(NULL);
    delay = (int) difftime(now, mktime(&init)); // Taking the difference between the two timestamps.
    
    if(delay > 0)
    {
        tim->startDelay = delay;
    } else
    {
        printf("Initialization Error! You are trying to initialize the timer at a past time.");
        delay = 0;                     // Adding a zero delay to prevent errors.
        tim->startDelay = delay;
    }
    
    pthread_create(&tim->tid, NULL, tim->prod, tim);
}

