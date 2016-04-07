/* My first personal shell */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> /* for fork() */
#include<sys/types.h> /* for pid_t */
#include<sys/wait.h> /* fpr wait() */


int main(int argc, char* argv[])
{
   char  fileName[255];
   pid_t pid;	
   
   


   switch (pid = fork()) {
	   case -1: //Did not fork properly
		perror("fork");
		break;

	   case 0: //child
		 //execv(fileName[0],fileName);
		static char *argv[] = { "echo", "Foo is my name." , NULL };
		execv("/bin/echo", argv);
		
		puts("Oh my. If this prints, execv() must have failed");
		exit(EXIT_FAILURE);
		break;
      	   default: //parent
		 //Infinite Loop   
		 while(1){
		   printf(" %s > ", argv[0]);
		   scanf("%s", fileName); // gets filename
		   if (fileName[0] == '\0') continue;
		   printf("\n Entered file: %s",fileName); // prints the fileName
		   waitpid(pid,0,0); /* wait for child to exit() */
		   break;
	   	}
   }





return 0;
}
