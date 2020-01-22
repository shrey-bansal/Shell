#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h> 


#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 100 // max number of commands to be supported 

void init_(){
    printf("\n\n\t<-------MY SHELL------->\n"); 
}

  
// function for parsing command words 
void parser(char* str, char* parsed_str[]) 
{   
    parsed_str[0] = strtok(str, " ");
    for (int i = 1; i < MAXLIST; i++) { 
        parsed_str[i] = strtok(NULL, " "); 
  		if (parsed_str[i] == NULL) break;
  		if (strlen(parsed_str[i]) == 0) i--; 
    } 
} 

int parse_output(char* str[], char* parsed_out[]){
	
	for(int i=0;str[i]!=NULL;i++){
		if(strcmp(str[i],">")==0){
			if(str[i+1]!=NULL)
				parsed_out[0] = str[i+1];
			else
				return 0;
			if(i==0){
				str[i] = str[i+2];
				i++;
			}
			for(int j = i; str[j-1] != NULL; j++) {
				str[j] = str[j+2];
      		}
      		return 1;
		}
		
	}
	return 0;
}
  
int parse_input(char* str[], char* parsed_in[]){
	
	for(int i=0;str[i]!=NULL;i++){
		if(strcmp(str[i],"<")==0){
			if(str[i+1]!=NULL)
				parsed_in[0] = str[i+1];
			else
				return 0;
			if(i==0){
				str[i] = str[i+2];
				i++;
			}
			for(int j = i; str[j-1] != NULL; j++) {
				str[j] = str[j+2];
      		}
      		return 1;
		}
	}
	return 0;
}

int parse_pipe(char* str[]){
	int count =0;
	for(int i=0;str[i]!=NULL;i++){
		if(strcmp(str[i],"|")==0)
			count++;
	}
	return count;
}

void preprocess(char* str, char* args[], char* parsed_in[],char* parsed_out[],int arr[]) 
{ 
	char tempstr[MAXCOM];
	strcpy(tempstr,str);
	parser(tempstr,args);
	int out = 0;
	out = parse_output(args,parsed_out);
	int in = 0;
	in = parse_input(args,parsed_in);
	int pipe;
	pipe = parse_pipe(args);
	arr[0] = pipe;
	arr[1] = in;
	arr[2] = out;

} 

void cmd_helper(char* parsed[],char* inp[],char* output[],int a[]) 
{ 
    if(a[0]==0 && a[1]==0 && a[2]==0){
	    int number_cmd = 5;  
	  	if(strcmp(parsed[0],"cd")==0){
	  		printf("%s\n","Custom Change Directory..");
	  		int temp = chdir(parsed[1]); 
	      if(temp!=0)
	        printf("%s\n","No such directory..");
	        return;
	  	}
	  	if(strcmp(parsed[0],"pwd")==0){
	  		char dir[1024];
	    	getcwd(dir, sizeof(dir)); 
	    	printf("%s\n","Custom Current Directory..");
	    	printf("$: %s\n", dir); 
	    	return;
	  	}
	  	if(strcmp(parsed[0],"mkdir")==0){
	  		printf("%s\n","Custom Make Directory..");
	  		int temp = mkdir(parsed[1],0777);
	      if(temp!=0)
	        printf("%s\n","Unable to create directory..");
	  		return;	  	
	  	}
	  	if(strcmp(parsed[0],"rmdir")==0){
	  		printf("%s\n","Custom Remove Directory..");
	  		int temp = rmdir(parsed[1]);
	      if(temp!=0){
	        printf("%s\n","No such directory or non-empty directory..");
	      }
	  		return;	  	
	  	}
	  	if(strcmp(parsed[0],"exit")==0){
	  		printf("Exiting shell..\n"); 
	        exit(0);
	  	}
	}
	if(a[0]){
		int b[a[0]+2];
		int i=0;
		b[0] = -1;
		i++;
		int j=0;
		for(j=0;parsed[j]!=NULL;j++){
			if(strcmp(parsed[j],"|")==0){
				b[i] = j;
				i++;
			}
		}
		b[a[0]+1] = j;

		int in=0;
		int fd[2];
		for(i=1;i<a[0]+2;i++){
			int size = b[i] - b[i-1];
			char* temp[size];
			int k = b[i-1]+1;
			for(int l=0;l<size-1;l++){
				temp[l] = parsed[k];
				k++;
			}
			temp[size-1]=NULL;
			pipe(fd);
			pid_t p1,tpid;
			int child_status;
			p1 = fork();
			if(p1==0){
				
				if(in!=0){
					dup2(in,0);
					close(in);
				}
				if(fd[1]!=1 && i!=a[0]+1){
					dup2(fd[1],1);
					close(fd[1]);
				}
				if(i==1 && a[1]==1){
					int infr = open(inp[0],O_RDONLY);
	      			dup2(infr,0);
	     			close(infr);
				}
				if(a[2]==1 && i==a[0]+1){
					int outr = creat(output[0],0664);
	     			dup2(outr,1);
	      			close(outr);
				}
				execv(temp[0],temp);
  				execvp(temp[0], temp);
       			printf("Unknown command..\n"); 
        		exit(0); 
			}
			else{
				// waiting for child to terminate 
	       		do {
	       		tpid = wait(&child_status);
	       		// if(tpid != child_pid) process_terminated(tpid);
	     		} while(tpid != p1);
			}
			close(fd[1]);
			in  = fd[0];
		}
		

	}
	else{

		pid_t child_pid,tpid;
	  	int child_status;
	 	child_pid = fork(); 
	  	if (child_pid == 0) { 
	  		if(a[1]==1){
		      int in = open(inp[0],O_RDONLY);
		      dup2(in,0);
		      close(in);
		    }
		    if(a[2]==1){
		      int out = creat(output[0],0664);
		      dup2(out,1);
		      close(out);
		  	}
	      	execv(parsed[0],parsed);
	  		execvp(parsed[0], parsed);
	        printf("Unknown command..\n"); 
	        exit(0); 
	    }
	    else { 
	        // waiting for child to terminate 
	       	do {
	       	tpid = wait(&child_status);
	       	// if(tpid != child_pid) process_terminated(tpid);
	     	} while(tpid != child_pid);
	    } 
  	}
}

int main() 
{ 
    char inp[MAXCOM];
    char* parsedArgs_1[MAXLIST];
    char* parsedArgs_2[MAXLIST];
    char* parsedArgs_3[MAXLIST];
    init_(); 
    while(1){
        // take input 
        printf("shell> "); 
  		if(fgets(inp, sizeof(inp), stdin) == NULL)
  			break;
  		if(inp[strlen(inp)-1] == '\n') { //remove newline char    
            inp[strlen(inp)-1] = '\0';    
        }
        if(strlen(inp)==0){
        	continue;
        }
        // process 
        int a[3];
        preprocess(inp,parsedArgs_1,parsedArgs_2,parsedArgs_3,a);
        // execute
        cmd_helper(parsedArgs_1,parsedArgs_2,parsedArgs_3,a);
    }
  
    return 0; 
} 
