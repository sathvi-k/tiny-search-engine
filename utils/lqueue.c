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
#include <queue.h>
#include <lqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  

typedef struct ilqueue_t{
  queue_t *queue; 
  pthread_mutex_t mutex;  
}ilqueue_t;

/* create an empty locked-queue */
lqueue_t* lqopen(void){
  ilqueue_t *lqp;
 	
	if(!(lqp=(ilqueue_t *)malloc(sizeof(ilqueue_t)))){
    printf("[Error: malloc failed allocating locked-queue]\n");
    return NULL; 
  }
	
  lqp->queue=qopen();
  pthread_mutex_init(&(lqp->mutex),NULL);
  lqueue_t *lqt=(lqueue_t*)lqp;
  return lqt;
}

/* deallocate a locked-queue, frees everything in it */
void lqclose(lqueue_t *lqp){
	ilqueue_t *ilqp=(ilqueue_t*)lqp;
	pthread_mutex_lock(&(ilqp->mutex));
	qclose(ilqp->queue);
	pthread_mutex_unlock(&(ilqp->mutex)); // function below, pthread_mutex_destroy(), fails if mutex is locked
	pthread_mutex_destroy(&(ilqp->mutex)); // frees the resources (queue) allocated for mutex
	free(ilqp);
}

/* put element at the end of the locked-queue returns 0 if successful;
 * nonzero otherwise
 */
int32_t lqput(lqueue_t *lqp, void *elementp){
	int32_t return_value;
  ilqueue_t *ilqp=(ilqueue_t*)lqp;
	pthread_mutex_lock(&(ilqp->mutex));
  return_value=qput(ilqp->queue,elementp);

	//printf("put\n");
	//fflush(stdout);
	//sleep(10);
	
  pthread_mutex_unlock(&(ilqp->mutex));
  return return_value;
}

/* get the first element from locked-queue, removing it from the locked-queue*/
void* lqget(lqueue_t *lqp){
  ilqueue_t *ilqp=(ilqueue_t*)lqp;
  void* ep;
  pthread_mutex_lock(&(ilqp->mutex));
	ep=qget(ilqp->queue);
	
	//printf("got \n");
	//fflush(stdout);
	//sleep(10);
	
  pthread_mutex_unlock(&(ilqp->mutex));
  return ep;
}

/* apply a function to every element of the locked-queue */ 
void lqapply(lqueue_t *lqp, void (*fn)(void* elementp)){
	ilqueue_t *ilqp=(ilqueue_t*)lqp;
  pthread_mutex_lock(&(ilqp->mutex));
	qapply(ilqp->queue,fn);
	
	//printf("applied\n");
  //fflush(stdout);
	//sleep(10);
	
  pthread_mutex_unlock(&(ilqp->mutex));
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
void* lqsearch(lqueue_t *lqp, bool (*searchfn)(void* elementp, const void* keyp), const void* skeyp){
  ilqueue_t *ilqp=(ilqueue_t*)lqp;
  pthread_mutex_lock(&(ilqp->mutex));
  void* lsearch=qsearch(ilqp->queue,searchfn,skeyp);

	//printf("searched\n");
	//fflush(stdout);
	//sleep(10);
	
  pthread_mutex_unlock(&(ilqp->mutex));
  return lsearch;
}

//check with the rest of the team
void lqsnp(lqueue_t *lqp,void *element, bool (*searchfn)(void* elementp, const void* keyp), const void* skeyp){
	ilqueue_t *ilqp=(ilqueue_t*)lqp;
	pthread_mutex_lock(&(ilqp->mutex));
	void* ep=qsearch(ilqp->queue,searchfn,skeyp);
	if(ep==NULL){
		qput(ilqp->queue,element);
	}
	pthread_mutex_unlock(&(ilqp->mutex));
}
