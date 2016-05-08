#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h> /* for fork() */
#include<sys/types.h> /* for pid_t */
#include<sys/wait.h> /* fpr wait() */
#include "mysh.h"

int status;
char fileName[256];
pid_t pid,waitPid;
char *args[129];

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