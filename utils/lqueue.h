#pragma once
/* 
 * lqueue.h -- public interface to the locked-queue module
 */
#include <stdint.h>
#include <stdbool.h>

/* the locked-queue representation is hidden from users of the module */
typedef void lqueue_t;		

/* create an empty queue */
lqueue_t* lqopen(void);        

/* deallocate a locked-queue, frees everything in it */
void* lqclose(void *arg);   

/* put element at the end of the locked-queue
 * returns 0 if successful; nonzero otherwise 
 */
void* lqput(void *arg); 

/* get the first element from locked-queue, removing it from the locked-queue */
void* lqget(void *arg);

/* apply a function to every element of the locked-queue */
void* lqapply(void *arg);

/* search a locked-queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the locked-queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* lqsearch(void* arg);

