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



int main(void){
	webpage_t *page=webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	if(webpage_fetch(page)){
		//char *html=webpage_getHTML(page);
		int pos = 0;
		char *result;
		while((pos=webpage_getNextURL(page,pos,&result)) > 0){
			if (IsInternalURL(result)){
				printf("Found internal url: %s\n", result);
			}
			else{
				printf("Found external url: %s\n", result);
			}
			free(result);
			result=NULL;
		}
	}
	else{
		exit(EXIT_FAILURE);
	}
	webpage_delete(page);
	page=NULL;
	
	exit(EXIT_SUCCESS);
}
