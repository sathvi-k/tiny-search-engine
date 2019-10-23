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

/* NormalizeWord()
 *
 * converts a word to lowercase
 * returns *char
 */

char* NormalizeWord(char *word){
	char *word=word;
	char *normalized;

	if (isalpha(word)){
		if numeric value, hens
	}
	
	return normalized;
}


int main(void){
	webpage_t *loaded;
	loaded=pageload(1,"pages");

	int pos=0;
	char *word;
	while((pos=webpage_getNextWord(loaded,pos,&word))>0){
		printf("%s\n",word);
		free(word);
	}
	
	return 0;
}
