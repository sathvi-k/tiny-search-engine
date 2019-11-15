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
	lqueue_t *lq2p;
	
	car_t *car;
	car_t *car1;
	car_t *car2;
	car_t *car3;
	car_t *car4;
	car_t *car5;
	car_t *car6;
	
	lqp=lqopen();
	car=make_car(2000,200);
	car1=make_car(2019,1700);
	car2=make_car(2000,200);
	lqput(lqp,(void*)car);
	lqput(lqp,(void*)car1);
	lqput(lqp,(void*)car2);

	lq2p=lqopen();
	car3=make_car(2003,500);
	car4=make_car(2009,100);
	car5=make_car(2011,7000);
	car6=make_car(2022,9000);
	lqput(lq2p,(void*)car3);
	lqput(lq2p,(void*)car4);
	lqput(lq2p,(void*)car5);
	lqput(lq2p,(void*)car6); 
	
	lqapply(lqp,print_anything);
	printf("---------------\n");
	lqapply(lq2p,print_anything);
	printf("---------------\n");
	
	//const	int y1=2000;
	const int y2=2000;
	//car_t *remover=(car_t*)qremove(qp,searchfn,(const void*)&y1);
	car_t *searchr=(car_t*)lqsearch(lqp,searchfn,(const void*)&y2);
	
	//car_t *rcar=(car_t*)qget(qp);
	//car_t *rcar1=(car_t*)qget(qp);
	//car_t *rcar2=(car_t*)qget(qp);
	//car_t *rcar3=(car_t*)qget(qp);
	
	//qconcat(q2p,qp);
	
	printf("---------------\n");
	lqapply(lqp,print_anything);
	//	printf("--------------\n");
	//	qapply(q2p,print_anything);
	//printf("search result:%d\n",remover->year);
	printf("search result:%f\n",searchr->price);
	/*
	const	int y1=2000;
	car_t *searchr=(car_t*)qsearch(qp,searchfn,(const void*)&y1);
	printf("search result:%d\n",searchr->year);
	car_t *remover=(car_t*)qremove(qp,searchfn,(const void*)&y1);
	printf("search result:%d\n",remover->year);
	qapply(qp,print_anything);
	*/
	
	/*car_t *rcar=(car_t*)qget(qp);
	car_t *rcar1=(car_t*)qget(qp);
	qapply(qp,print_anything);

	qp1=qopen();
	car2=make_car(1999,17);
	car3=make_car(2007,54);
	qput(qp1,(void*)car2);
	qput(qp1,(void*)car3);
	qapply(qp1,print_anything);
	car_t *rcar2=(car_t*)qget(qp1);
	car_t *rcar3=(car_t*)qget(qp1);
	qapply(qp1,print_anything);*/
	
	//car_t *rcar3=(car_t*)qget(qp);

	//printf("%d\n",rcar->year);
	//printf("%f\n",rcar1->price);
	
	//if(rcar3==NULL){
	//	printf("NULL\n");
	//	}
	//free(rcar);
	//free(rcar1);
	//free(rcar2);
	//free(rcar3);
	//free(remover);
	
	free(car);
	free(car1);
	free(car2);
	free(car3);
	free(car4);
	free(car5);
	free(car6);
	lqclose(lqp);
	lqclose(lq2p);
	exit(EXIT_SUCCESS);
}
