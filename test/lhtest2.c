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
 	
	const char *ca = car->name;
	const char *cb = car1->name;
	const char *cc = car2->name;
	const char *cd = car3->name;
	const char *ce = car4->name;
	const char *cf= "does not exist";
	
	lhashtable_t *lht = lhopen(5);
	
	lhput(lht,(void*)car,ca,strlen(ca));
	lhput(lht,(void*)car1,cb,strlen(cb));
	lhput(lht,(void*)car2,cc,strlen(cc));
	lhput(lht,(void*)car3,cd,strlen(cd));
	lhput(lht,(void*)car4,ce,strlen(ce));
	
	lhapply(lht,print_anything);

	printf("--------------------------\n");
	
	car_t *searchr=(car_t*)lhsearch(lht,searchfn,cb,strlen(cb));
	car_t *not_there=(car_t*)lhsearch(lht,searchfn,cf,strlen(cf));

	printf("search result:%s\n",searchr->name);
	
	if((strcmp(searchr->name,"Jack")==0) && not_there==NULL){
		free(car);
		free(car1);
		free(car2);
		free(car3);
		free(car4);
		lhclose(lht);
		exit(EXIT_SUCCESS);
	}
	else{
		free(car);
		free(car1);
		free(car2);
		free(car3);
		free(car4);
		lhclose(lht);
		exit(EXIT_FAILURE);
	}
}
