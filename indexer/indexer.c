/* indexer.c --- 
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

/* NormalizeWord()
 *
 * converts a word to lowercase
 * returns *char
 */

char* NormalizeWord(char *word){
  char word_a[30];
  strcpy(word_a,word);
  char *normalized=NULL;  
  int p=strlen(word_a);
  if (p>=3){
    for (int i=0; i<p; i++){
      if (isalpha(word_a[i])!=0){
	word_a[i]=tolower(word_a[i]);
      }
      else{
	return normalized;
      }
    }
    strcpy(normalized,word_a);
  }
  return normalized;
}


int main(void){
	webpage_t *loaded;
	loaded=pageload(1,"pages");

	int pos=0;
	char *word;
	while((pos=webpage_getNextWord(loaded,pos,&word))>0){
	  if (NormalizeWord(word)!=NULL){
	    printf("%s\n",word);
	  }
	  free(word);
	}
	return 0;
}
