/* indexer3.c --- 
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

//global variable
//need it to calculate a sum of all counts of words in a hashtable 
int sum=0;

typedef struct nwordc{
  char *norm_word;
  int count;
}nwordc_t;

void all_counts(void *elementp){
  nwordc_t *wrdp=(nwordc_t*)elementp;
  sum+=wrdp->count;
}

bool searchfn(void* elementp,const void* searchkeyp){
  nwordc_t *wrdp2=(nwordc_t*)elementp;
  char *wordP = (char*)searchkeyp;
  char *wrdp2_word=wrdp2->norm_word;

  if(strcmp(wordP,wrdp2_word)==0){
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
  /* char word_a[30];
  strcpy(word_a,word);
  char *normalized=(char *)calloc(1, strlen(word_a) + 1);
  */

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
    //strcpy(normalized,word_a);
    normalized=word;
  }
  return normalized;
}


int main(void){
  webpage_t *loaded;
  loaded=pageload(1,"pages");

  int pos=0;
  char *word;

  hashtable_t *wordH=hopen(150);
  while((pos=webpage_getNextWord(loaded,pos,&word))>0){
    if (NormalizeWord(word)!=NULL){
      //printf("%s\n",word);

      nwordc_t *w_obj = malloc(sizeof(*w_obj));
      w_obj->norm_word=word;
      w_obj->count=0;
  
      //if there's NO word object in the hashtable with the norm_word=word, put this object into
      //the hashtable
      if (hsearch(wordH,searchfn,word,strlen(word))==NULL){	
	w_obj->count=1;
	hput(wordH,(void*)w_obj,word,strlen(word));
      }
      //if there IS word object in the hashtable with
      //its norm_word attribute equal to the value of "word" variable
      else{
	nwordc_t *hword=(nwordc_t*) hsearch(wordH,searchfn,word,strlen(word));
	hword->count+=1;
	free(w_obj);
      }
    }
    free(word);
  }
  
  happly(wordH,all_counts);
  /*
  for (int i=0; i<150; i++){
    nwordc_t *word_obj=(nwordc_t*)qget(wordH->qtable[i]);
    sum+=word_obj->count;
    }*/
  printf("sum: %d\n",sum);
  hclose(wordH);
  webpage_delete(loaded);
  loaded=NULL;
  exit(EXIT_SUCCESS);
}

