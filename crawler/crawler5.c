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

  char dir_path[50];
  char *path= "../";
	
  sprintf(dir_path,"%s%s",path,dirname);  
  struct stat dirstat;
  
  if (stat(dir_path,&dirstat) == -1) {
    mkdir (dir_path, 0700);
  }

  //open the file

  char file_path[50];
  sprintf(file_path,"%s/%d",dir_path,id);
  FILE *my_file = fopen(file_path, "w");


  // write the four lines

  char *url=webpage_getURL(pagep);
	fprintf(my_file,"%s\n",url);
	//free(url);

  int depth=webpage_getDepth(pagep);
  fprintf(my_file,"%d\n",depth);

  int html_len=webpage_getHTMLlen(pagep);
	fprintf(my_file,"%d\n",html_len);

  char *html=webpage_getHTML(pagep);
  fprintf(my_file,"%s\n",html);
	//free(html);

  //close the file
  fclose(my_file);

  return 0;

}



int main(void){

	webpage_t *page=webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	
	if(webpage_fetch(page)){
	  printf("value returned from pagesave function: %d\n",pagesave(page,1,"pages"));
	}
	else{
		exit(EXIT_FAILURE);
	}
	
	webpage_delete(page);
	exit(EXIT_SUCCESS);
}
