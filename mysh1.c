/* My first personal shell */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char* argv[])
{
   char  fileName[255];
   pid_t pid;	
   
   


   switch (pid = fork()) {
	   case -1: //Did not fork properly
		perror("fork");
		break;

	   case 0: //child
		execvp(fileName[0],fileName);
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
		   //wait()
				
	   	}
   }





return 0;
}
