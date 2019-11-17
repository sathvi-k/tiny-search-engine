#pragma once
/* lhash.h --- 
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

typedef void lhashtable_t;

/* lhopen -- opens a locked hash table with initial size hsize */
lhashtable_t *lhopen(uint32_t hsize);

/* lhclose -- closes a locked hash table */
void lhclose(lhashtable_t *lhp);

/* lhput -- puts an entry into a locked hash table under designated key
 * returns 0 for success; non-zero otherwise
 */
int32_t lhput(lhashtable_t *lhp, void *ep, const char *key, int keylen);

/* lhapply -- applies a function to every entry in locked hash table */
void lhapply(lhashtable_t *lhp, void (*fn)(void* ep));

/* lhsearch - searches for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if 
 * not found
 */
void *lhsearch(lhashtable_t *lhp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen);

/* lhqget -- looks for a specific queue in the locked hashtable using a key.
 * If the key does not exist, returns NULL */
void *lhget(lhashtable_t *lhp, const char *key, int32_t keylen);

int32_t lhsnp(lhashtable_t *lhp,void* element,bool (*searchfn)(void* elementp, const void* searchkeyp),const char *key,int32_t keylen);
