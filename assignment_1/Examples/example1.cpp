
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int sum=0;
void *add(void *);
pthread_mutex_t mut;

int main()
{
    pthread_t thread[10];
    int num;
    long count;

    cout << "Enter the number of thread (1-10):";
    cin  >> num;
    
    cout << "Enter number to count to: ";
    cin  >> count;
    
    for (int x=0; x<num; x++)
        pthread_create(&thread[x], NULL, add, (void *) count);
    
    for (int x=0; x<num; x++)
        pthread_join(thread[x], NULL);
    
    cout << sum << endl;
    return 0;
}

void *add (void *count)
{
    long num;
    num = (long) count;

    pthread_mutex_lock(&mut); 
    for(long x=1; x<num; x++)
    {
        sum+=x;
        cout << sum << '\t' << x << endl;
        
    }
    pthread_mutex_unlock(&mut);
}

