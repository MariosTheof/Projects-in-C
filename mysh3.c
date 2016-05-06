/* My third personal shell */
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h> /* for fork() */
#include<sys/types.h> /* for pid_t */
#include<sys/wait.h> /* fpr wait() */
#include "mysh.h"

char fileName[256];
char *args[129];
pid_t pid,waitPid;
int status,input,output;
char *output_filename[5];
char *input_filename[5];

/*Reads a line from the terminal */
void readLine(void){
	if (fgets(fileName,sizeof(fileName), stdin) != 0){
		fileName[strcspn(fileName, "\n")] = '\0'; //strcspn = string span ?

			char **argv = args;
			char *cmd = fileName;
			const char *whisp = " \t\f\r\b\n";
			char *token;

			while ((token = strtok(cmd,whisp)) != 0) // strtok = breaks string into a series of tokens using the delimeter delim(whisp)
			{
				*argv++ = token;
				cmd = 0;
			}// /while

			*argv = 0;
	}
}

/*Function that starts the process of executing */
void process(void){
	switch (pid = fork()) {
		case -1:
			printf("panic: can't fork\n");
			exit(-1);
		case 0:
      /*execlp goes first to run a shell command if there is one.
      If not, then execv will execute a file of our own. */
      execlp(args[0],args[0],args[1],NULL);
      execv(args[0], args);
			fprintf(stderr, "OH MEIN GOTT! \n");
      exit(-1);

		default:
			waitPid = wait(&status);
		}
  }

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
