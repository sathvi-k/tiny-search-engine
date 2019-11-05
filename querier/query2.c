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

int main(void){
	bool printit=true;
  char input[1000];
	
  printf("> ");
	FILE *indexfile=fopen("../indexer/index1","r");
  while(fgets(input,1000,stdin)!=NULL){
		if(input[0] == '\n'){
			printf("> ");
		}
		input[strlen(input)-1]='\0';
		
    // split user's input by spaces and tabs
		char *token = strtok(input," \t");
		char output[500]="";
    int rank=2147483647;
		while (token != NULL){			
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
			char word[50];
			char idncount[20];
			if ((strlen(token)>=3) && (strcmp(token, "and")!=0) && printit){	
				while(fscanf(indexfile,"%s %[^\n]",word,idncount)!= EOF){
					if(strcmp(word,token)==0){
						char *token2 = strtok(idncount," ");
						int i = 0;
						char *array[2];
						while(token2!=NULL){
							array[i]=token2;
							i++;
							token2=strtok(NULL," ");
						}

						int id=atoi(array[0]);
						int count=atoi(array[1]);

						printf("word: %s count: %d\n",token, count);
						sprintf(output,"%s%s:%d ", output, token, count);
						if(count<rank){
							rank=count;
						}
						break;
					}
				}
			}
			// after converting to lowercase, print word, then move pointer
			// to next word
			token = strtok(NULL," \t");	
		}
		fclose(indexfile);
		// print newline and > to prompt user for input
		if(printit){
			printf("%s -- %d\n",output,rank);
			printf("> ");
		}
		else{
			printf("[invalid query]\n");
			printf("> ");
			printit=true;
		}		
	}

	printf("^D\n");
  return 0;
}
