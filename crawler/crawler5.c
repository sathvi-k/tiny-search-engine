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
#include <sys/stat.h>

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



int32_t pagesave(webpage_t *pagep, int id, char *dirname) {

  //make the dirname if it doesnt exist

  char dir_name[50];

  char *path_to_dir = "../";
  sprintf(dir_name, "%s%s", path_to_dir, dirname);  
  struct stat dir_stat = {0};
  
  if (stat(dir_name, &dir_stat)<0) {
    mkdir (dir_name, 0700);
  }

  //open the file

  char file_name[50];
  sprintf(file_name, "/%d", id);
  char *file_path = strcat(dirname, file_name);
  FILE *my_file = fopen(file_path, "w");


  // write the four lines

  char *url = webpage_getURL(pagep);

  fprintf(my_file, "%s\n", url);

  int depth =  webpage_getDepth(pagep);
  
  fprintf(my_file, "%d\n", depth);

  int html_len = webpage_getHTMLlen(pagep);

  fprintf(my_file, "%d\n", html_len);

  char *html = webpage_getHTML(pagep);

  fprintf(my_file, "%s\n", html);

  //close the file
  fclose(my_file);

  return 0;


}



int main(void){

	webpage_t *page=webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	if(webpage_fetch(page)){



	  /*

	    int pos = 0;
		//		const char *wa;
		char *result=NULL;
		queue_t *webq=qopen();
		hashtable_t *urlH=hopen(7);
		webpage_t* inter_page=NULL;
		while((pos=webpage_getNextURL(page,pos,&result)) > 0){
			if (IsInternalURL(result)){
				printf("Found internal url: %s\n", result);
				inter_page=webpage_new(result, 1, NULL);
				if(hsearch(urlH,searchfn,result,sizeof(*result))==NULL){
					const char *wa=webpage_getURL(inter_page);
					hput(urlH,(void*)inter_page, wa, sizeof(*wa));
					qput(webq,(void*)inter_page);
				}
				//				else{
				//	free(inter_page);
				//}
			}
			else{
				printf("Found external url: %s\n", result);
			}
			free(result);
		}
		qapply(webq,print_anything);
		qapply(webq,webpage_delete);
		qclose(webq);
		hclose(urlH);
       		webpage_delete(inter_page);
		//		inter_page=NULL;         */


	  printf("return of pagedsave: %d\n",  pagesave(page, 1, "pages"));

	  
	}
	else{
		exit(EXIT_FAILURE);
	}
       	webpage_delete(page);
	exit(EXIT_SUCCESS);
}
