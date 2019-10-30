/* indexer7.c --- 
 * 
 * 
 * Author: Sathvika R. Korandla
 * Created: Wed Oct 23 01:06:54 2019 (-0400)
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
#include <sys/stat.h>
#include <webpage.h>
#include <pageio.h>
#include <ctype.h>
#include <queue.h>
#include <hash.h>
#include <dirent.h>
#include <indexio.h>

//global variable
//need it to calculate a sum of all counts of words in a hashtable 
int sum=0;

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



void qcount(void *elementp){
	counter_t* doc=(counter_t*)elementp;
	sum+=doc->count;
}

void sumwords(void *elementp){
  word_t *wrdp=(word_t*)elementp;
	queue_t *qtp=wrdp->queue;
	qapply(qtp,qcount);
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

/* NormalizeWord()
 *
 * converts a word to lowercase
 * returns *char
 */

char* NormalizeWord(char *word){
	char *normalized=NULL;
	int p=strlen(word);
	if (p>=3){
		for (int i=0; i<p; i++){
			if (isalpha(word[i])!=0){
				word[i]=tolower(word[i]);
      }
      else{
				return normalized;
      }
    }
    normalized=word;
  }
  return normalized;
}


int main(int argc,char *argv[]){
	
	if(argc==3){

		char *pagedir=argv[1];
		char *indexnm=argv[2];

		int idmax=0;
		int idcount=1;
		
		char dir_path[50];                                                                                             
		char *path= "../";                                                                                             
		sprintf(dir_path,"%s%s",path,pagedir);                                                                         
		struct stat dirstat; 


		if (stat(dir_path,&dirstat) == -1) {                                                                           
			printf("Directory does not exist");
			exit(EXIT_FAILURE);
		}

		DIR *dir;
		struct dirent *entry;
		dir=opendir(dir_path);
		
		while((entry=readdir(dir)) != NULL){
			idmax+=1;
		}
		closedir(dir);
		
		hashtable_t *wordH=hopen(150);
		while(idcount<=idmax){
		
			webpage_t *loaded;
			loaded=pageload(idcount,"pages");
			
			int pos=0;
			char *wordp;
			
			while((pos=webpage_getNextWord(loaded,pos,&wordp))>0){
				
				if (NormalizeWord(wordp)!=NULL){
					
					//if there's NO word object in the hashtable with the norm_word=word, put this object into
					//the hashtable
					if (hsearch(wordH,hsearchfn,wordp,strlen(wordp))==NULL){
						word_t *wordt=(word_t*)malloc(sizeof(word_t));
						strcpy(wordt->word,wordp);
						queue_t *qp=qopen();
						wordt->queue=qp;
						
						counter_t *doc=(counter_t*)malloc(sizeof(counter_t));
						doc->id=idcount;
						doc->count=1;
						
						qput(qp,doc);
						hput(wordH,(void*)wordt,wordp,strlen(wordp));
					}
					
					//if there IS word object in the hashtable with
					//its norm_word attribute equal to the value of "word" variable
					else{
						word_t *wtp=(word_t*)hsearch(wordH,hsearchfn,wordp,strlen(wordp));
						queue_t *qp=wtp->queue;
						
						if(qsearch(qp,qsearchfn,(const void*)&idcount)==NULL){
							counter_t *doc1=(counter_t*)malloc(sizeof(counter_t));
							doc1->id=idcount;
							doc1->count=1;
							
							qput(qp,doc1);
						}
						
						else{
							counter_t *ctp=(counter_t*)qsearch(qp,qsearchfn,(const void*)&idcount);
							ctp->count+=1;
						}
						
					}
					
				}
				free(wordp);
				wordp=NULL;
			}
			idcount+=1;
			webpage_delete(loaded);
			loaded=NULL;
		}
		
		index_t *indext=(index_t*)malloc(sizeof(index_t));                                                             
		indext->hashtable=wordH;                                                                                       
		
		indexsave(indext,indexnm);                                                                                 
		happly(wordH,word_delete);                                                                                     
		hclose(wordH);                                                                                                 
		free(indext);   	
			
		exit(EXIT_SUCCESS);
	}
	
	else{
		printf("usage:indexer <pagedir> <indexnm>");
		exit(EXIT_FAILURE);
	}
}
