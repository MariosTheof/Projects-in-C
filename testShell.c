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
int status,input,output;
char *output_filename[5];
char *input_filename[5];

typedef enum { false, true } bool;

/*Reads a line from the terminal */
void readLine(void)
{

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
	int i = 0;
	while (args[i] != NULL){
		printf("%s \n ",args[i]);
		i++;
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
		}// else if (commandType == 1) {
		// 	piping();
		// }

		default:
			waitPid = wait(&status);
		}

}

/*Checks for a special symbol in our command such as "<" or ">" */
int findSpecialSymbol(void){
	int i,numOfSpSym = 0;
	char c;
	bool booleanOutput = false;
	bool booleanInput = false;
	for ( i = 0; args[i] != '\0'; i++){
		if (!strcmp(args[i], ">")){
			booleanOutput = true;
			numOfSpSym++;
		}
	 	if (!strcmp(args[i], "<")){
			booleanInput = true;
			numOfSpSym++;
		}

		// if (!strcmp(args[i], "|"))
		// 	return args[i][0];
	}
	return numOfSpSym;
}



	//Splits one command with arguments into two commands with arguments
// void extractCommand(char* symbol) {
//   int i;
//   int count = 0;
//   for (i = 0; args[i] != NULL; i++)
//       if (!strcmp(args[i], symbol)) {
// 				printf ("word that will be nulled %s \n",args[i]);
//          args[i] = NULL;
//          while (args[i+1] != NULL) {
//                args2[count] = args[i+1];
// 							 printf ("word that will be nulled %s \n",args[i + 1]);
//                args[i+1] = NULL;
//                i++;
//                count++;
//          }
//       }
// 			i = 0;
// 			while (args2[i] != NULL) {
// 				printf ("This is args2[i] :  ");
// 				printf("%s\n",args2[i] );
// 				i++;
// 			}
// }


// //Redirects the standard output into a filename
// void outRedirection(void) {
// 	extractCommand(">");
//   int fd;
//   if ((pid = fork()) == -1) {
//       perror("fork");
//       exit(1);
//   }
//   if (pid == 0) {
//       close(1);
//       fd = creat(args2[0], 0644);
// 			execvp(args[0], args);
//       perror("execv");
//       exit(1);
//   }
//   if (pid != 0) {
//       wait(NULL);
//
// 			printf("Done ");
//       printf(args[0]);
//       printf(".\n");
//   }
// }
//
// //Reads from a file as standard input
// void inRedirection(void) {
// 	extractCommand("<");
//   int fd;
//   if ((pid = fork()) == -1) {
//       perror("fork");
//       exit(1);
//   }
//   if (pid == 0) {
//       close(0);
//       //open the file args2[0] and use it as standard input
//       fd = open(args2[0], O_RDWR);
//       execvp(args[0], args);
//       perror("execv");
//       exit(1);
//   }
//   if (pid != 0) {
//       wait(NULL);
// 			//	printf("I am the parent process %d \n",getpid());
//       printf("Done ");
//       printf(args[0]);
// 			printf(".\n");
//   }
// }
//
// void piping(void) {
// 	extractCommand("|");
// 	int fd[2];
// 	if(pipe(fd) < 0)
// 		printf ("Cannot get a pipe\n");
// 	if ((pid = fork()) == 0){
// 		close(1);
// 		dup(fd[1]);
// 		close(fd[0]);
// 		execvp(args[0],args);
// 	} else if (pid > 0) {
// 			close(0);
// 			dup(fd[0]);
// 			close(fd[1]);
// 			execvp(args2[0],args2);
// 	} else
// 		printf("Unable to fork \n");
//
// }

int redirect_input(void) {
	printf("HELLO");
  int i;
  int j;
  for(i = 0; args[i] != NULL; i++) {
    if(args[i][0] == '<') {
      free(args[i]);
      if(args[i+1] != NULL) {
	input_filename[0] = args[i+1];
      } else {
	return -1;
      }
			// Adjust the rest of the arguments in the array
      for(j = i; args[j-1] != NULL; j++) {
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
int redirect_output(void) {
  int i;
  int j;

  for(i = 0; args[i] != NULL; i++) {
    // Look for the >
    if(args[i][0] == '>') {
      free(args[i]);
      // Get the filename
      if(args[i+1] != NULL) {
	output_filename[0] = args[i+1];
      } else {
	return -1;
      }
      // Adjust the rest of the arguments in the array
      for(j = i; args[j-1] != NULL; j++) {
	args[j] = args[j+2];
      }
      return 1;
    }
  }
  return 0;
}

void inputOutput(void){
	int result;
	if ((pid = fork())== -1){
		perror("fork");
		exit(1);
	}if (pid == 0){

	if (input == 1)
		freopen(input_filename[0], "r", stdin);

 	if (output == 1)
		freopen(output_filename[0], "w+", stdout);

	result =	execvp(args[0],args);
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
		printf ("can you enter?");
		input = redirect_input();
		output = redirect_output();
		switch (findSpecialSymbol()) {
			case 0:	process(0);
							break;
			default:

							printf("Sand is coarse and irrititating \n");

							inputOutput();
							break;
		}

		// switch(findSpecialSymbol()){
		// 	case '<': inRedirection();
		// 						break;
		//
		// 	case '>': outRedirection();
		// 						break;
		// 	case '|': process(1);
		// 						break;
		//
		// 	default: process(0);
		// 					 break;
	 	//}

	} // /while

	return 1;
}
