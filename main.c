#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h> 



int main() 
{ 
    pid_t child_pid,tpid;
  	int child_status;
 	child_pid = fork(); 
 	char* parsed[] = {"exe/shell",NULL};
  	if (child_pid == 0) { 
      	execv(parsed[0],parsed);
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
    return 0; 
} 
