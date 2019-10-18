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
		char *result;
		queue_t *webq=qopen();
		hashtable_t *urlH=hopen(7);
		webpage_t* inter_page;
		while((pos=webpage_getNextURL(page,pos,&result)) > 0){
			if (IsInternalURL(result)){
				printf("Found internal url: %s\n", result);
				inter_page=webpage_new(result, 1, NULL);
				if(hsearch(urlH,searchfn,result,sizeof(*result)){
					qput(webq, (void*)inter_page);
					//TODO: change code for hash.c to just hold
					//string values instead of having queues in each
					//box of the hashtable
					hput(urlH,(void*)?);
				}
				
			}
			else{
				printf("Found external url: %s\n", result);
			}
			free(result);
		}
		qapply(webq,print_anything);
		qapply(webq,webpage_delete);
		qclose(webq);
		//		inter_page=NULL;
	}
	else{
		exit(EXIT_FAILURE);
	}
	webpage_delete(page);
	exit(EXIT_SUCCESS);
}
