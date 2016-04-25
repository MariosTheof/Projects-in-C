#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h> /* for fork() */
#include<sys/types.h> /* for pid_t */
#include<sys/wait.h> /* fpr wait() */

char fileName[256];
char *args[129];
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



/*Executes a command */
void process(void){
	switch(pid = fork()) {
	  case -1:
		printf("panic: can't fork \n");
		exit(-1);
	  case 0:
		execlp(args[0], args[0], args[1], NULL);

		execv(args[0], args);

		fprintf(stderr, "Ooops! \n");
		break;

	  default:
		waitPid = wait(&status);


	}
}

/*Checks for a special symbol */
char checkForSymbol(void){
	int i;
	char c;
	for (i = 0; args[i] != '\0'; i++) {
	  if (!strcmp(args[i], ">"))
		return args[i][0];
	  }
	if (!strcmp(args[i], ">"))
		return args[i][0];
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
	}else if (!strcmp(args[0], "exit" )){
		break;
	}

	
	process();


	} // /while
	return 1;
}
