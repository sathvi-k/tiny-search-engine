/* qtest.c --- test queue.c
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
	lqueue_t *lqp;
 
 	car_t *car;
	car_t *car1;
	car_t *car2;
	car_t *car3;
	
	
	lqp=lqopen();
	
	car=make_car(2000,200);
	car1=make_car(2019,1700);
	car2=make_car(1977,200);
	car3=make_car(2003,500);
	
	lqput(lqp,(void*)car);
	lqput(lqp,(void*)car1);
	lqput(lqp,(void*)car2);
	lqput(lqp,(void*)car3);

	//search for something already in the list
	const int y1=2000;                                                                                                                    
  car_t *searchr=(car_t*)lqsearch(lqp,searchfn,(const void*)&y1);
	
	lqapply(lqp,print_anything);
	printf("----------------------\n");
	
	car_t *g1=(car_t*)lqget(lqp);
	lqapply(lqp,print_anything);
	printf("----------------------\n");

	//check for something that has already been removed from the list, so not in the list
	const int y2=2000;                                                                                                                    
  car_t *searchr1=(car_t*)lqsearch(lqp,searchfn,(const void*)&y2);
	
	car_t *g2=(car_t*)lqget(lqp);
	lqapply(lqp,print_anything);
	printf("----------------------\n");
	
	car_t *g3=(car_t*)lqget(lqp);
	lqapply(lqp,print_anything);
	printf("----------------------\n");
	
	car_t *g4=(car_t*)lqget(lqp);
	lqapply(lqp,print_anything);
	printf("----------------------\n");


	//search an empty list
	const int y3=2019;                                                                                                                    
  car_t *searchr2=(car_t*)lqsearch(lqp,searchfn,(const void*)&y3);
	
	if((searchr->year)==2000 && searchr1==NULL && searchr2==NULL){
		free(g1);
		free(g2);
		free(g3);
		free(g4);
		lqclose(lqp);
		exit(EXIT_SUCCESS);
	}
	else{
		free(g1);
		free(g2);
		free(g3);
		free(g4);
		lqclose(lqp);
		exit(EXIT_FAILURE);
	}
}
