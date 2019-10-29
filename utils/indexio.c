/* indexio.c --- 
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
#include <indexio.h>
#include <queue.h>
#include <hash.h>
#include <ctype.h>
#include <stdbool.h>
FILE *my_file = NULL;
int isID=0;
bool reset=true;

typedef struct word{                                                  
  char word[100];                              
  queue_t *queue;                                 
}word_t;

typedef struct counter{                                                  
  int id;                           
  int count;                                         
}counter_t;

typedef struct index{            
  hashtable_t *hashtable;     
}index_t;   


void printqueue(void *elementp){                                                
  counter_t *doc=(counter_t*)elementp;                                          
  fprintf(my_file,"%d %d ",doc->id,doc->count);                                
}                                                                               

                      
void save_index(void *elementp){                                               
  word_t *wordt=(word_t*)elementp;
	if (my_file!=NULL) {
		fprintf(my_file, "%s ",wordt->word);                                              
		queue_t *qp=wordt->queue;                                                     
		qapply(qp,printqueue);
		fprintf(my_file,"\n");
	}
}


int32_t indexsave(index_t *indexp, char *indexnm){

	hashtable_t *hash=indexp->hashtable;
	
  //open the file                                                               
  char file_path[50];
	char *path= "../indexer";
  sprintf(file_path,"%s/%s",path,indexnm);                                       

	my_file = fopen(file_path, "w");                                        

	happly(hash,save_index);
	                                                                              
  //close the file                                                              
  fclose(my_file);                                                              
	return 0;
}

index_t *indexload(char *indexnm){
	char file_path[50];                                                                                                            
  char *path= "../indexer";                                                                                                      
  sprintf(file_path,"%s/%s",path,indexnm);  

	hashtable_t *wordH=hopen(150);
	queue_t *qp=NULL;
	counter_t *doc=NULL;
	word_t *wordt=NULL;
	
	FILE *indexname=fopen(file_path,"r");

	int letter=fgetc(indexname);

	char prevc;
	char cp[200];
	
	while (letter!=EOF) {
		
		char c= (char) letter;
		
		if(reset){
			sprintf(cp,"");
		}
		
		if(isalpha(c)!=0){
			sprintf(cp,"%s%c",cp,c);
		}
		
		else if((isdigit(c)!=0) && isID==0){
			sprintf(cp,"%s%c",cp,c);
		}	
			
		else if((isdigit(c)!=0) && isID==1){
			sprintf(cp,"%s%c",cp,c);
		}	
		
		else if(c==' '){
			
			if(isalpha(prevc)!=0){
				wordt=(word_t*)malloc(sizeof(word_t));
				strcpy(wordt->word,cp);
				qp=qopen();
				wordt->queue=qp;
				reset=true;
			}

			else if((isdigit(prevc)!=0) && isID==0){
				doc=(counter_t*)malloc(sizeof(counter_t));
				int id=atoi(cp);
				doc->id=id;
				isID=1;
				reset=true;
			}
			
			else if((isdigit(prevc)!=0) && isID==1){
				int count=atoi(cp);
				doc->count=count;
				qput(qp,doc);
				isID=0;
				reset=true;
			}
		}
		
		else if(c=='\n'){
			hput(wordH,(void*)wordt,wordt->word,strlen(wordt->word));
		}
		prevc=c;
		letter=fgetc(indexname);
	}
	
	//WE SHOULD CONVERT wordH TO INDEX DATA STRUCTURE
	index_t *loaded_index=(index_t*)malloc(sizeof(index_t));
	loaded_index->hashtable=wordH;
	return loaded_index;	
}

