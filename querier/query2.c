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
  while((returned=scanf(" %[^\n]", input))==1){
		// split user's input by spaces and tabs
		
		char *token = strtok(input, " \t");
		char output[50]="";
    
		while (token != NULL){
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
			
			if (j==0){
				sprintf(output,"%s%s ",output, token);
			}
			// after converting to lowercase, print word, then move pointer
			// to next word
			token = strtok(NULL, " \t");
		}
		if (j==0){	
			printf("%s",output);
		}
		else if (j==1){
			printf("[invalid query]");
		}
		printf("\n> ");
  }
  return 0;
}
