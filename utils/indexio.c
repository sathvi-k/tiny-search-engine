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


FILE *my_file = NULL;
/*                                                                             
 * indexsave -- save the index to a file named indexnm                         
 * returns: 0 for success; nonzero otherwise                 
 */


void printqueue(void *elementp){                                                
  counter_t *doc=(counter_t*)elementp;                                          
  fprintf(my_file,"%d %d ",doc->id,doc->count);                                
}                                                                               
                                                                                
void save_index(void *elementp){                                               
  word_t *wordt=(word_t*)elementp;
	if (my_file!=NULL) {
		fprintf(my_file, "\n%s ",wordt->word);                                              
		queue_t *qp=wordt->queue;                                                     
		qapply(qp,printqueue);
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
	hclose(hash);
	return 0;
	
}
                                                                               
/*                                                                             
 * indexload -- loads the index in <indexnm> into a new index                  
 * returns: non-NULL for success; NULL otherwise                               
 */                                                                            
index_t *indexload(char *indexnm){
	char file_path[50];                                                                                                            
  char *path= "../indexer";                                                                                                      
  sprintf(file_path,"%s/%s",path,indexnm);  

	hashtable_t *wordH=hopen(150);    
	
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	FILE *indexname = fopen (file_path, "r");
	
	while ((read = getline(&line, &len, indexname)) != -1) {

		char allinfo[500];
		sprintf(allinfo,"%s",line);
		
		char *token = strtok(allinfo, " ");
		strcpy(word,token);
		word_t *wordt=(word_t*)malloc(sizeof(word_t));
		strcpy(wordt->word,word);   

		queue_t *qp=qopen();                                                                       
		wordt->queue=qp;

		int i=0;

		counter_t *doc;
		
		while (token != NULL) {
				
				token = strtok(NULL, " ");
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
		}
		hput(wordH,(void*)wordt,word,strlen(word));   
	}
	//WE SHOULD CONVERT wordH TO INDEX DATA STRUCTURE
	return wordH;	
}

