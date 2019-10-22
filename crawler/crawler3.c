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

int main(void){
	webpage_t *page=webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	if(webpage_fetch(page)){
		//char *html=webpage_getHTML(page);
		int pos = 0;
		char *result=NULL;
		queue_t *webq=qopen();
		webpage_t* inter_page;
		while((pos=webpage_getNextURL(page,pos,&result)) > 0){
		  
			if (IsInternalURL(result)){
				printf("Found internal url: %s\n", result);
				inter_page=webpage_new(result, pos, NULL);
				qput(webq, (void*)inter_page);
			}
			else{
				printf("Found external url: %s\n", result);
			}
			free(result);
			result=NULL;
		}
		printf("Printing from Queue:\n");
		qapply(webq,print_anything);
		qapply(webq,webpage_delete);
		qclose(webq);
	}
	else{
		exit(EXIT_FAILURE);
	}
	webpage_delete(page);
	page=NULL;
	
	exit(EXIT_SUCCESS);
}
