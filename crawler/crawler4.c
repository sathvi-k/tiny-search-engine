/* crawler.c --- 
 * 
 * 
 * Author: Sathvika R. Korandla
 * Created: Thu Oct 17 16:44:08 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <queue.h>
#include <hash.h>
#include <webpage.h>

void print_anything(void *elementp){                                       
  webpage_t *wp=(webpage_t*)elementp;
  printf("URL: %s\n", webpage_getURL(wp));
}

bool searchfn(void* elementp,const void* searchkeyp){                     
  webpage_t *wp2=(webpage_t*)elementp;                                     
  char *urlP = (char*)searchkeyp;                                          
  char *wp2url=webpage_getURL(wp2);                                        
  
  if(strcmp(urlP,wp2url)==0){                                              
    return true;                                                           
  }
	
  else{                                                                    
    return false;                                                          
  }                                                                        
}     

int main(void){
	webpage_t *page=webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	if(webpage_fetch(page)){
		int pos = 0;
		
		//		const char *wa;
		char *result=NULL;
		
		queue_t *webq=qopen();
		hashtable_t *urlH=hopen(10);
		
		webpage_t* inter_page=NULL;
		
		while((pos=webpage_getNextURL(page,pos,&result)) > 0){
			
			if (IsInternalURL(result)){
				
				printf("Found internal url: %s\n", result);
				inter_page=webpage_new(result, pos, NULL);
				
				if(hsearch(urlH,searchfn,result,sizeof(*result))==NULL){
					const char *wa=webpage_getURL(inter_page);
					hput(urlH,(void*)inter_page, wa, sizeof(*wa));
					qput(webq,(void*)inter_page);
				}
				
				//delete(free) created webpage_t if it is a duplicate
				else{
					webpage_delete(inter_page);
					inter_page=NULL;
				}
				
			}
			else{
				printf("Found external url: %s\n", result);
			}
			free(result);
			result=NULL;
		}
		printf("Printing from queue:\n");
		qapply(webq,print_anything);
		qapply(webq,webpage_delete);
		hclose(urlH);
		qclose(webq);
		
		//webpage_delete(inter_page); This was because the webpage_t
		//struct that had the duplicate URL was not being deleted in the
		//qapply method. It is no longer needed now that we have an else statement to do the deleting.
		
	}
	
	else{
		exit(EXIT_FAILURE);
	}
	webpage_delete(page);
	page=NULL;
	
	exit(EXIT_SUCCESS);
}
