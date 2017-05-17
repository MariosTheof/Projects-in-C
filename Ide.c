#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n,s,numOfthreads;

int main(void) {
	
	
	
	printf("Enter size of array: \n");
	 //input_value
    scanf("%d", &n);
	
	printf("Enter seed: \n");
	
	scanf("%d", &s);
	
	printf("Choose 1,2,4,8 threads : \n");
	
	scanf("%d", &numOfthreads);
	
	printf("Size of array : %d , seed : %d , number of threads : %d \n", n, s, numOfthreads);
	
	int initialArray[n],i; // initial array that is going to be merge sorted
	
	srand(s); // Seeds the pseudo-random number generator
	
	// Fills the array with random generated numbers
	for (i = 0; i < n; i++ ){
	    initialArray[i] = rand();
	    printf("[%d] : %d \n", i+1, initialArray[i]);
	}
	
	
	

	return 0;
}
