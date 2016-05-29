/*First Attempt at a POSIX program in C*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *insertRoutine(void *threadId) {
  int *tid;
  tid = (int *)threadId;
  printf("Hello Darness my old friend");
  

  pthread_exit(tid);
}




int main(int argc, char *argv[]){
  pthread_t *threads;

  if (argc != 6)}{
    printf("Incorrect number of arguments.\n");
    exit(-1);
  }

  /*converts argv elements to integers */
  int numberOfProducers = atoi(argv[2]);
  int numberOfConsumers = atoi(argv[3]);
  int numberOfRandomNumbers = atoi(argv[4]);
  int queueCapacity = atoi(argv[5]);
  int seed = atoi(argv[6]);

  int queue[queueCapacity];
  int front = 0;
  int rear = -1;

  int totalNumberOfThreads = numberOfProducers + numberOfConsumers;
  threads = malloc(totalNumberOfThreads * sizeof(pthread_t));

  if (threads == NULL){
    printf("Mein Gott. Ze memory won't suffice");
    return -1;
  }

  int rc, threadCount;
  int countArray[totalNumberOfThreads];

  for (threadCount = 0; threadCount < totalNumberOfThreads ;threadCount++){
    printf("Main: creating thread %d\n", threadCount);

        countArray[threadCount] = threadCount + 1;

        rc = pthread_create(&threads[threadCount], NULL, PrintHello,&countArray[threadCount]);

        if (rc != 0 ) {
          printf("ERROR: return code from pthread_create() is %d\n", rc);
          exit(-1);
        }

        // /*dimiourgia tou thread*/
        //   rc = pthread_create(&threads[threadCount], NULL, PrintHello, &countArray[threadCount]);
        //
        // /*elegxos oti to thread dimiourgithike swsta.*/
        //   if (rc != 0) {
        //     printf("ERROR: return code from pthread_create() is %d\n", rc);
        //       exit(-1);
        //     }



  }



}
