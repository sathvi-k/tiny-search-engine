/* lhtest1.c --- test the hash.c module
 * 
 * 
 * Author: John B. Kariuki Jr. Hanna , Aya , Sathvika
 * Created: Sat Oct 12 16:33:53 2019 (-0400)
 * Version: 1.0
 * 
 * Description: test if the hash.c module thoroughly 
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <hash.h>
#include <queue.h>
#include <lqueue.h>
#include <lhash.h>
#include <string.h>

typedef struct car_t{
  int year;
  double price;
	char name[100];
}car_t;

static car_t* make_car(int year,double price,char* namep){
  car_t *c;
  if(!(c=(car_t*)malloc(sizeof(car_t)))){
    printf("[Error: malloc failed allocating element]\n");
    return NULL; 
  }
  c->year=year;
  c->price=price;
	strcpy(c->name,namep);
  return c;
}

void print_anything(void *elementp){
  car_t *cp1=(car_t*)elementp;
  printf("this is the cars year %d\n", cp1->year);                            
}

bool searchfn(void* elementp,const void* searchkeyp){
  car_t *cp2=(car_t*)elementp;
	char *pikey; 
  pikey = (char*)searchkeyp;
	char *keystr=pikey;
	char *cp2name=cp2->name;
  //printf("searchkeyp:%p\n",searchkeyp);
	//printf("keyval:%d\n",keyval);
	//printf("year:%d\n",cp2->year);
  if(strcmp(keystr,cp2name)==0){
    return true;
  }
  else{
    return false;
  }
}            

int main(void){
	
	car_t *car;
  car_t *car1;
  car_t *car2;
  car_t *car3;
  car_t *car4;

	car=make_car(2000,200,"John");
	car1=make_car(2019,1700,"Jack");
  car2=make_car(2000,200,"Pat");
	car3=make_car(2003,500,"Tracy");
  car4=make_car(2009,100,"Allan");
	/*	int a = 11;
	int b = 11;
	int c = 7;
	int d = 1;
	int e = 10;*/
	
	const char *ca = car->name;
	const char *cb = car1->name;
	const char *cc = car2->name;
	const char *cd = car3->name;
	const char *ce = car4->name; 
	
	lhashtable_t *lht = lhopen(5);
	
	lhput(lht,(void*)car,ca,sizeof(*ca));
	lhput(lht,(void*)car1,cb,sizeof(*cb));
	lhput(lht,(void*)car2,cc,sizeof(*cc));
	lhput(lht,(void*)car3,cd,sizeof(*cd));
	lhput(lht,(void*)car4,ce,sizeof(*ce));

	car_t *searchr=(car_t*)lhsearch(lht,searchfn,cb,sizeof(*cb));
	printf("search result:%s,%d\n",searchr->name,searchr->year);
	
	lhapply(lht,print_anything);
	lhclose(lht);
}
