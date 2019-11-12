/* lqueue.c --- Locked queue module 
 * 
 * 
 * Author: John B. Kariuki Jr., Hanna , Ayazhan , and Sathvika
 * Created: Mon Oct  7 21:39:00 2019 (-0400)
 * Version: 1.0
 * 
 * Description: Creates a locked-queue module that implements the lqueue.h interface
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include "queue.h"
#include "lqueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct ilqueue_t{
  queue_t *queue; 
  pthread_mutex_t mutex;  
}ilqueue_t;

typedef struct arguments{
	lqueue_t *queue;
	void *element;
	void (*fn)(void* elementp);
	bool (*searchfn)(void* elementp,const void* keyp);
	const void* skeyp;
}arguments_t;

/* create an empty locked-queue */
lqueue_t* lqopen(void){
  ilqueue_t *lqp;
  if(!(lqp=(ilqueue_t *)malloc(sizeof(ilqueue_t)))){
    printf("[Error: malloc failed allocating locked-queue]\n");
    return NULL; 
  }
  queue_t *q=qopen();
  pthread_mutex_t mq;
  pthread_mutex_init(&mq,NULL); //initializes the mutex associated with the queue "q" 
  lqp->queue=q;
  lqp->mutex=mq;
  lqueue_t *lqt=(lqueue_t*)lqp;
  return lqt;
}

/* deallocate a locked-queue, frees everything in it */
void* lqclose(void *arg){
  arguments_t *ap=(arguments_t*)arg;
	ilqueue_t *ilqp=(ilqueue_t*)ap->queue;
 
  pthread_mutex_t m=ilqp->mutex;
  //pthread_mutex_unlock(&m); // function below, pthread_mutex_destroy(), fails if mutex is locked
  pthread_mutex_destroy(&m); // frees the resources (queue) allocated for mutex
	qclose(ilqp->queue);
	free(ilqp);
	
	return 0;
}

/* put element at the end of the locked-queue returns 0 if successful;
 * nonzero otherwise
 */
void* lqput(void *arg){
 
	arguments_t *ap=(arguments_t*)arg;
  ilqueue_t *ilqp=(ilqueue_t*)ap->queue;
	void *elementp=ap->element;
	
  pthread_mutex_t m=ilqp->mutex;
  pthread_mutex_lock(&m);
  int32_t returned=qput(ilqp->queue,elementp);
  pthread_mutex_unlock(&m);

	int32_t *return_value=&returned;
	
  return return_value;
}

/* get the first element from locked-queue, removing it from the locked-queue */
void* lqget(void *arg){
	arguments_t *ap=(arguments_t*)arg;
  ilqueue_t *ilqp=(ilqueue_t*)ap->queue;
	
  void* ep;
  pthread_mutex_t m=ilqp->mutex;
  pthread_mutex_lock(&m);
  ep=qget(ilqp->queue);
  pthread_mutex_unlock(&m);
  return ep;
}

/* apply a function to every element of the locked-queue */ 
void* lqapply(void* arg){
	arguments_t *ap=(arguments_t*)arg;
  ilqueue_t *ilqp=(ilqueue_t*)ap->queue;
	
  pthread_mutex_t m=ilqp->mutex;
  pthread_mutex_lock(&m);
  qapply(ilqp->queue,ap->fn);
  pthread_mutex_unlock(&m);

	return 0;
}

/* search a locked-queue using a supplied boolean function 
 * skeyp -- a key to search for 
 * searchfn -- a function applied to every element of the locked-queue 
 *          -- elementp - a pointer to an element 
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h 
 * returns a pointer to an element, or NULL if not found
 */ 
void* lqsearch(void* arg){
	arguments_t *ap=(arguments_t*)arg;
  ilqueue_t *ilqp=(ilqueue_t*)ap->queue;
	
  pthread_mutex_t m=ilqp->mutex;
  pthread_mutex_lock(&m);
  void* lsearch=qsearch(ilqp->queue,ap->searchfn,ap->skeyp);
  pthread_mutex_unlock(&m);
  return lsearch;
}
