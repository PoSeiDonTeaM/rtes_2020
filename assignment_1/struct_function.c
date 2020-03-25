#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592654

/* START OF THE WORK FUNCTIONS */

void work1(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work1 is: %f\n", output);
 }
 
 void work2(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work2 is: %f\n", output);
 }
 
 void work3(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work3 is: %f\n", output);
 }
 
 void work4(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work4 is: %f\n", output);
 }
 
 void work5(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work5 is: %f\n", output);
 }
 
 void work6(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work6 is: %f\n", output);
 }
 
 void work7(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work7 is: %f\n", output);
 }
 
 void work8(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work8 is: %f\n", output);
 }
 
 void work9(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work9 is: %f\n", output);
 }
 
 void work10(int arg)
 {
    double angles[10] = {PI/4, PI, PI/2, PI/6, PI/3, -PI, -PI/4, -PI/2, -PI/6, -PI/3}; 
     
    double output = cos(angles[arg]);
    
    printf("Value of work10 is: %f\n", output);
 }

/* END OF THE WORK FUNCTIONS */


int main ()
{
    int i;
    
    i = rand()%9;
    
    /* Creating a pointer array that stores the addresses of all the work functions to be used by the struct */
    void (*work_ptr[])(int) = {&work1, &work2, &work3, &work4, &work5, &work6, &work7, &work8, &work9, &work10};
    
    (*work_ptr[i])(i);
    
}

struct workFunction {
  void * (*work)(void *);
  void * arg;
};

