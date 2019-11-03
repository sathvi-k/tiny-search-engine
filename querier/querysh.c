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

int main(void){
  char input[50];
 
  printf("> ");
	
  int returned;
	FILE *indexfile=fopen("../indexer/index1","r");
	
	char word[20];
	char countersp[100];

	char printed[100];
	int min=2147483647;
	
  while((returned=scanf(" %[^\n]",input))==1){
		// split user's input by spaces and tabs
		
		char *token = strtok(input,"  \t");
	 
		while (token != NULL){
			
			if (strcmp(token,"and")==0){
				;
			}
			
			// if user enters ^D, break out of loop and stop scanning input
			if (returned==EOF){
				break;
			}
			
			// take word from string user enters and check that it only
			// has alphabetical characters, and convert to lowercase letters
			for (int i=0;i<strlen(token);i++){
				if (isalpha(token[i])!=0){
					token[i]=tolower(token[i]);
				}
				
				//otherwise, there's numbers and punctuations, so it's invalid
				else if (isalpha(token[i])==0){
					printf("[invalid query]");
					break;
				}					
			}
			
			while(fscanf(indexfile,"%s %[^\n]",word,countersp)!=EOF){
				
				if(strcmp(word,token)==0){
					strcpy(printed, word);
					
					char *idorcount=strtok(countersp," ");
					int count;
					
					while(idorcount!=NULL){
					 
						count=atoi(strtok(NULL," "));
						sprintf(printed, "%s:%d ", printed, count);
						
						if (count<min){
							min=count;
						}
						idorcount=strtok(NULL," ");
					}
				}
			}
			// after converting to lowercase, print word, then move pointer
			// to next word
			token = strtok(NULL,"  \t");
		}
		
		sprintf(printed, "%s -- %d", printed, min);
		printf("%s", printed);
		printf("\n> ");
		
	}
	return 0;
}
