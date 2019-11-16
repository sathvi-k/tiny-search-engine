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
#include <lqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_t tid1;
pthread_t tid2;

typedef struct car_t{
  double price;
	int year;
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

arguments_t* make_argument(lqueue_t *queue,void *element,void (*fn)(void* elementp),bool (*searchfn)(void* elementp,const void* keyp),
														const void* skeyp){
	arguments_t *arg;
	if(!(arg=(arguments_t*)malloc(sizeof(arguments_t)))){
    printf("[Error: malloc failed allocating argument_t]\n");     
    return NULL;                                             
  }

	arg->queue=queue;
	arg->element=element;
	arg->fn=fn;
	arg->searchfn=searchfn;
	arg->skeyp=skeyp;
	return arg;
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

void* get(void* arg){
	arguments_t *argt=(arguments_t*)arg;
	lqget(argt->queue);
	return NULL;
}

void t1(arguments_t *arg,void*(*get)(void* arg)){	
	pthread_create(&tid1,NULL,get,(void*)arg);
}

void t2(arguments_t *arg,void*(*get)(void* arg)){
	sleep(2);
	pthread_create(&tid2,NULL,get,(void*)arg);
}

int main(void){
	lqueue_t *lqp=lqopen();
	
	car_t *car=make_car(2000,200);                                                                                                    
  car_t *car1=make_car(2019,1700);

	lqput(lqp,(void*)car);
	lqput(lqp,(void*)car1);

	lqapply(lqp,print_anything);
	printf("--------------------------------\n");
	
	//test lput
	arguments_t *arg1=make_argument(lqp,NULL,NULL,NULL,NULL);
	arguments_t *arg2=make_argument(lqp,NULL,NULL,NULL,NULL);
	t1(arg1,get);
	t2(arg2,get);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	printf("--------------------------------\n");
	lqapply(lqp,print_anything);
	printf("--------------------------------\n");

	car_t *should_be_null=(car_t*)lqget(lqp);
	
	
	if(should_be_null==NULL){
		free(car);
		free(car1);
		free(arg1);
		free(arg2);
		lqclose(lqp);
		exit(EXIT_SUCCESS);
	}
	else{
		free(car);
		free(car1);
		free(arg1);
		free(arg2);
		lqclose(lqp);
		exit(EXIT_FAILURE);
	}
}

//REMEMBER TO REMOVE THE SLEEP PUT IN THE LQUEUE.C MODULE!!!!!!!
