#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h> /* for fork() */
#include<sys/types.h> /* for pid_t */
#include<sys/wait.h> /* fpr wait() */

int main(int argc, char* argv[])
{	
	pid_t pid,waitPid;
	int status;
	char fileName[256];
	while(!feof(stdin))
	{
	printf(" %s > ", argv[0]);
	if (fgets(fileName, sizeof(fileName), stdin) != 0)
	{
		fileName[strcspn(fileName, "\n")] = '\0'; //strcspn = calculates the length of the initial fileName segment, which  consists of chars not in str2("\n")
	
		char *args[129];
		char **argv = args;
		char *cmd = fileName;
		const char *whisp = " \t\f\r\b\n";
		char *token;
		while ((token = strtok(cmd,whisp)) != 0) // strtok = breaks string into a series of tokens using the delimeter delim(whisp)
		{
			*argv++ = token;
			cmd = 0;
		}// /while

		if (feof(stdin))
		{
		break;
		}


		*argv = 0;
		

		pid = fork();
		if (pid == 0){
		
		/*execlp goes first to run a shell command if there is one.
		If not, then execv will execute a file of our own. */
		execlp(args[0],args[0], args[1],NULL);

		execv(args[0], args);

		fprintf(stderr, "Oops! \n");
		break;
		} // /if
		waitPid = wait(&status);

		

	}// /if(fgets..)
	} // /while
	return 1;
}
