#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for fork() */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* fpr wait() */
#include <fcntl.h>

char fileName[256];
char *args[129];
pid_t pid,waitPid;
int status,input,output,myPipe;
char *output_filename[5];
char *input_filename[5];
char *pipeCommand[5];

typedef enum { false, true } bool;

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
void process(int commandType){
	switch (pid = fork()) {
		case -1:
			printf("panic: can't fork\n");
			exit(-1);
		case 0:
		 if (commandType == 0){
			/*execlp goes first to run a shell command if there is one.
			If not, then execv will execute a file of our own. */
			execlp(args[0],args[0], args[1],NULL);
			execv(args[0], args);
			fprintf(stderr, "Oops! \n");
			break;
		}
		default:
			waitPid = wait(&status);
		}
}

int checkForPipes(void){
 int i,j, x=0;
 for (i = 0; args[i] != NULL; i++){
  if (args[i][0] == '|'){
 	x=1;
   args[i] = NULL;
   //Get myPipe command
   if (args[i + 1] !=  NULL) {
    pipeCommand[0] = args[i + 1];
   } else {
    return -1;
   }
	 // Adjust the rest of the arguments in the array
       for(j = i; args[j+1] != NULL; j++) {
   args[j] = args[j+1];
       }
    args[j] = NULL;
  }
 }
 if (x==1){return 1;}else{return 0;}
}

/* */
int redirectInput() {
	int i,j;
  for(i = 0; args[i] != NULL; i++) {
    if(args[i][0] == '<') {
      args[i] = NULL;
      if(args[i+1] != NULL) {
	input_filename[0] = args[i+1];
	  } else {
	return -1;
      }
		//	Adjust the rest of the arguments in the array
       for(j = i; args[j-1] != NULL; j++) {
	 			printf("ROAR! \n");
	 args[j] = args[j+2];
       }

    return 1;
    }
  }
  return 0;
}

/*
 * Check for output redirection
 */
int redirectOutput(void) {
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
			//	Adjust the rest of the arguments in the array
	       for(j = i; args[j-1] != NULL; j++) {
		 			printf("ROAR! \n");
		 args[j] = args[j+2];
	       }

				printf("args[0] = %s\n",args[0] );
				printf("args[1] = %s\n",args[1] );
				printf("args[2] = %s\n",args[2] );
				printf("args[3] = %s\n",args[3] );
				printf("args[4] = %s\n",args[4] );
				printf("args[5] = %s\n",args[5] );
				printf("args[6] = %s\n",args[6] );
				printf("args[7] = %s\n",args[7] );
				printf("args[8] = %s\n",args[8] );
				printf("args[9] = %s\n",args[9] );
				printf("args[10] = %s\n",args[10] );



      return 1;
    }
  }
  return 0;
}

void piping(void){
	if (input == 1)
		freopen(input_filename[0], "r", stdin);

	if (output == 1)
		freopen(output_filename[0], "w+", stdout);

	int fd[2];
	if (pipe(fd) < 0)
		printf("Cannot get a pipe \n" );
	if ((pid = fork()) == 0){
		close(1);
		dup(fd[1]);
		close(fd[0]);
		execvp(args[0], args);
	} else if (pid > 0){
		close (0);
		dup(fd[0]);
		close(fd[1]);
		execvp (pipeCommand[0],pipeCommand);
	}else{
		printf("Unable to fork\n");
	}
}

void pipeLoop(char **args){
	int i = 0;
	int p[2];
	int fd_in = 0;
	while (args[i] != NULL){
		pipe(p);
		if ((pid = fork()) == -1){
			perror("OH MEIN GOTT!");
			exit(EXIT_FAILURE);
		}else if (pid == 0){
			dup2(fd_in, 0); //change the input according to the old one
			if (args[i + 1] != NULL){
				dup2(p[1],1);
				close(p[0]);
				execvp(args[0], *args);
        exit(EXIT_FAILURE);
			}else{
				wait(NULL);
				close(p[1]);
				fd_in = p[0];//save the input for the next command
				args++;
			}
		}
	}
}

void inputOutput(void){
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

		input = redirectInput();

		output = redirectOutput();
		myPipe = checkForPipes();
		
		if (myPipe == 1){
			pipeLoop(args);
		}
		if (input == 1 || output == 1){
			inputOutput();
		}else{ process(0);}
	} // /while
	return 1;
}
