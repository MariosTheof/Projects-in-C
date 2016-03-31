/* My first personal shell */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char* argv[])
{
   char  fileName[55];
   pid_t pid;	
   
   pid = fork(); 


   switch (pid = fork()) {
	   case 0:
		exec(fileName);
		
      	   default:	 
         //Infinite Loop   
         while(1){
	   printf(" %s > ", argv[0]);

	   scanf("%s", fileName); // gets filename
	   if (fileName[0] == NULL) continue;
	   printf("\n Entered file: %s",fileName); // prints the fileName
	
	  
	

			
   	}
   }





return 0;
}
