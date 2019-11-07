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
#include <indexio.h>
#include <webpage.h>
#include <queue.h>
#include <hash.h>
#include <pageio.h>
#include <dirent.h>
#include <sys/stat.h>

FILE* goutputfile;

typedef struct index{
	hashtable_t *hashtable;
}index_t;

typedef struct word{
	char word[100];
	queue_t *queue;
}word_t;

typedef struct counter{
	int id;
	int count;
}counter_t;

typedef struct docrank{
	int id;
	int rank;
	char url[500];
}docrank_t;

typedef struct ANDquery{
	char query[1000];
}ANDquery_t;

void docrankclose(void *data){
	docrank_t *doc=(docrank_t*)data;
	free(doc);
}

void qdataclose(void *data){
	counter_t *doc=(counter_t*)data;
	free(doc);                         
}

void word_delete(void *data){
	word_t *wtp=(word_t*)data;
	queue_t *qp=wtp->queue;
	qapply(qp,qdataclose);
	qclose(qp);
	free(wtp);
}    

bool hsearchfn(void* elementp,const void* searchkeyp){
	word_t *wrdp2=(word_t*)elementp;
	char *wordP = (char*)searchkeyp;
	char *wrdp2_word=wrdp2->word;
	if(strcmp(wordP,wrdp2_word)==0){
		return true;
	}
	else{
		return false;
	}
}

bool qsearchfn(void* elementp,const void* searchkeyp){
	counter_t *cp=(counter_t*)elementp;
	int *id=(int*)searchkeyp;
	int cpid=cp->id;

	if(cpid==*id){
		return true;
	}

	else{
		return false;
	}
}

bool docqsearchfn(void* elementp,const void* searchkeyp){
	docrank_t *docp=(docrank_t*)elementp;
	int *id=(int*)searchkeyp;
	int docid=docp->id;

	if(docid==*id){
		return true;
	}

	else{
		return false;
	}
}

void print_rank_queue(void *data){
	docrank_t *doc=(docrank_t*)data;
	int rank=doc->rank;
	int id=doc->id;
	char *url=doc->url;
	if(rank==2147483647){
		rank=0;
	}
	printf("rank:%d : doc:%d : %s\n",rank,id,url);
}

void fprint_rank_queue(void *data){
	docrank_t *doc=(docrank_t*)data;
	int rank=doc->rank;
	int id=doc->id;
	char *url=doc->url;
	if(rank==2147483647){
		rank=0;
	}
	fprintf(goutputfile,"rank:%d : doc:%d : %s\n",rank,id,url);
}

void print_anything(void *elementp){                                                                                                                                                                                                                                              
  ANDquery_t *word=(ANDquery_t*)elementp;                                                                                                           
  printf("word in queue:%s\n", word->query);                                                                                                                                                                                                                              
}

queue_t* createqueryqueue(char* query){
	queue_t *queue=qopen();
	char *token;
	char ANDstring[1000]="";
	
	token=strtok(query," ");
	
	while(token!=NULL){
		if(strcmp(token,"or")==0){
			ANDquery_t *stringput=(ANDquery_t*)malloc(sizeof(ANDquery_t));
			strcpy(stringput->query,ANDstring);
			qput(queue,stringput);
			sprintf(ANDstring,"%s","");
		}
		else{
			sprintf(ANDstring,"%s%s ",ANDstring,token);
		}
		token=strtok(NULL," ");
	}
	ANDquery_t *stringput=(ANDquery_t*)malloc(sizeof(ANDquery_t));
	strcpy(stringput->query,ANDstring);
	qput(queue,stringput);
	return queue;
}

