/* lqueuetest.c --- 
 * 
 * 
 * Author: John B. Kariuki Jr., Hanna, Aya, Sathvika
 * Created: Mon Nov 11 18:37:38 2019 (-0500)
 * Version: 1.0
 * 
 * Description: Test a locked-queue and all of its functions for a single thread 
 * 
 */
#include <stdint.h>                                                                                               
#include <stdbool.h>                                                                                                          
#include "lqueue.h"                                                                                               
#include <stdlib.h>                                                                                               
#include <stdio.h>                                                                                                
#include <pthread.h>
#include <unistd.h>

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
	pthread_t tid1,tid2;
	car_t *car=make_car(2000,200);                                                                                                   
  car_t *car1=make_car(2019,1700);                                                                                                 
  car_t *car2=make_car(1999,2);
	car_t *car3=make_car(1907,50);

	//arguments for the happly that will test the pthreads
	arguments_t *arg4=(arguments_t*)malloc(sizeof(arguments_t));
	arg4->queue=lqp;
	arg4->element=NULL;
	arg4->fn=print_anything;
	arg4->searchfn=NULL;
	arg4->skeyp=NULL;
	
	//test lput
	arguments_t *arg1=(arguments_t*)malloc(sizeof(arguments_t));
	arg1->queue=lqp;
	arg1->element=(void*)car;
	arg1->fn=NULL;
	arg1->searchfn=NULL;
	arg1->skeyp=NULL;
		
	//int32_t *res1;
	pthread_create(&tid2,NULL,lqput,(void*)arg1);
	printf(".\n");
	//printf("lqput result 1:%d\n",*res1);
	
	pthread_join(tid2,NULL);
	lqapply((void*)arg4);
	fflush(stdout);
	
	//sleep(4);

	arguments_t *arg2=(arguments_t*)malloc(sizeof(arguments_t));
	arg2->queue=lqp;
	arg2->element=(void*)car1;
	arg2->fn=NULL;
	arg2->searchfn=NULL;
	arg2->skeyp=NULL;
	
	//int32_t *res2;
	pthread_create(&tid1,NULL,lqput,(void*)arg2);
	printf(".\n");
	//printf("lqput result 2:%d\n",*res2);

	pthread_join(tid1,NULL);
	lqapply((void*)arg4);
	fflush(stdout);
	
	//sleep(4);
	
	arguments_t *arg3=(arguments_t*)malloc(sizeof(arguments_t));
	arg3->queue=lqp;
	arg3->element=(void*)car2;
	arg3->fn=NULL;
	arg3->searchfn=NULL;
	arg3->skeyp=NULL;
	
	//int32_t *res3;
	pthread_create(&tid2,NULL,lqput,(void*)arg3);
	printf(".\n");
	//printf("lqput result 3:%d\n",*res3);
	pthread_join(tid2,NULL);
	lqapply((void*)arg4);
	fflush(stdout);
	
	//sleep(4);
	
	arguments_t *arg3i=(arguments_t*)malloc(sizeof(arguments_t));
	arg3i->queue=lqp;
	arg3i->element=(void*)car3;
	arg3i->fn=NULL;
	arg3i->searchfn=NULL;
	arg3i->skeyp=NULL;
	
	//int32_t *res3i=
	pthread_create(&tid1,NULL,lqput,(void*)arg3i);
	printf(".\n");
	//printf("lqput result 3i:%d\n",*res3i);
	
	pthread_join(tid1,NULL);
	lqapply((void*)arg4);
	fflush(stdout);

	
	
	
	//lose the elements of queue using queue apply
	arguments_t *arg5=(arguments_t*)malloc(sizeof(arguments_t));
	arg5->queue=lqp;
	arg5->element=NULL;
	arg5->fn=close_queue_content;
	arg5->searchfn=NULL;
	arg5->skeyp=NULL;
	
	lqapply((void*)arg5);

	//close everything else
	lqclose((void*)arg5);
	free(arg1);
	free(arg2);
	free(arg3);
	free(arg3i);
	free(arg4);
	free(arg5);
	exit(EXIT_SUCCESS);
}
