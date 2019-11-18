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
#include <lqueue.h>
#include <lhash.h>
#include <webpage.h>
#include <sys/stat.h>
#include <pthread.h>

int number_of_done=0;

typedef struct arguments{
	lqueue_t *queue;
  lhashtable_t *hashtable;
	int maxdepth;
	int id;
	char *pagedir;
  bool (*searchfn)(void* elementp,const void* searchkeyp);
	pthread_mutex_t mutex;
	int threadcount;
}arguments_t;  

arguments_t* make_argument(lqueue_t *queue,lhashtable_t *hashtable,int maxdepth,int id,char *pagedir,bool (*searchfn)(void* elementp,const void*searchkeyp),int threadcount){
  arguments_t *arg;
  if(!(arg=(arguments_t*)malloc(sizeof(arguments_t)))){
    printf("[Error: malloc failed allocating argument_t]\n");
    return NULL;
  }
	arg->queue=queue;
  arg->hashtable=hashtable;
  arg->maxdepth=maxdepth;
  arg->id=id;
	arg->pagedir=pagedir;
  arg->searchfn=searchfn;
  pthread_mutex_init(&(arg->mutex),NULL);
	arg->threadcount=threadcount;
  return arg;
}                   

void print_anything(void *elementp){                                       
  webpage_t *wp=(webpage_t*)elementp;
  printf("URL: %s\n", webpage_getURL(wp));
}

void print_url(void *url){
	printf("url: %s\n", (char*)url);
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
	
  int depth=webpage_getDepth(pagep);
  fprintf(my_file,"%d\n",depth);

  int html_len=webpage_getHTMLlen(pagep);
	fprintf(my_file,"%d\n",html_len);

  char *html=webpage_getHTML(pagep);
  fprintf(my_file,"%s\n",html);
	
  //close the file
  fclose(my_file);
  return 0;
}

void* crawl_page(void* arg){
	bool was_empty=false;
	arguments_t *argt=(arguments_t*)arg;
	
	webpage_t *page;

	while(number_of_done<argt->threadcount){
		if((page=lqget(argt->queue))!=NULL){

			if(was_empty==false){
				;
			}
			else{
				pthread_mutex_lock(&(argt->mutex));
				number_of_done--;
				was_empty=false;
				pthread_mutex_unlock(&(argt->mutex));
			}
			
			if(webpage_fetch(page)){
				//check if depth exceeds the max depth if not carry on
				if(webpage_getDepth(page)<=(argt->maxdepth)){
					
					pthread_mutex_lock(&(argt->mutex));
					
					pagesave(page,argt->id,argt->pagedir);
					printf(".");
					fflush(stdout);
					
					argt->id=(argt->id)+1;
					pthread_mutex_unlock(&(argt->mutex));
					
					int pos = 0;
					char *result;
					webpage_t* inter_page=NULL;
					
					while((pos=webpage_getNextURL(page,pos,&result)) > 0){
						if(IsInternalURL(result)){
							inter_page=webpage_new(result,(webpage_getDepth(page))+1, NULL);
							//printf("Found internal url: %s\n",result);
							if(lhsnp(argt->hashtable,(void*)inter_page,argt->searchfn,result,strlen(result))==0){
								lqput(argt->queue,(void*)inter_page);
							}
							else{
								webpage_delete(inter_page);
								inter_page=NULL;
							}
						}                         
						else{                           
							//printf("Found external url: %s\n", result);
						}
						free(result);
						result=NULL;
					}
				}
			}
		}
		else{
			if(was_empty){
				;
			}
			else{
				pthread_mutex_lock(&(argt->mutex));
				number_of_done++;
				was_empty=true;
				pthread_mutex_unlock(&(argt->mutex));
			}
		}
	}
	return NULL;
}

int main(int argc,char *argv[]){
	
	if(argc==5){
		//get values from commandline
		char *url=argv[1];
		char *pagedir=argv[2];
		int maxdepth=atoi(argv[3]);
		
		int threadcount=atoi(argv[4]);
		
		//track ID and depth
		int id=1;
		
		//check if provided depth is greater than or equal to 1
		if(maxdepth>=0){
			
			lqueue_t *webq=lqopen();
			lhashtable_t *urlH=lhopen(20);
			
			// create webpage, put it in a queue and the URL in hash
			//then save it in a file
			webpage_t *page=webpage_new(url,0,NULL);
			lhput(urlH,(void*)page,url,strlen(url));
			lqput(webq,(void*)page);

			arguments_t *argument=make_argument(webq,urlH,maxdepth,id,pagedir,searchfn,threadcount);
			
			int i;
			pthread_t tid[threadcount];
			
			for(i=0;i<threadcount;i++){
				pthread_create(&tid[i],NULL,crawl_page,(void*)argument);
			}
			
			for(i=0;i<threadcount;i++){
				pthread_join(tid[i],NULL);
			}
					
			printf("\n");
			pthread_mutex_destroy(&(argument->mutex));
			free(argument);
			lqclose(webq);
			lhapply(urlH,webpage_delete);
			lhclose(urlH);
			exit(EXIT_SUCCESS);
		}
		else{
			printf("Maxdepth is negative, must be >=0\n");
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Usage:pcrawlerf <seedurl> <pagedir> <maxdepth> <numberofthreads>\n");
		exit(EXIT_FAILURE);
	}
}
