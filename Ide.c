#include <stdio.h>
#include <pthread.h>

int n,s,numOfthreads;

int main(void) {
	// your code goes here
	
	printf("Enter size of array: \n");
	 //input_value
    scanf("%d", &n);
	
	printf("Enter seed: \n");
	
	scanf("%d", &s);
	
	printf("Choose 1,2,4,8 threads : \n");
	
	scanf("%d", &numOfthreads);
	
	printf("Size of array : %d , seed : %d , number of threads : %d ", n, s, numOfthreads);
	return 0;
}

