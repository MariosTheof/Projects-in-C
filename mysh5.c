/* My fifth personal shell */
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
int status,input,output,myPipe;
char *output_filename[5];
char *input_filename[5];
char *pipeCommand[5];

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

	/*Checks for input */
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

	// int checkForPipe(void){
	// 	int i,j;
	// 	for (i = 0; args[i] != NULL; i++){
	// 		if (args[i][0] == '|'){
	// 			args[i] = NULL;
	// 			//Get myPipe command
	// 			if (args[i + 1] !=  NULL) {
	// 				pipeCommand[0] = args[i + 1];
	// 			} else {
	// 				return -1;
	// 			}
	// 			// Adjust the rest of the arguments in the array
	//       for(j = i; args[j-1] != NULL; j++) {
	// 				args[j] = args[j+2];
	//       }
	//       return 1;
	// 		}
	// 	}
	// 	return 0;
	// }


	// void piping(void){
	// if (input == 1)
	// 	freopen(input_filename[0], "r", stdin);
	//
	// if (output == 1)
	// 	freopen(output_filename[0], "w+", stdout);
	//
	// int fd[2];
	// if (pipe(fd) < 0)
	// 	printf("Cannot get a pipe \n" );
	// if ((pid = fork()) == 0){
	// 	close(1);
	// 	dup(fd[1]);
	// 	close(fd[0]);
	// 	execvp(args[0], args);
	// } else if (pid > 0){
	// 	close (0);
	// 	dup(fd[0]);
	// 	close(fd[1]);
	// 	execvp (pipeCommand[0],pipeCommand);
	// }else{
	// 	printf("Unable to fork\n");
	// }
//}

void multiPipes(int numberOfPipes){
	int pipePositions[numberOfPipes];
	int new_fd[2], old_fd[2];
	int x = 0;
	int counter,i;
	char *cmd1[x];
	//For error handling
	int closeval;
	int dup2val;

	for(i = 0; args[i] != NULL; i++) {
				if(args[i][0] == '|'){
					pipePositions[x] = i;
					x++;
				}
			}

	for (counter = 0; counter <= numberOfPipes; counter++ ){
		/*Reset x so that args from previous pipe
		are overwritten by args for next pipe */
		x = 0;
		while(1){
			//if you reached end of args list or a pipe -> break
				if(args[i] == NULL || args[i][0] == '|'){
					i++;
					break;
				}
				cmd1[x] = args[i];
				i++;
				x++;
		}
		//Make array null terminating so it works with execvp() syscall
			cmd1[x] = NULL;
			//If you are not on the last loop then make a new pipe
			if(counter < numberOfPipes){
				int pipeval = pipe(new_fd);
				//Error handling
				if(pipeval == -1){
					printf("OH MEIN GOTT FIRST ERROR HANDLIN !\n");
				}
			}

			if ((pid = fork()) == -1 ){
				printf("OH MEIN GOTT FORK ERROR  !\n" );
			}
			if (pid == 0){ //child
				/*Do this only if a previous
				pipe has been written to by a child process in the past
				Read data from old pipe to
				use as input for executing next command	*/
				if(counter > 0){
					//Close write end of old pipe, wont need it
					closeval = close(old_fd[1]);
					//Error handling
					if(closeval == -1){
					printf("OH MEIN GOTT close() ERROR  !\n");
					}
					//Take as input data from the read end of the old pipe
					dup2val = dup2(old_fd[0], 0);
					//Error handling
					if(dup2val == -1){
						printf("OH MEIN GOTT dup2() ERROR  !\n");
					}
					//Close read end of old pipe
					closeval = close(old_fd[0]);
					//Error handling
					if(closeval == -1){
						printf("OH MEIN GOTT close() ERROR  !\n");
					}
			}

			/*Do this for every loop except for the last one
			Prepare to write output into the new pipe */
			if (counter < numberOfPipes){
				//close read of a new pipe, don't need it
				closeval = close(new_fd[0]);
				//error handling
				if(closeval == -1){
					printf("OH MEIN GOTT close() ERROR  !\n");
				}
				//redirect output
				dup2val = dup2(new_fd[1],1);
				//error handling
				if(dup2val == -1){
					printf("OH MEIN GOTT dup2() ERROR  !\n");
				}
				//close write end of new pipe
				closeval = close(new_fd[1]);
				//Error handling
				if(closeval == -1){
					printf("OH MEIN GOTT close() ERROR  !\n");
				}
			}

			//Execute command and die if sucessful
			execvp(cmd1[0], cmd1);

			//If child process doesn't die, print error number and manually kill child
			printf("OH MEIN GOTT ZE CHILD WON'T DIE !\n");
			exit(1);

		}else{ // parent begins
		//Do this for every loop except the first loop
		//The old pipe is not in use yet at the first loop
		if(counter > 0){
			//Closes both ends of the old pipe
			closeval = close(old_fd[0]);
			//Error handling
			if(closeval == -1){
				printf("OH MEIN GOTT close() ERROR !\n");
			}
			closeval = close(old_fd[1]);
			//Error handling
			if(closeval == -1){
				printf("OH MEIN GOTT close() ERROR !\n");
			}
		}

		//Do this for every loop but the last
		//Don't need this on last loop because the last child process displays its results
		//to the terminal, so no new data will be written into a new pipe
		if(counter < numberOfPipes){
			//The old pipe = the last new pipe used by a child process
			//store data outputted by last child process into the old pipe for the next child to read in
			old_fd[0] = new_fd[0];
			old_fd[1] = new_fd[1];

		}

		//On last loop only
		//Wait for last child to display its results to the terminal and then exit loop
		if(counter == numberOfPipes){
			int status;
			int waitval;
			waitval = wait(&status);
			//Error handling
			if(waitval == -1){
				printf("OH MEIN GOTT wait() ERROR  !\n");
			}
			break;
		}
	}
}
} // end function


    int main(int argc, char* argv[])
    {
			int numberOfPipes,i;
    	while(1)
    	{
				//resets the numberOfPipes
				numberOfPipes = 0;

    		printf(" %s > ", argv[0]);
      	readLine();

    		if (args[0] == NULL)
    			continue;

    		if (feof(stdin)){
    			break;
    		}else if(!strcmp(args[0], "exit")){
    			break;
    		}

				for(i = 0; args[i] != NULL; i++) {
						if(args[i][0] == '|'){
							numberOfPipes++;
						}
				}


				multiPipes(numberOfPipes);
				// input = checkForInput();
				// output = checkForOutput();
				//
				//
				//
				// if (input == 1 || output == 1){
				// 	IOcommand();
				// }else{ process();}
    	} // /while
    	return 1;
    }
