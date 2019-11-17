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
#include <stdlib.h>                                                                                                                     
#include <stdio.h>                                                                                                                      
#include <hash.h>                                                                                                                       
#include <queue.h>                                                                                                                      
#include <lqueue.h>                                                                                                                     
#include <lhash.h>                                                                                                                      
#include <string.h>
#include <pthread.h>
#include <unistd.h>

bool good_result=true;
pthread_t tid1;
pthread_t tid2;

typedef struct car_t{
	int year;
  double price;
	char name[100];
}car_t;

typedef struct arguments{
	lhashtable_t *hashtable;
	void *element;
	void (*fn)(void* elementp);
	bool (*searchfn)(void* elementp,const void* searchkeyp);
	const void* key;
	int keylen;
}arguments_t;    

car_t* make_car(int year,double price,char* namep){
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

arguments_t* make_argument(lhashtable_t *hashtable,void *element,void (*fn)(void* elementp),bool (*searchfn)(void* elementp,const void*
																																														 searchkeyp),const void* key, int keylen){

	arguments_t *arg;
	if(!(arg=(arguments_t*)malloc(sizeof(arguments_t)))){
    printf("[Error: malloc failed allocating argument_t]\n");     
    return NULL;                                             
  }

	arg->hashtable=hashtable;
	arg->element=element;
	arg->fn=fn;
	arg->searchfn=searchfn;
	arg->key=key;
	arg->keylen=keylen;
	
	return arg;
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
                                                         
      

void* put(void* arg){
	arguments_t *argt=(arguments_t*)arg;
	int32_t result=lhput(argt->hashtable,argt->element,argt->key,argt->keylen);

	if(result==0){
		;
	}
	else{
		good_result=false;
	}
	return NULL;
}

void t1(arguments_t *arg,void*(*put)(void* arg)){	
	pthread_create(&tid1,NULL,put,(void*)arg);
}

void t2(arguments_t *arg,void*(*put)(void* arg)){
	sleep(2);
	pthread_create(&tid2,NULL,put,(void*)arg);
}

int main(void){
	lhashtable_t *lht=lhopen(15);
	
	car_t *car=make_car(2000,200,"John");                                                                                                 
  car_t *car1=make_car(2019,1700,"Allan");

	const char *ca=car->name;
	const char *cb=car1->name;

	//test lput
	arguments_t *arg1=make_argument(lht,(void*)car,NULL,NULL,ca,strlen(ca));
	arguments_t *arg2=make_argument(lht,(void*)car1,NULL,NULL,cb,strlen(cb));
	t1(arg1,put);
	t2(arg2,put);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	lhapply(lht,print_anything);

	free(car);
	free(car1);
	free(arg1);
	free(arg2);
	lhclose(lht);

	if(good_result){
		exit(EXIT_SUCCESS);
	}
	else{
		exit(EXIT_FAILURE);
	}
}

//REMEMBER TO REMOVE THE SLEEP PUT IN THE LQUEUE.C MODULE!!!!!!!
