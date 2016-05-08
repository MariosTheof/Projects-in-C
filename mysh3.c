/* My third personal shell */
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h> /* for fork() */
#include<sys/types.h> /* for pid_t */
#include<sys/wait.h> /* fpr wait() */
#include "mysh.h"
#include "functions.c"


int input,output;
char *output_filename[5];
char *input_filename[5];



	/*Checks for input */
	int checkForInput() {
			int i,j;
		  for(i = 0; args[i] != NULL; i++) {
		    if(args[i][0] == '<') {
		      args[i] = NULL;
		      if(args[i+1] != NULL) {
						input_filename[0] = args[i+1];
			  } else {
					return -1;
	      }
			//	For- loop to make input AND output functional
	       for(j = i; args[j-1] != NULL; j++) {
		 		 		args[j] = args[j+2];
	       }
	    return 1;
	    }
	  }
	  return 0;
	}

	/*Checks for output */
	int checkForOutput(void) {
	  int i;
	  int j;
	  for(i = 0; args[i] != NULL; i++) {
	    // Look for the >
	    if(args[i][0] == '>') {
	      args[i] = NULL;
	      // Get the filename
	      if(args[i+1] != NULL) {
		output_filename[0] = args[i+1];
	      } else {
		return -1;
	      }
				//For- loop to make input AND output functional
		       for(j = i; args[j-1] != NULL; j++) {
			 			 	args[j] = args[j+2];
		       }
	      return 1;
	    }
	  }
	  return 0;
	}

	void IOcommand(void){
		if ((pid = fork())== -1){
			perror("fork");
			exit(1);
		}if (pid == 0){

		if (input == 1)
			freopen(input_filename[0], "r", stdin);

	 	if (output == 1)
			freopen(output_filename[0], "w+", stdout);

			execvp(args[0],args);
			exit(-1);
	 }
	 if (pid != 0 ){
		 wait(NULL);
		 printf("Done ");
		 printf(args[0]);
		 printf(".\n");
	 }
	}

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

				input = checkForInput();
				output = checkForOutput();

				if (input == 1 || output == 1){
					IOcommand();
				}else{ process();}
    	} // /while
    	return 1;
    }
