#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for fork() */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* fpr wait() */
#include <fcntl.h>

char fileName[256];
char *args[129];
char *args2[129];
pid_t pid,waitPid;
int status;

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
			execlp(args[0],args[0], args[1],NULL);

			execv(args[0], args);

			fprintf(stderr, "Oops! \n");
			break;

		default:
			waitPid = wait(&status);
		}

}

/*Checks for a special symbol in our command such as "<" or ">" */
char findSpecialSymbol(void){
	int i;
	char c;
	for ( i = 0; args[i] != '\0'; i++){
		if (!strcmp(args[i], ">"))
	 return args[i][0];
		if (!strcmp(args[i], "<"))
			 return args[i][0];
	}
	return '\0';
}

//Splits one command with arguments into two commands with arguments
void extractCommand(char* symbol) {
  int i = 0;
  int count = 0;
  for (i = 0; args[i] != NULL; i++)
      if (!strcmp(args[i], symbol)) {
         args[i] = NULL;
         while (args[i+1] != NULL) {
               args2[count] = args[i+1];
               args[i+1] = NULL;
               i++;
               count++;
         }
      }
}


//Redirects the standard output into a filename
void outRedirection(void) {
	extractCommand(">");
  int fd;
  if ((pid = fork()) == -1) {
      perror("fork");
      exit(1);
  }
  if (pid == 0) {
      close(1);
      fd = creat(args2[0], 0644);
      execvp(args[0], args);
      perror("execv");
      exit(1);
  }
  if (pid != 0) {
      wait(NULL);
      printf("Done ");
      printf(args[0]);
      printf(".\n");
  }
}

//Reads from a file as standard input
void inRedirection(void) {
	extractCommand("<");
  int fd;
  if ((pid = fork()) == -1) {
      perror("fork");
      exit(1);
  }
  if (pid == 0) {
      close(0);
      //open the file args2[0] and use it as standard input
      fd = open(args2[0], O_RDONLY);
      execvp(args[0], args);
      perror("execv");
      exit(1);
  }
  if (pid != 0) {
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

		if (feof(stdin)){
			break;
		}else if(!strcmp(args[0], "exit")){
			break;
		}

		switch(findSpecialSymbol()){
			case '<': inRedirection();
								break;

			case '>': outRedirection();
								break;

			default: process();
							 break;
		}

	} // /while

	return 1;
}
