/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                   *
 * @file         : timer.h                                           *
 *                                                                   *
 * @brief        : Header file of timer.c                            *
 *                                                                   *
 * @author       : Giannis Kotsakiachidis                            *
 *                                                                   *
 * @date         : 20 August 2020                                    *
 *                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include "queue.h"

typedef struct {
    Queue *queue;
    int period;
    int tasksToExecute;
    int startDelay;
    int *input;
    int *timDrift;
    void *(*startFcn)(void *arg);
    void *(*stopFcn)(void *arg);
    void *(*timerFcn)(void *arg);
    void *(*errorFcn)();
    void *userData;
    void *(*prod)(void *arg);
    pthread_t tid;
    pthread_mutex_t *timMut;
} Timer;

void timerInit(Timer *tim, int period, int tasksToExecute, void *(*stopFcn)(void *arg), void *(*timerFcn)(void *arg), void *(*errorFcn)(), Queue *queue, void *(*prod)(void *arg), int *input, int *timDrift, pthread_mutex_t *timMut);

void start(Timer *tim);

void startat(Timer *tim, int d, int m, int y, int h, int min, int sec);
