/* query.c --- 
 * 
 * Author: Sathvika R. Korandla
 * Created: Thu Oct 31 22:29:05 2019 (-0400)
 * Version: 
 * 
 * Description: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <indexio.h>
#include <webpage.h>
#include <queue.h>
#include <hash.h>
#include <pageio.h>
#include <dirent.h>

typedef struct index{
	hashtable_t *hashtable;
}index_t;

typedef struct word{
	char word[100];
	queue_t *queue;
}word_t;

typedef struct counter{
	int id;
	int count;
}counter_t;

typedef struct docrank{
	int id;
	int rank;
	char url[500];
}docrank_t;


void docrankclose(void *data){
	docrank_t *doc=(docrank_t*)data;
	free(doc);
}

void qdataclose(void *data){
	counter_t *doc=(counter_t*)data;
	free(doc);                         
}

void word_delete(void *data){
	word_t *wtp=(word_t*)data;
	queue_t *qp=wtp->queue;
	qapply(qp,qdataclose);
	qclose(qp);
	free(wtp);
}    

bool hsearchfn(void* elementp,const void* searchkeyp){
	word_t *wrdp2=(word_t*)elementp;
	char *wordP = (char*)searchkeyp;
	char *wrdp2_word=wrdp2->word;
	if(strcmp(wordP,wrdp2_word)==0){
		return true;
	}
	else{
		return false;
	}
}

bool qsearchfn(void* elementp,const void* searchkeyp){
	counter_t *cp=(counter_t*)elementp;
	int *id=(int*)searchkeyp;
	int cpid=cp->id;

	if(cpid==*id){
		return true;
	}

	else{
		return false;
	}
}

bool docqsearchfn(void* elementp,const void* searchkeyp){
	docrank_t *docp=(docrank_t*)elementp;
	int *id=(int*)searchkeyp;
	int docid=docp->id;

	if(docid==*id){
		return true;
	}

	else{
		return false;
	}
}

void print_rank_queue(void *data){
	docrank_t *doc=(docrank_t*)data;
	int rank=doc->rank;
	int id=doc->id;
	char *url=doc->url;
	printf("rank:%d : doc:%d : %s\n",rank,id,url);
}


queue_t *splitted_by_or(char* input){
	// split user's input by spaces and tabs
	char *token = strtok(input," \t");
	char output[1000]="";
	queue_t *docqueue=qopen();
	
	while (token != NULL){

		int id=1;
		
		// take word from string user enters and check that it only
			// has alphabetical characters, and convert to lowercase letters
		for (int i=0;i<strlen(token);i++){
			if (isalpha(token[i])!=0){
				token[i]=tolower(token[i]);
			}
			
			//otherwise, there's numbers and punctuations, so it's invalid
			else if (isalpha(token[i])==0){
				printit=false;
			}					
		}
		
		sprintf(output,"%s%s ",output,token);
		
		if(strcmp(token,"and")==0 || strlen(token)<3){
			;
		}
		
		else{   
			word_t *foundword;
			//search for token in hashtable using hsearch
			foundword=hsearch(hashtable,hsearchfn,token,strlen(token));
			
			if(foundword!=NULL){ // token is in index
				while(id<=idmax){
					queue_t *queue=foundword->queue;
					counter_t *doc=qsearch(queue,qsearchfn,(const void*)&id);
					
					if(doc!=NULL){ // token is in document
						int count=doc->count;
						docrank_t *sdoc=qsearch(docqueue,docqsearchfn,(const void*)&id);
						
						if(sdoc!=NULL){ // document is in queue of docs containing token
							if(count<(sdoc->rank)){ // replace with lower rank
								sdoc->rank=count;
							}
						}				
					}
					
					else{ // doc is NULL
						docrank_t *rdoc=qremove(docqueue,docqsearchfn,(const void*)&id);
						
						if(rdoc!=NULL){
							free(rdoc);
						}
					}
					id++;	
				}
			}
			else{ // foundword is NULL, therefore not in the index
				printit=false;
			}
		}
		
		// after converting to lowercase, print word, then move pointer
		// to next word
		token = strtok(NULL," \t");
		qclose(docqueue);
		return docqueue;
	}
}


int main(void){	
  char dir_path[50];
	char *path= "../";
	char *pagedir="pages";
	sprintf(dir_path,"%s%s",path,pagedir);
	int idmax=0;
	
	DIR *dir;
	struct dirent *entry;

	dir=opendir(dir_path);  
	
	while((entry=readdir(dir)) != NULL){
		
		if((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0)){
			;                             
		}

		else{
			idmax=idmax+1;
		}
	}          
	closedir(dir);

	
	
	index_t *index = indexload("index2");
	hashtable_t *hashtable = index->hashtable;
	

	int idcount=1;
	
	while(idcount<=idmax){
		docrank_t *docrank = (docrank_t*)malloc(sizeof(docrank_t));
		docrank->id=idcount;
		docrank->rank=2147483647;

		char webpage_path[300];
		sprintf(webpage_path,"%s/%d",dir_path,idcount);
		
		FILE *webpage=fopen(webpage_path,"r");
		char url[500];
		fscanf(webpage,"%s",url);
		strcpy(docrank->url,url);
		fclose(webpage);
		
		qput(docqueue,docrank);
		idcount++;
	}
		
	bool printit=true;
  char input[1000];
	

	
  printf("> ");
  
  while(fgets(input,1000,stdin)!=NULL){
		
		if(input[0] == '\n'){
			printf("> ");
		}
		
		input[strlen(input)-1]='\0';

		queue_t word_queues[20];
    char* token = strtok(input, " /t");
		int wq_i=0;
		char string[10];
		while (token!=NULL){
			if (strcmp(token,"or")==0){
				queue_t *q=splitted_by_or(string);
				word_queues[wq_i]=q;
				wq_i++;
				memset(string, 0, sizeof(string));
			}
			else{
				sprintf(string, "%s %s", string, token);
			}
		}
		
		// print newline and > to prompt user for input
		if(printit){
			printf("%s\n",output);
			qapply(docqueue,print_rank_queue);
			printf("> ");
		}
		
		else{
			printf("[invalid query]\n");
			printf("> ");
			printit=true;
		}		
	}

	for (int i=0; i<sizeof(word_queues); i++){
		if (word_queues[i]!=NULL|| word_queues[i]!=0){
			
		}
		
	}
	
	qapply(docqueue,docrankclose);
	qclose(docqueue);
	happly(hashtable,word_delete);
	hclose(hashtable);
	free(index);
	printf("^D\n");
  return 0;
}
