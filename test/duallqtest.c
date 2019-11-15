/* duallqtest.c --- test queue.c
 * 
 * 
 * Author: John B. Kariuki Jr. Hanna, Aya, Sathvika
 * Created: Tue Oct  8 20:40:28 2019 (-0400)
 * Version: 1.0
 * 
 * Description: Test queue.c
 * 
 */
#include <stdint.h>           
#include <stdbool.h>                          
#include <lqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


lqueue_t *lqp;


typedef struct car_t{
	int year;
	double price;
}car_t;


static car_t* make_car(int year,double price){
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
	pthread_t tid1, tid2;

 	car_t *car;
	car_t *car1;
	car_t *car2;
	car_t *car3;
		
	lqp=lqopen();
	
	car=make_car(2000,200);
	car1=make_car(2019,1700);
	car2=make_car(2000,200);
	car3=make_car(2003,500);
	
	lqput(lqp,(void*)car);
	lqput(lqp,(void*)car1);
	lqput(lqp,(void*)car2);
	lqput(lqp,(void*)car3);

	printf("Queue initially: \n");
	lqapply(lqp,print_anything);
		 
	if(pthread_create(&tid1,NULL,lqget,lqp)!=0)
		exit(EXIT_FAILURE);
	
	if(pthread_join(tid1,NULL)!=0)                                      
    exit(EXIT_FAILURE);

	printf("Queue after first thread: \n");                                    
	lqapply(lqp,print_anything);
	
	if(pthread_create(&tid2,NULL,lqget,lqp)!=0)
		exit(EXIT_FAILURE);

	if(pthread_join(tid2,NULL)!=0)
		exit(EXIT_FAILURE);

	printf("Queue after second thread: \n");              
  lqapply(lqp,print_anything);  

	free(car);
	free(car1);
	free(car2);
	free(car3);
	lqclose(lqp);
	exit(EXIT_SUCCESS);
}
