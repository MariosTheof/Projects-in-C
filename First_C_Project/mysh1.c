/* My first personal shell */
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
int status;

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
      execlp(args[0],args[0],NULL);
      execv(args[0], args);
			fprintf(stderr, "OH MEIN GOTT! \n");
      exit(-1);

		default:
			waitPid = wait(&status);
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
        process();
    	} // /while
    	return 1;
    }