int main(int argc,char* argv[]){

	if(argc==3){

		char *pagedir=argv[1];
		char *indexnm=argv[2];
		queue_t *docqueueholder=qopen();
		int idmax=0;
		
		char dir_path[50];
		char *path= "../";
		sprintf(dir_path,"%s%s",path,pagedir);
		struct stat dirstat;                                                                                                   
		
    
		if (stat(dir_path,&dirstat) == -1) {                                                                                   
			printf("Directory does not exist\n");                                                                                  
			exit(EXIT_FAILURE);                                                                                                  
		}

		FILE *indexfile;
		char index_path[50];
		char *ipath="../indexer/";
		sprintf(index_path, "%s%s",ipath,indexnm);

		if((indexfile=fopen(index_path,"r"))){
			fclose(indexfile);
		}
		else{
			printf("Indexfile does not exist\n");
			exit(EXIT_FAILURE);
		}
		
		DIR *dir;
		struct dirent *entry;
		dir=opendir(dir_path);	
		while((entry=readdir(dir)) != NULL){
			if((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0)){
				;                             
			}
			else{
				idmax=idmax+1;
			}
		}          
		closedir(dir);
		
		index_t *index = indexload(indexnm);
		hashtable_t *hashtable = index->hashtable;

		bool printit=true;
		char input[1000];
		
		printf("> ");	
		while(fgets(input,1000,stdin)!=NULL){
			
			if(input[0] == '\n'){
				printit=false;
			}
			else if(input[0] == '\t'){
				printit=false;
			}	
			else if(input[strlen(input)-2] == '\t'){
				printit=false;
			}
			
			input[strlen(input)-1]='\0';
			
			// split user's input by spaces and tabs
			char *token = strtok(input," \t");
			char output[1000]="";
			char printedoutput[1000]="";
			char *prevtoken="";
			
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

				if(strcmp(prevtoken,"and")==0 && strcmp(token,"and")==0){
					printit=false;
				}
				else if(strcmp(prevtoken,"or")==0 && strcmp(token,"or")==0){
					printit=false;
				}
				else if(strcmp(prevtoken,"and")==0 && strcmp(token,"or")==0){
					printit=false;
				}
				else if(strcmp(prevtoken,"or")==0 && strcmp(token,"and")==0){
					printit=false;
				}
				else if(strcmp(prevtoken,"")==0 && strcmp(token,"and")==0){
					printit=false;
				}
				else if(strcmp(prevtoken,"")==0 && strcmp(token,"or")==0){
					printit=false;
				}
				
				sprintf(output,"%s%s ",output,token);
				sprintf(printedoutput,"%s%s ",printedoutput,token);
				prevtoken=token;
				token = strtok(NULL," \t");
			}

			if(strcmp(prevtoken,"and")==0 || strcmp(prevtoken,"or")==0){
				printit=false;
			}
			
			queue_t *queryqueue=createqueryqueue(output);
			//qapply(queryqueue,print_anything);
			ANDquery_t *ANDqueryt;
			
			while((ANDqueryt=(ANDquery_t*)qget(queryqueue))!=NULL){
				char* ANDquery=ANDqueryt->query;
				queue_t *docqueue=qopen();
				int idcount=1;
				
				while(idcount<=idmax){
					docrank_t *docrank = (docrank_t*)malloc(sizeof(docrank_t));
					docrank->id=idcount;
					docrank->rank=2147483647;
					
					char webpage_path[300];
					sprintf(webpage_path,"%s/%d",dir_path,idcount);
					
					FILE *webpage=fopen(webpage_path,"r");
					char url[500];
					fscanf(webpage,"%s",url);
					strcpy(docrank->url,url);
					fclose(webpage);
					
					qput(docqueue,docrank);
					
					idcount++;
				}
				
				char *ANDtoken=strtok(ANDquery," ");
				while(ANDtoken!=NULL){
					
					if(strcmp(ANDtoken,"and")==0 || strlen(ANDtoken)<3){
						;
					}
					
					else{   
						word_t *foundword;
						//search for token in hashtable using hsearch
						foundword=hsearch(hashtable,hsearchfn,ANDtoken,strlen(ANDtoken));
						
						if(foundword!=NULL){		
							int id=1;
							
							while(id<=idmax){
								
								queue_t *queue=foundword->queue;
								counter_t *doc=qsearch(queue,qsearchfn,(const void*)&id);
								
								if(doc!=NULL){
									int count=doc->count;
									docrank_t *sdoc=qsearch(docqueue,docqsearchfn,(const void*)&id);
									
									if(sdoc!=NULL){
										if(count<(sdoc->rank)){
											sdoc->rank=count;
										}
									}
									
								}
								
								else{
									docrank_t *rdoc=qremove(docqueue,docqsearchfn,(const void*)&id);
									
									if(rdoc!=NULL){
										free(rdoc);
									}
									
								}
								id++;	
							}
						}
						
						else{
							printit=false;
							//printf("word not in index\n");
						}
						
					}
					ANDtoken=strtok(NULL," ");
					
				}
				qput(docqueueholder,docqueue);
				free(ANDqueryt);
			}
			qclose(queryqueue);
			//loop through docqueueholder and get the overall count for each document 
			queue_t *newqueue=qopen(); //queue holding a docrank object for each document in pages directory and total end rank (this is the one we print at the end)
			int count1=1;
			queue_t *docqueue1;
			docrank_t *doc1;
			docrank_t *docnew;
			
			while(count1<=idmax){
				docrank_t *newdoc=(docrank_t*)malloc(sizeof(docrank_t));
				newdoc->id=count1;
				newdoc->rank=0;
				
				char webpage_path1[300];
				sprintf(webpage_path1,"%s/%d",dir_path,count1);
				
				FILE *webpage1=fopen(webpage_path1,"r");
				char url1[500];
				fscanf(webpage1,"%s",url1);
				strcpy(newdoc->url,url1);
				fclose(webpage1);
				
				qput(newqueue,newdoc);
				count1++;
			}
			
			
			while((docqueue1=(queue_t*)qget(docqueueholder))!=NULL){
				
				int count2=1;
				while(count2<=idmax){
					doc1=qsearch(docqueue1,docqsearchfn,(const void*)&count2); //
					docnew=qsearch(newqueue,docqsearchfn,(const void*)&count2);
					
					if(doc1==NULL){
						;
					}
					
					else{
						int docsrank=doc1->rank;
						int newrank=docnew->rank;
						docnew->rank=newrank+docsrank;
					}
					count2++;
				}
				qapply(docqueue1,docrankclose);
				qclose(docqueue1);
			}
			
			// print newline and > to prompt user for input
			if(printit){
				printf("%s\n",printedoutput);
				qapply(newqueue,print_rank_queue);
				printf("> ");
			}
			else{
				printf("[invalid query]\n");
				printf("> ");
				printit=true;
			}
			qapply(newqueue,docrankclose);
			qclose(newqueue);
		}
		
		qclose(docqueueholder);
		
		happly(hashtable,word_delete);
		hclose(hashtable);
		free(index);
		
		printf("^D\n");
		return 0;
	}
	
	if(argc==6){
		char *pagedir=argv[1];
		char *indexnm=argv[2];
		char *myqueries=argv[4];
		char *myoutput=argv[5];
		
		
		if(strcmp(argv[3],"-q")==0){
			queue_t *docqueueholder=qopen();
			int idmax=0;
			
			char dir_path[50];
			char *path= "../";
			sprintf(dir_path,"%s%s",path,pagedir);
			struct stat dirstat;                                                                                                   
		 
			if (stat(dir_path,&dirstat) == -1) {                                                                                   
				printf("Directory does not exist\n");                                                                                  
				exit(EXIT_FAILURE);
			}
			
			FILE *indexfile;                                                                                
			char index_path[50];                                                                                        
			char *ipath="../indexer/";                                                                
			sprintf(index_path, "%s%s",ipath,indexnm);                                           
                                                                                                                                                   
			if((indexfile=fopen(index_path,"r"))){          
				fclose(indexfile);                                                                                         
			}                                                                                                                      
			else{                                                                                                      
				printf("Indexfile does not exist\n");                                                                  
				exit(EXIT_FAILURE);                                                                                                                       
			}        
			
	
			DIR *dir;
			struct dirent *entry;
			dir=opendir(dir_path);	
			while((entry=readdir(dir)) != NULL){
				if((strcmp(entry->d_name,".")==0) || (strcmp(entry->d_name,"..")==0)){
					;                             
				}
				else{
					idmax=idmax+1;
				}
			}          
			closedir(dir);

			FILE *queryfile;
			FILE *outputfile=fopen(myoutput, "w");
			goutputfile=outputfile;
			
			if((queryfile=fopen(myqueries, "r"))){
				;
			}
			else{
				printf("queryfile does not exist\n");
				exit(EXIT_FAILURE);
			}
	
			index_t *index = indexload(indexnm);
			hashtable_t *hashtable = index->hashtable;
			
			bool printit=true;
			char input[1000];
			
			fprintf(outputfile,"> ");	
			while(fgets(input,1000,queryfile)!=NULL){
				
				if(input[0] == '\n'){
					printit=false;
				}
				else if(input[0] == '\t'){
					printit=false;
				}		
				else if(input[strlen(input)-2] == '\t'){
					printit=false;
				}
				
				input[strlen(input)-1]='\0';
				
				// split user's input by spaces and tabs
				char *token = strtok(input," \t");
				char output[1000]="";
				char printedoutput[1000]="";
				char *prevtoken="";
				
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

					if(strcmp(prevtoken,"and")==0 && strcmp(token,"and")==0){
						printit=false;
					}
					else if(strcmp(prevtoken,"or")==0 && strcmp(token,"or")==0){
						printit=false;
					}
					else if(strcmp(prevtoken,"and")==0 && strcmp(token,"or")==0){
						printit=false;
					}
					else if(strcmp(prevtoken,"or")==0 && strcmp(token,"and")==0){
						printit=false;
					}
					else if(strcmp(prevtoken,"")==0 && strcmp(token,"and")==0){
						printit=false;
					}
					else if(strcmp(prevtoken,"")==0 && strcmp(token,"or")==0){
						printit=false;
					}
					
					sprintf(output,"%s%s ",output,token);
					sprintf(printedoutput,"%s%s ",printedoutput,token);
					prevtoken=token;
					token = strtok(NULL," \t");	
				}

				if(strcmp(prevtoken,"and")==0 || strcmp(prevtoken,"or")==0){
					printit=false;
				}
				
				
				queue_t *queryqueue=createqueryqueue(output);
				//qapply(queryqueue,print_anything);
				ANDquery_t *ANDqueryt;
				
				while((ANDqueryt=(ANDquery_t*)qget(queryqueue))!=NULL){
					char* ANDquery=ANDqueryt->query;
					queue_t *docqueue=qopen();
					int idcount=1;
					
					while(idcount<=idmax){
						docrank_t *docrank = (docrank_t*)malloc(sizeof(docrank_t));
						docrank->id=idcount;
						docrank->rank=2147483647;
						
						char webpage_path[300];
						sprintf(webpage_path,"%s/%d",dir_path,idcount);
						
						FILE *webpage=fopen(webpage_path,"r");
						char url[500];
						fscanf(webpage,"%s",url);
						strcpy(docrank->url,url);
						fclose(webpage);
						
						qput(docqueue,docrank);
						
						idcount++;
					}
					
					char *ANDtoken=strtok(ANDquery," ");
					while(ANDtoken!=NULL){
						
						if(strcmp(ANDtoken,"and")==0 || strlen(ANDtoken)<3){
							;
						}
						
						else{   
							word_t *foundword;
							//search for token in hashtable using hsearch
							foundword=hsearch(hashtable,hsearchfn,ANDtoken,strlen(ANDtoken));
							
							if(foundword!=NULL){		
								int id=1;
								
								while(id<=idmax){
									
									queue_t *queue=foundword->queue;
									counter_t *doc=qsearch(queue,qsearchfn,(const void*)&id);
									
									if(doc!=NULL){
										int count=doc->count;
										docrank_t *sdoc=qsearch(docqueue,docqsearchfn,(const void*)&id);
										
										if(sdoc!=NULL){
											if(count<(sdoc->rank)){
												sdoc->rank=count;
											}
										}
										
									}
									
									else{
										docrank_t *rdoc=qremove(docqueue,docqsearchfn,(const void*)&id);
										
										if(rdoc!=NULL){
											free(rdoc);
										}
										
									}
									id++;	
								}
							}
							
							else{
								printit=false;
								//fprintf(outputfile, "word not in index\n");
							}
							
						}
						ANDtoken=strtok(NULL," ");
						
					}
					qput(docqueueholder,docqueue);
					free(ANDqueryt);
				}
				qclose(queryqueue);
				//loop through docqueueholder and get the overall count for each document 
				queue_t *newqueue=qopen(); //queue holding a docrank object for each document in pages directory and total end rank (this is the one we print at the end)
				int count1=1;
				queue_t *docqueue1;
				docrank_t *doc1;
				docrank_t *docnew;
				
				while(count1<=idmax){
					docrank_t *newdoc=(docrank_t*)malloc(sizeof(docrank_t));
					newdoc->id=count1;
					newdoc->rank=0;
					
					char webpage_path1[300];
					sprintf(webpage_path1,"%s/%d",dir_path,count1);
					
					FILE *webpage1=fopen(webpage_path1,"r");
					char url1[500];
					fscanf(webpage1,"%s",url1);
					strcpy(newdoc->url,url1);
					fclose(webpage1);
					
					qput(newqueue,newdoc);
					count1++;
				}
				
				
				while((docqueue1=(queue_t*)qget(docqueueholder))!=NULL){
					
					int count2=1;
					while(count2<=idmax){
						doc1=qsearch(docqueue1,docqsearchfn,(const void*)&count2); //
						docnew=qsearch(newqueue,docqsearchfn,(const void*)&count2);
						
						if(doc1==NULL){
							;
						}
						
						else{
							int docsrank=doc1->rank;
							int newrank=docnew->rank;
							docnew->rank=newrank+docsrank;
						}
						count2++;
					}
					qapply(docqueue1,docrankclose);
					qclose(docqueue1);
				}
				
				// print newline and > to prompt user for input
				if(printit){
					fprintf(outputfile,"%s\n",printedoutput);
					qapply(newqueue,fprint_rank_queue);
					fprintf(outputfile,"> ");
				}
				
				else{
					//fprintf(outputfile,"%s\n",printedoutput);
					fprintf(outputfile,"[invalid query]\n");
					fprintf(outputfile,"> ");
					printit=true;	
				}
				qapply(newqueue,docrankclose);
				qclose(newqueue);
			}
			
			qclose(docqueueholder);
			
			happly(hashtable,word_delete);
			hclose(hashtable);
			free(index);

			fprintf(outputfile,"^D\n");
			printf("^D\n");
			
			fclose(queryfile);
			fclose(outputfile);
			return 0;
		}
		else{
			printf("no such flag exists use -q\n");
		}	
	}	
	else{                                                                                                                    
		printf("usage:query <pageDirectory> <indexFile> [-q]\n");                                                                           
		exit(EXIT_FAILURE);                                                                                                    
	}   		
}
