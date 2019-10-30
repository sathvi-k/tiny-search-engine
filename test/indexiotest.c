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
#include <hash.h>
#include <queue.h>
#include <ctype.h>

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
  
  int idmax=atoi(argv[1]);                                                      
  int idcount=1;                                                                
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

	indexsave(indext,"indexsave");
	happly(wordH,word_delete);     
	hclose(wordH);
	free(indext);
	
	index_t *loadedindex=indexload("indexsave");
	indexsave(loadedindex,"indexsave1");

	hashtable_t *loadedhash=loadedindex->hashtable;                              
  happly(loadedhash,word_delete);                                              
  hclose(loadedhash);                                                          
  free(loadedindex); 
	
	index_t *loadedindex1=indexload("indexsave1");
	indexsave(loadedindex1,"indexsave2");

	hashtable_t *loadedhash1=loadedindex1->hashtable;
	happly(loadedhash1,word_delete);
	hclose(loadedhash1);
	free(loadedindex1);
	
  exit(EXIT_SUCCESS);
}
