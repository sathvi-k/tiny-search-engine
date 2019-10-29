#pragma once
/* indexio.h --- 
 * 
 * 
 * Author: Sathvika R. Korandla
 * Created: Mon Oct 28 17:57:26 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>                                                             
#include <stdlib.h>                                                            
#include <string.h>                                                            
#include <inttypes.h>                                                          
#include <unistd.h>                                                            
#include <webpage.h>
#include <pageio.h>

/*create a page index_t struct to hold the hashtable of words*/
//typedef struct index index_t;
typedef void index_t;

/* indexsave -- save the index to a file named indexnm                         
 * returns: 0 for success; nonzero otherwise                 
 */                                                                            
int32_t indexsave(index_t *indexp, char *indexnm);                     
                                                                               
/* indexload -- loads the index in <indexnm> into a new index                  
 * returns: non-NULL for success; NULL otherwise                               
 */                                                                            
index_t *indexload(char *indexnm);
