/* lhash.c --- 
 * 
 * 
 * Author: Sathvika R. Korandla
 * Created: Wed Nov 13 18:15:00 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <hash.h>
#include <queue.h>
#include <lhash.h>
#include <pthread.h>

typedef struct ilhashtable_t{
	hashtable_t *hash;
	pthread_mutex_t mutex;
}ilhashtable_t;  

/* lhopen -- opens a locked hash table with initial size hsize */
lhashtable_t *lhopen(uint32_t hsize){
	ilhashtable_t *lhp;
 
	if(!(lhp=(ilhashtable_t *)malloc(sizeof(ilhashtable_t)))){
    printf("[Error: malloc failed allocating locked-hashtable]\n");
    return NULL;
  }
	
	lhp->hash=hopen(hsize);
	pthread_mutex_init(&(lhp->mutex),NULL);
	lhashtable_t *lht=(lhashtable_t*)lhp;
	return lht;
}

/* lhclose -- closes a locked hash table */
void lhclose(lhashtable_t *lhp){
	ilhashtable_t *ilhp=(ilhashtable_t*)lhp;
	pthread_mutex_lock(&(ilhp->mutex));
	hclose(ilhp->hash);
	pthread_mutex_unlock(&(ilhp->mutex));
	pthread_mutex_destroy(&(ilhp->mutex));
	free(ilhp);
}

/* lhput -- puts an entry into a locked hash table under designated key
 * returns 0 for success; non-zero otherwise
 */
int32_t lhput(lhashtable_t *lhp, void *ep, const char *key, int keylen){
	int32_t return_value;
	ilhashtable_t *ilhp=(ilhashtable_t*)lhp;
	pthread_mutex_lock(&(ilhp->mutex));
	return_value=hput(ilhp->hash,ep,key,keylen);
	pthread_mutex_unlock(&(ilhp->mutex));
	return return_value;
}

/* lhapply -- applies a function to every entry in locked hash table */
void lhapply(lhashtable_t *lhp, void (*fn)(void* ep)){
	ilhashtable_t *ilhp=(ilhashtable_t*)lhp;
	pthread_mutex_lock(&(ilhp->mutex));
	happly(ilhp->hash,fn);
	pthread_mutex_unlock(&(ilhp->mutex));
}

/* lhsearch - searches for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if 
 * not found
 */
void *lhsearch(lhashtable_t *lhp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
	ilhashtable_t *ilhp=(ilhashtable_t*)lhp;
	pthread_mutex_lock(&(ilhp->mutex));
	void* lsearch=hsearch(ilhp->hash,searchfn,key,keylen);
	pthread_mutex_unlock(&(ilhp->mutex));
	return lsearch;
}

/* lhqget -- looks for a specific queue in the locked hashtable using a key.
 * If the key does not exist, returns NULL */
void *lhget(lhashtable_t *lhp, const char *key, int32_t keylen){
	ilhashtable_t *ilhp=(ilhashtable_t*)lhp;
	pthread_mutex_lock(&(ilhp->mutex));
	void* lget=hqget(ilhp->hash,key,keylen);
	pthread_mutex_unlock(&(ilhp->mutex));
	return lget;
}

void lhsnp(lhashtable_t *lhp,bool (*searchfn)(void* elementp, const void* searchkeyp),const char *key,int32_t keylen){
  ilhashtable_t *ilhp=(ilhashtable_t*)lhp;
  pthread_mutex_lock(&(ilhp->mutex));  
  void* ep=hsearch(ilhp->hash,searchfn,key,keylen);
	if(ep==NULL){
		hput(ilhp->hash,ep,key,keylen);
	}
	pthread_mutex_unlock(&(ilhp->mutex));
}  


