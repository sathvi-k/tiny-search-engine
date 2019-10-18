/* queue.c --- Create a queue 
 * 
 * 
 * Author: John B. Kariuki Jr., Hanna , Aya , and Sathvika
 * Created: Mon Oct  7 21:39:00 2019 (-0400)
 * Version: 1.0
 * 
 * Description: Creates a queue module that implements the queue.h interface
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct element_t{
	struct element_t *next;
	void *data;
}element_t;

typedef struct iqueue_t{
	element_t *front;
	element_t *back;
}iqueue_t;

static element_t* create_element(void *elementp){              
 	element_t *et;
  if(!(et=(element_t *)malloc(sizeof(element_t)))){
    printf("[Error: malloc failed allocating element]\n");
    return NULL;
  }
  et->next=NULL;
	et->data=elementp;
  return et;
}

/* create an empty queue */
queue_t* qopen(void){               
	iqueue_t *qp;
  if(!(qp=(iqueue_t *)malloc(sizeof(iqueue_t)))){
    printf("[Error: malloc failed allocating queue]\n");
    return NULL; 
  }
  qp->front=NULL;
	qp->back=NULL;
	queue_t *qt=(void*)qp;
	return qt;
}

/* deallocate a queue, frees everything in it */
void qclose(queue_t *qp){
	iqueue_t *iqp=(iqueue_t*)qp;
	element_t *i=iqp->front;
	element_t *save=NULL;
	
	while(i!=NULL){
		save=i->next;
		//free(i->data);
		free(i);
		i=save;
	}
	free(iqp);
}

/* put element at the end of the queue returns 0 if successful;
 * nonzero otherwise
 */
int32_t qput(queue_t *qp,void *elementp){
	element_t *element=create_element(elementp);
	iqueue_t *iqp=(iqueue_t*)qp;
	int32_t return_var=1;
	
	if((iqp->front==NULL) && (iqp->back==NULL)){
		iqp->front=element;
		iqp->back=element;
		return_var=0;
		//return 0;
	}
	else{
		iqp->back->next=element;
		iqp->back=element;
		return_var=0;
		//return 0;
	}
	return return_var;
}

/* get the first element from queue, removing it from the queue */
void* qget(queue_t *qp){
	element_t *save=NULL;
	iqueue_t *iqp=(iqueue_t*)qp;
	void* sdata;
	
	if(iqp->front!=NULL && iqp->back!=NULL){
		save=iqp->front;
		sdata=save->data;
		iqp->front=iqp->front->next;
		if(iqp->front==NULL){
			iqp->back=NULL;
		}
	}
	if(save==NULL){
		return NULL;
	}
	free(save);
	return sdata;
}

/* apply a function to every element of the queue */ 
void qapply(queue_t *qp, void (*fn)(void* elementp)){
	iqueue_t *iqp=(iqueue_t*)qp;
	if (iqp->front!=NULL && iqp->back!=NULL){
		element_t *ep;
		for(ep=iqp->front; ep!=NULL;ep=ep->next){
			//fn(ep->data);
			(*fn)(ep->data);
		}
	}
	else{
		printf("The list is empty, can't apply the function\n");
	}
}

/* search a queue using a supplied boolean function 
 * skeyp -- a key to search for 
 * searchfn -- a function applied to every element of the queue 
 *          -- elementp - a pointer to an element 
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h 
 * returns a pointer to an element, or NULL if not found
 */ 
void* qsearch(queue_t *qp,
              bool (*searchfn)(void* elementp,const void* keyp),
              const void* skeyp){
	iqueue_t *iqp=(iqueue_t*)qp;
	element_t *ep;
	for(ep=iqp->front;ep!=NULL;ep=ep->next){
		if(searchfn(ep->data,skeyp)==true){
			return ep->data;
		}
	}
	return NULL;
}

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* qremove(queue_t *qp,
              bool (*searchfn)(void* elementp,const void* keyp),
              const void* skeyp){
	iqueue_t *iqp=(iqueue_t*)qp;
	element_t *ep;
	element_t *prev;
	void* sdata;
	
	for(ep=iqp->front;ep!=NULL;ep=ep->next){
		if(searchfn(ep->data,skeyp)==true){
			if(ep==iqp->front){
				iqp->front=iqp->front->next;
				ep->next=NULL;
				sdata=ep->data;
				free(ep);
				return sdata;
			}
			else if(ep==iqp->back){
				iqp->back=prev;
				prev->next=NULL;
				ep->next=NULL;
				sdata=ep->data;
				free(ep);
				return sdata;
			}
			else{
				prev->next=ep->next;
				ep->next=NULL;
				sdata=ep->data;
				free(ep);
				return sdata;
			}
		}
		prev=ep;
	}
	return NULL;
} 


/* concatenatenates elements of q2 into q1
 * q2 is dealocated, closed, and unusable upon completion
 */ 
void qconcat(queue_t *q1p, queue_t *q2p) {
	iqueue_t *iq1p=(iqueue_t*)q1p;
	iqueue_t *iq2p=(iqueue_t*)q2p;
	
	if(iq1p->front==NULL && iq2p->front==NULL){
		printf("Both lists are empty\n");
		qclose(iq2p);
	}
	else if(iq1p->front==NULL && iq2p->front!=NULL){
		iq1p->front=iq2p->front;
		iq1p->back=iq2p->back;
		iq2p->front=NULL;
		iq2p->back=NULL;
		qclose(iq2p);
	}
	else if(iq1p->front!=NULL && iq2p->front==NULL){
		printf("List in second argument is empty\n");
		qclose(iq2p);
	}
	else{
	iq1p->back->next=iq2p->front;
	iq2p->front=NULL;
	iq1p->back=iq2p->back;
	iq2p->back=NULL;
	qclose(iq2p);
	}
}
