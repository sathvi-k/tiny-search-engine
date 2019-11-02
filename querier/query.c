/* query.c --- 
 * 
 * Author: Sathvika R. Korandla
 * Created: Thu Oct 31 22:29:05 2019 (-0400)
 * Version: 
 * 
 * Description: 
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void){
	char input[50];
	printf("> ");
	int returned;
	while((returned=scanf(" %[^\n]", input))==1){
		// split user's input by spaces and tabs
		char *token = strtok(input, " \t");
		while (token != NULL){
			// if user enters ^D, break out of loop and stop scanning input
			if (returned == EOF){
				break;
			}

			// take string user enters and check that it only has alphabetical
			// characters, and convert to lowercase letters
			for (int i=0;i<strlen(token);i++){
				if (isalpha(token[i])!=0){
					token[i]=tolower(token[i]);
				}
				//otherwise, there's numbers and punctuations, so it's invalid
				else{
					printf("[invalid query]");
				}
			}
			// after converting to lowercase, print word, then move pointer to next
			printf("%s ", token);
			token = strtok(NULL, " \t");
		}
		// print newline and > to prompt user for input
		printf("\n> ");
	}
	return 0;
}
