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


typedef struct index_t{
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

hashtable_t* make_index(int id){
	
}
