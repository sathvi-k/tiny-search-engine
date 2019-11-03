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
  int j; // keep track of if there are numbers/punctuation (0 if no, 1 if yes)

  printf("> ");
  int returned;
	FILE *index = fopen("../indexer/index1", "r");
	//int id = 1;
	char word[20];
	char countersp[100];
	
  while((returned=scanf(" %[^\n]", input))==1){
		// split user's input by spaces and tabs
		
		char *token = strtok(input, " \t");
		//char output[50]="";

		char printed[100];
		int min=2147483647;
    
		while (token != NULL){
			if (strcmp(token,"and")!=0){
				// if user enters ^D, break out of loop and stop scanning input
				if (returned==EOF){
					break;
				}
				
				// take word from string user enters and check that it only
				// has alphabetical characters, and convert to lowercase letters
				for (int i=0;i<strlen(token);i++){
					if (isalpha(token[i])!=0){
						token[i]=tolower(token[i]);
						j=0;
					}
					//otherwise, there's numbers and punctuations, so it's invalid
					else if (isalpha(token[i])==0){
						j=1;
					}					
				}

				while(fscanf(index, "%s %[^\n]", word, countersp)!=EOF){
					if(strcmp(word,token)==0){
						strcpy(printed, word);
						//int i=0; //check if id or count
						char *idorcount=strtok(countersp, " ");
						int count;
						while(idorcount!=NULL){
							//if (i==0){
							count=atoi(strtok(NULL, " "));
							sprintf(printed, "%s:%d ", printed, count);
							if (count<min){
								min=count;
							}
							//i=1;
									//	}
							/*							else{							
								i=0;
								}*/
							idorcount=strtok(NULL, " ");
							
						}
					}
				}
				
				/*if (j==0){
					sprintf(output,"%s%s ",output, token);
					}*/
			}
			// after converting to lowercase, print word, then move pointer
			// to next word
			token = strtok(NULL, " \t");
			
		}
		sprintf(printed, "%s -- %d", printed, min);
		
		//if (j==0){	
		//printf("%s",output);
		//}
		if (j==1){
			printf("[invalid query]");
		}
		printf("%s", printed);
		printf("\n> ");
  }
  return 0;
}
