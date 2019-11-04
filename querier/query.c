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
  
  while(fgets(input,1000,stdin)!=NULL){
		
		if(input[0] == '\n'){
			printf("> ");
		}
		
		input[strlen(input)-1]='\0';
		
    // split user's input by spaces and tabs
		char *token = strtok(input," \t");
		char output[500]="";
    
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
			
			if(printit){
				sprintf(output,"%s%s ",output, token);
			}
			
			// after converting to lowercase, print word, then move pointer
			// to next word
			token = strtok(NULL," \t");	
		}
		
		// print newline and > to prompt user for input
		if(printit){
			printf("%s\n",output);
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
