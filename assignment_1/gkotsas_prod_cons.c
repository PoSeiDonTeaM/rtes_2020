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
#include <sys/time.h>

#define pro_threads 100
#define con_threads 50
#define MAX_QUEUE_SIZE 100
#define MAX_LOOPS 80000

using namespace std; 

// Function declaration of all required functions
void *producer (void *args);
void *consumer (void *args);

// Shared queue 
queue<int> Q; 

int sum = 0;
int consumerCount = 0;

struct timeval t1, t2, start_program, end_program;


int elapsedTime[MAX_LOOPS*pro_threads];

// Getting the mutex 
pthread_mutex_t mutex           = PTHREAD_MUTEX_INITIALIZER; 

pthread_cond_t  dataNotProduced = PTHREAD_COND_INITIALIZER; 
pthread_cond_t  dataNotConsumed = PTHREAD_COND_INITIALIZER; 

int main ()
{
  double total_elapsed_time = 0;  
  gettimeofday(&start_program, NULL);
  
  double mean_elapsed_time = 0;
    
  pthread_t pro[pro_threads], con[con_threads];
    
  
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
  
  cout << "\nWow, you made it to the end sailor! Congrats.\n" << endl;
  
  gettimeofday(&end_program, NULL);
  total_elapsed_time =  (end_program.tv_sec - start_program.tv_sec)*1000;
  total_elapsed_time += (end_program.tv_usec - start_program.tv_usec)/1000;
  
  cout << "Total program elapsed time is: " << total_elapsed_time << " ms\n" << endl;
  
  for (int x=0; x<MAX_LOOPS*pro_threads; x++)
      mean_elapsed_time += elapsedTime[x];
      
  mean_elapsed_time = mean_elapsed_time/(MAX_LOOPS*pro_threads);
  
  cout << "The mean value of the elapsed time between a producer thread and a consumer one is: " << mean_elapsed_time << " us\n" << endl;
  return 0;
}

void *producer (void *q)
{
  static int producerCount = 0;  

  

  while(1) {
      
    pthread_mutex_lock (&mutex); // Locking queue with mutex
    
    if (Q.size() < MAX_QUEUE_SIZE && producerCount < MAX_LOOPS*pro_threads)
    {
        // Generating a random number from 0~9 to be used in function selection
        int num = rand()%9;
        cout << "Producer with id(" << pthread_self() << "): Function " << num << " has been selected" << endl;
        
        // Pushing the number into queue
        Q.push(num);
        
        // Starting the timer
        gettimeofday(&t1, NULL);
        
        cout << t1.tv_sec << endl;
        
        producerCount++;
        
        pthread_cond_broadcast(&dataNotProduced);
    }
    
    // If queue is full release mutex and break the loop
    else if (producerCount == MAX_LOOPS*pro_threads)
    {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    
    // If another thread is executing, then wait.
    else{
        cout << "Producer: Hold on there cowboy, another thread is executing..." << endl;
        cout << "Queue size is: " << Q.size() << endl;
        
        pthread_cond_wait(&dataNotConsumed, &mutex);
    }
    
    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
    

  }
}

void *consumer (void *q)
{
    

  while(1) {
      
    pthread_mutex_lock (&mutex);
    
    if(Q.size() > 0 && consumerCount < MAX_LOOPS*pro_threads)
    {
        // Get the data from the front of the queue
        int data = Q.front();
        
        // Stopping the timer
        gettimeofday(&t2, NULL);
        
        // Saving the elapsedTime in an array
        elapsedTime[consumerCount]   =  (t2.tv_usec - t1.tv_usec); // sec to ms
        
        cout << "Hey, I'm Consumer-" << pthread_self() << ". I'm using the function: " << data << endl;
        
        // Selecting the right function
        // At this stage, we just sum the data and integrate the extra functions later.
        sum += data;
        
        // Pop the consumed data from the queue 
        Q.pop();
        
        consumerCount++;
        
        pthread_cond_signal(&dataNotConsumed);
    }
    
    // Check if consumers have consumed all the threads that have been produced
    else if(consumerCount == MAX_LOOPS*pro_threads)
    {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    
    // If another thread is executing, then wait.
    else{
        cout << "Consumer: Hold on there cowboy, another thread is executing..." << endl;
        pthread_cond_wait(&dataNotConsumed, &mutex);
    }
    
    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
  }
}
