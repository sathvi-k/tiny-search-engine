/* pageio.c --- 
 * 
 * 
 * Author: Sathvika R. Korandla
 * Created: Tue Oct 22 17:11:56 2019 (-0400)
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

int32_t pagesave(webpage_t *pagep, int id, char *dirname) {	
  //make the dirname if it doesnt exist
  char dir_path[50];
  char *path= "../";
  sprintf(dir_path,"%s%s",path,dirname);  
  struct stat dirstat;
  
  if (stat(dir_path,&dirstat) == -1) {
    mkdir (dir_path, 0700);
  }

  //open the file
  char file_path[50];
  sprintf(file_path,"%s/%d",dir_path,id);
  FILE *my_file = fopen(file_path, "w");

  // write the four lines
  char *url=webpage_getURL(pagep);
	fprintf(my_file,"%s\n",url);
	
  int depth=webpage_getDepth(pagep);
  fprintf(my_file,"%d\n",depth);

  int html_len=webpage_getHTMLlen(pagep);
	fprintf(my_file,"%d\n",html_len);

  char *html=webpage_getHTML(pagep);
  fprintf(my_file,"%s\n",html);
	
  //close the file
  fclose(my_file);
  return 0;
}

/*                                                                             
 * pageload -- loads the numbered filename <id> in direcory <dirnm>            
 * into a new webpage                                                          
 *                                                                             
 * returns: non-NULL for success; NULL otherwise                               
 */                                                                            
webpage_t *pageload(int id, char *dirnm){
	webpage_t *wp;
	char dir_path[50];
  char *path= "../";
  sprintf(dir_path,"%s%s",path,dirnm);
	char file_path[50];
  sprintf(file_path,"%s/%d",dir_path,id);
  FILE *my_file = fopen(file_path, "r");
	if (my_file==NULL){
		return NULL;
	}
	//reading from file
	char url[30];
	int depth;
	int html_length;
	fscanf(my_file, "%s %d %d", url, &depth, &html_length);

	fclose(my_file);

	wp=webpage_new(url,depth,NULL);
	if (!webpage_fetch(wp)){
		return NULL;
	}
	return wp;
}
