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

void print_url(void *url){
	printf("url: %s\n", (char*)url);
}

bool searchfn(void* elementp,const void* searchkeyp){                     
  char *wp2=(char*)elementp;                                     
  char *urlP = (char*)searchkeyp;                                          
                                          
  if(strcmp(urlP,wp2)==0){                                              
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



int main(int argc,char *argv[]){
	
	if(argc==4){
		//gotten from arguments
		char *url=argv[1];
		char *pagedir=argv[2];
		int maxdepth=atoi(argv[3]); 
		
		//track ID and depth
		int id=1;
		
		//check if provided depth is greater than or equal to 1
		if(maxdepth>=0){
			
			queue_t *webq=qopen();                                                                                                            
			hashtable_t *urlH=hopen(20);
			
			// create webpage, put it in a queue and the URL in hash
			//then save it in a file
			webpage_t *page=webpage_new(url,0,NULL);
			
			char seedurl[100];
			sprintf(seedurl,"%s",url);
			
			hput(urlH,(void*)seedurl,seedurl,strlen(seedurl));
			qput(webq,(void*)page);
					
			//check if queue if empty if not carry on
			while((page=qget(webq))!=NULL){
				
				if(webpage_fetch(page)){
					
					//check if depth exceeds the max depth if not carry on
					if(webpage_getDepth(page)<=maxdepth){
						
						pagesave(page,id,pagedir);
						id=id+1;
						
						int pos = 0;
						char *result;
						webpage_t* inter_page=NULL;
						
						while((pos=webpage_getNextURL(page,pos,&result)) > 0){
							
							char hurl[100];
							sprintf(hurl,"%s",result);
							if(IsInternalURL(result)){
								
								
								//printf("Found internal url: %s\n",hurl);

								//happly(urlH, print_url);
								//printf("hsreturn: %s\n", (char*)hsearch(urlH,searchfn,hurl,strlen(hurl)));
					 
								if(hsearch(urlH,searchfn,hurl,strlen(hurl))==NULL){
									printf("queued:%s\n",result);
									inter_page=webpage_new(hurl,(webpage_getDepth(page))+1, NULL);
									
									hput(urlH,(void*)hurl,hurl,strlen(hurl));
									qput(webq,(void*)inter_page);

									free(result);
							    result=NULL;
								}
							}                                                                                                                   
							else{                                                                                                            
								//	printf("Found external url: %s\n", result);                                                                      
							}
							
						}
					}
				}
				webpage_delete(page);
				page=NULL;
			}
			qclose(webq);
			hclose(urlH);
			exit(EXIT_SUCCESS);
		}
		else{
			printf("Maxdepth is negative, must be >=0\n");
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Usage:crawlerf <seedurl> <pagedir> <maxdepth>\n");
		exit(EXIT_FAILURE);
	}
}


































		
	
	                                                                                                                            
                                                                                                                                      
