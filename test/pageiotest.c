/* pageiotest.c --- 
 * 
 * 
 * Author: Sathvika R. Korandla
 * Created: Tue Oct 22 21:28:11 2019 (-0400)
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

int main(void){
	char *dirname="pages";
	char *path="../";
	char dirpath[50];
	int id1=1;
	int id2=10;
	int id3=11;
	sprintf(dirpath,"%s%s",path,dirname);
	webpage_t *wp=pageload(id1,dirname); //load a page already in pages directory
	int32_t result1=pagesave(wp,id2,dirname); // save loaded copy to new file
	webpage_t *reload=pageload(id2,dirname); //load page from new file
	int32_t result2=pagesave(reload,id3,dirname);

	//deallocate memory
	webpage_delete(wp);
	webpage_delete(reload);
	wp=NULL;
	reload=NULL;
	
	printf("Result of pagesaves: %d %d\n",result1, result2);
	
	char filepath2[50];
	char filepath3[50];

	sprintf(filepath2,"%s/%d",dirpath,id2);
	sprintf(filepath3,"%s/%d",dirpath,id3);
	
	//open files
	FILE *fp1 = fopen(filepath2,"r");
	FILE *fp2 = fopen(filepath3,"r");
	
	//loop through all characters in both files until you get to end of a file
	char c1='x'; // initial values for now
	char c2='x';
	while(c1!=EOF && c2!=EOF) {
		// get next characters from both files
		c1=fgetc(fp1);
		c2=fgetc(fp2);

		// if both are end of file characters, then files are same
		if (c1==EOF && c2==EOF){
			printf("files are the same!\n");
		}

		// if characters are not same, files are not identical
		else if(c1!=c2){
			printf("files aren't the same!\n");
		}
	}
	fclose(fp1);
	fclose(fp2);
}
