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
														const void* skeyp;){
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

void put(void* arg){
	arguments_t *argt=(arguments_t*)arg;
	int32_t *res=lqput(argt->queue,argt->element);
}

void apply(void *arg){
	arguments_t *argt=(arguments_t*)arg;
	lqapply(argt->queue,argt->fn);
}

int main(void){
	lqueue_t *lqp=lqopen();
	
	car_t *car=make_car(2000,200);                                                                                                    
  car_t *car1=make_car(2019,1700);                                                                                                   
  car_t *car2=make_car(1999,2);

	//test lput
	arguments_t *arg1=(arguments_t*)malloc(sizeof(arguments_t));
	arg1->queue=lqp;
	arg1->element=(void*)car;
	arg1->fn=NULL;
	arg1->searchfn=NULL;
	arg1->skeyp=NULL;
		
	int32_t *res1=(int32_t*)lqput((void*)arg1);
	printf("lqput result 1:%d\n",*res1);
	

	arguments_t *arg2=(arguments_t*)malloc(sizeof(arguments_t));
	arg2->queue=lqp;
	arg2->element=(void*)car1;
	arg2->fn=NULL;
	arg2->searchfn=NULL;
	arg2->skeyp=NULL;
	
	int32_t *res2=(int32_t*)lqput((void*)arg2);
	printf("lqput result 2:%d\n",*res2);

	arguments_t *arg3=(arguments_t*)malloc(sizeof(arguments_t));
	arg3->queue=lqp;
	arg3->element=(void*)car2;
	arg3->fn=NULL;
	arg3->searchfn=NULL;
	arg3->skeyp=NULL;
	
	int32_t *res3=(int32_t*)lqput((void*)arg3);
	printf("lqput result 3:%d\n",*res3);

	printf("-----------------------------------------\n");
	
	//test lapply
	arguments_t *arg4=(arguments_t*)malloc(sizeof(arguments_t));
	arg4->queue=lqp;
	arg4->element=NULL;
	arg4->fn=print_anything;
	arg4->searchfn=NULL;
	arg4->skeyp=NULL;
	
	lqapply((void*)arg4);

	printf("----------------------------------------\n");

	//test lget
	arguments_t *arg6=(arguments_t*)malloc(sizeof(arguments_t));
	arg6->queue=lqp;
	arg6->element=NULL;
	arg6->fn=NULL;
	arg6->searchfn=NULL;
	arg6->skeyp=NULL;
	
	car_t *cgot=(car_t*)lqget((void*)arg6);
	lqapply((void*)arg4);

	//test lsearch
	const int y1=1999;
	arguments_t *arg7=(arguments_t*)malloc(sizeof(arguments_t));
	arg7->queue=lqp;
	arg7->element=NULL;
	arg7->fn=NULL;
	arg7->searchfn=searchfn;
	arg7->skeyp=(const void*)&y1;

	car_t *csearched=(car_t*)lqsearch((void*)arg7);

	printf("-----------------------------------\n");
	printf("search result:%d\n",csearched->year);


	//lose the elements of queue using queue apply
	arguments_t *arg5=(arguments_t*)malloc(sizeof(arguments_t));
	arg5->queue=lqp;
	arg5->element=NULL;
	arg5->fn=close_queue_content;
	arg5->searchfn=NULL;
	arg5->skeyp=NULL;
	
	lqapply((void*)arg5);

	free(cgot); //free the rest from lqget

	//close everything else
	lqclose((void*)arg5);
	free(arg1);
	free(arg2);
	free(arg3);
	free(arg4);
	free(arg5);
	free(arg6);
	free(arg7);
	exit(EXIT_SUCCESS);
}
