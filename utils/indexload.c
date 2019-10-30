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

	hashtable_t *wordH=hopen(1000);
	queue_t *qp=NULL;
	counter_t *doc=NULL;
	word_t *wordt=NULL;
	
	FILE *indexname=fopen(file_path,"r");

	char cp[50];
	char countsp[500];

	while (!feof(indexname)) {
		if (fscanf(indexname, "%s %s", cp, countsp)>0){
			wordt=(word_t*)malloc(sizeof(word_t));
			strcpy(wordt->word,cp);
			qp=qopen();
			wordt->queue=qp;

			char idncount[50];
			strcpy(idncount, countsp);

			char *token = strtok(idncount, " ");

			int i = 0;

			while (token != NULL) {
				if (i==0) {
					doc=(counter_t*)malloc(sizeof(counter_t));
					doc->id=token;
					i=1;
				}
				else if (i==1) {
					doc->count=token;
					i=0;
					qput(qp,doc);
				}
				token = strtok(NULL, " ");
			}
			hput(wordH,(void*)wordt,wordt->word,strlen(wordt->word));
		}
	}
}
		
