/* My second personal shell */
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h> /* for fork() */
#include<sys/types.h> /* for pid_t */
#include<sys/wait.h> /* fpr wait() */
#include "mysh.h"
#include "functions.c"

    int main(int argc, char* argv[])
    {
    	while(1)
    	{
    		printf(" %s > ", argv[0]);
      	readLine();

    		if (args[0] == NULL)
    			continue;

    		if (feof(stdin)){
    			break;
    		}else if(!strcmp(args[0], "exit")){
    			break;
    		}
        process();
    	} // /while
    	return 1;
    }
