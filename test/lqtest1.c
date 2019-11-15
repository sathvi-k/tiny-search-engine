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
	car2=make_car(2000,200);
	car3=make_car(2003,500);
	
	int32_t r1=lqput(lqp,(void*)car);
	int32_t r2=lqput(lqp,(void*)car1);
	int32_t r3=lqput(lqp,(void*)car2);
	int32_t r4=lqput(lqp,(void*)car3);
	
	lqapply(lqp,print_anything);
	
	
	
	
	free(car);
	free(car1);
	free(car2);
	free(car3);
	lqclose(lqp);
	
	if(r1==0 && r2==0 && r3==0 && r4==0){
		exit(EXIT_SUCCESS);
	}
	else{
		exit(EXIT_FAILURE);
	}
}
