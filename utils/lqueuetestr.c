/* lqueuetest.c --- 
 * 
 * 
 * Author: John B. Kariuki Jr., Hanna, Aya, Sathvika
 * Created: Mon Nov 11 18:37:38 2019 (-0500)
 * Version: 1.0
 * 
 * Description: Test a locked-queue and all of its functions 
 * 
 */
#include <stdint.h>                                                                                               
#include <stdbool.h>                                                                                                          
#include "lqueue.h"                                                                                               
#include <stdlib.h>                                                                                               
#include <stdio.h>                                                                                                
#include <pthread.h>

typedef struct car_t{                                                                                                                   
  int year;                                                                                                                             
  double price;                                                                                                                         
}car_t;

typedef struct arguments{                                                                                                               
  lqueue_t *queue;                                                                                                                       
  void *element;                                                                                                                        
  void (*fn)(void* elementp);                                                                                                           
  bool (*searchfn)(void* elementp,const void* keyp);                                                                                    
  const void* skeyp;                                                                                                                    
}arguments_t;    

car_t* make_car(int year,double price){                                                                                          
                                                                                                                                        
  car_t *c;                                                                                                                             
                                                                                                                                        
  if(!(c=(car_t*)malloc(sizeof(car_t)))){                                                                                               
    printf("[Error: malloc failed allocating element]\n");                                                                              
    return NULL;                                                                                                                        
  }                                                                                                                                     
  c->year=year;                                                                                                                         
  c->price=price;                                                                                                                       
  return c;                                                                                                                             
}
                                                                                                                                        
void print_anything(void *elementp){                                                                                                    
  car_t *cp1=(car_t*)elementp;                                                                                                          
  printf("this is the cars price %f\n", cp1->price);                                                                                    
}

void close_queue_content(void *elementp){
	car_t *cp1=(car_t*)elementp;
	free(cp1);
}
                                                                                                                                        
bool searchfn(void* elementp,const void* keyp){                                                                                         
  car_t *cp2=(car_t*)elementp;                                                                                                          
  int *pikey;                                                                                                                           
                                                                                                                                        
  pikey = (int*)keyp;                                                                                                                   
                                                                                                                                        
  const int keyval = *pikey;                                                                                                            
                                                                                                                                        
  if((cp2->year)==keyval){                                                                                                              
    return true;                                                                                                                        
  }                                                                                                                                     
  else{                                                                                                                                 
    return false;                                                                                                                       
  }                                                                                                                                     
}      

int main(void){
	lqueue_t *lqp=lqopen();
	pthread_t tid1;

	car_t *car=make_car(2000,200);                                                                                                    
  car_t *car1=make_car(2019,1700);                                                                                                   
  car_t *car2=make_car(1999,2);

	arguments_t *arg1=(arguments_t*)malloc(sizeof(arguments_t));
	arg1->queue=lqp;
	arg1->element=(void*)car;
	arg1->fn=NULL;
	arg1->searchfn=NULL;
	arg1->skeyp=NULL;
		
	if(pthread_create(&tid1,NULL,lqput,(void*)arg1)!=0){
		exit(EXIT_FAILURE);
	}
	

	arguments_t *arg2=(arguments_t*)malloc(sizeof(arguments_t));
	arg1->queue=lqp;
	arg1->element=(void*)car1;
	arg1->fn=NULL;
	arg1->searchfn=NULL;
	arg1->skeyp=NULL;
	
	if(pthread_create(&tid1,NULL,lqput,(void*)arg2)!=0){
		exit(EXIT_FAILURE);
	}
	

	arguments_t *arg3=(arguments_t*)malloc(sizeof(arguments_t));
	arg1->queue=lqp;
	arg1->element=(void*)car2;
	arg1->fn=NULL;
	arg1->searchfn=NULL;
	arg1->skeyp=NULL;
	
	if(pthread_create(&tid1,NULL,lqput,(void*)arg3)!=0){
		exit(EXIT_FAILURE);
	}
	

	arguments_t *arg4=(arguments_t*)malloc(sizeof(arguments_t));
	arg1->queue=lqp;
	arg1->element=NULL;
	arg1->fn=print_anything;
	arg1->searchfn=NULL;
	arg1->skeyp=NULL;
	
	if(pthread_create(&tid1,NULL,lqapply,(void*)arg4)!=0){
		exit(EXIT_FAILURE);
	}
	

	arguments_t *arg5=(arguments_t*)malloc(sizeof(arguments_t));
	arg1->queue=lqp;
	arg1->element=NULL;
	arg1->fn=close_queue_content;
	arg1->searchfn=NULL;
	arg1->skeyp=NULL;
	
	if(pthread_create(&tid1,NULL,lqapply,(void*)arg5)!=0){
		exit(EXIT_FAILURE);
	}
 

	free(arg1);
	free(arg2);
	free(arg3);
	free(arg4);
	free(arg5);
	lqclose(lqp);
	exit(EXIT_SUCCESS);
}
