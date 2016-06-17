/*First Attempt at a POSIX program in C*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "proco1.h"

//global variables
int seed,rc,numberOfRandomNumbers;
pthread_mutex_t mutex;
circular_buffer *cb;
FILE *fc, *fc2;

/* Αντιγράψτε από εδώ στο αρχείο .c */

//initialize circular buffer
//capacity: maximum number of elements in the buffer
//sz: size of each element
void cb_init(circular_buffer *cb, size_t capacity, size_t sz)
{
    cb->buffer = malloc(capacity * sz);
    if(cb->buffer == NULL){
    printf("Could not allocate memory..Exiting! \n");
    exit(1);
    }
        // handle error
    cb->buffer_end = (char *)cb->buffer + capacity * sz;
    cb->capacity = capacity;
    cb->count = 0;
    cb->sz = sz;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

//destroy circular buffer
void cb_free(circular_buffer *cb)
{
    free(cb->buffer);
    // clear out other fields too, just to be safe
}

//add item to circular buffer
void cb_push_back(circular_buffer *cb, const void *item)
{
    if(cb->count == cb->capacity)
        {
      printf("Access violation. Buffer is full\n");
      exit(1);
    }
    memcpy(cb->head, item, cb->sz);
    cb->head = (char*)cb->head + cb->sz;
    if(cb->head == cb->buffer_end)
        cb->head = cb->buffer;
    cb->count++;
}

//remove first item from circular item
void cb_pop_front(circular_buffer *cb, void *item)
{
    if(cb->count == 0)
        {
      printf("Access violation. Buffer is empy\n");
      exit(1);
  }
    memcpy(item, cb->tail, cb->sz);
    cb->tail = (char*)cb->tail + cb->sz;
    if(cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;
    cb->count--;
}

/* Αντιγράψτε μέχρι εδώ στο αρχείο .c */


void initialization(){
  /*Initialize mutex lock */
  pthread_mutex_init(&mutex,NULL);
  // /*Initialize pthread attributes to default */
  // pthread_attr_init(&attr);
  /*Initialize circular buffer */

}


void *producer(void *producerId) {
  int *prodId;
  int r,i;

  prodId = (int *)producerId;

  printf("Hello darkness my old friend \n");
  printf("Hello from producer(thread) %d \n",*prodId);

  for(i = 0; i < numberOfRandomNumbers; i++){
    r = rand_r(&seed);

    /*Locking mutex*/
    rc = pthread_mutex_lock(&mutex);
    if (rc != 0){
      printf("ERROR: retrn code from pthread_mutex_lock*() is %d \n",rc);
      pthread_exit(&rc);
    }



    cb_push_back(cb, (void*)&r);

    /*Unlocking mutex*/
    rc = pthread_mutex_unlock(&mutex);
    if (rc != 0){
      printf("ERROR: retrn code from pthread_mutex_lock*() is %d \n",rc);
      pthread_exit(&rc);
    }


    fprintf(fc, "Producer %d : %d \n",*prodId,r );
  }
  pthread_exit(prodId);
}

void *consumer(void *consumerId){
  int *consId;
  consId = (int *)consumerId;
  printf("Hello from producer(thread) %d \n",*consId);

  /*Locking mutex*/
  rc = pthread_mutex_lock(&mutex);
  if (rc != 0){
    printf("ERROR: retrn code from pthread_mutex_lock*() is %d \n",rc);
    pthread_exit(&rc);
  }

  int *tmp_read = (int*)malloc(4);

  cb_pop_front(cb, (void*)tmp_read);

  fprintf(fc2,"Consumer %d :  \n",*consId);

  free(tmp_read);

  /*Unlocking mutex*/
  rc = pthread_mutex_unlock(&mutex);
  if (rc != 0){
    printf("ERROR: retrn code from pthread_mutex_lock*() is %d \n",rc);
    pthread_exit(&rc);
  }

  pthread_exit(consId);
}





int main(int argc, char *argv[]){
  pthread_t *threads;
  circular_buffer *cb;
  int threadCount;

  cb = (circular_buffer*)malloc(sizeof (struct circular_buffer));

  if (argc != 6){
    printf("Incorrect number of arguments.\n");
    exit(-1);
  }

  /*converts argv elements to integers */
  int numberOfProducers = atoi(argv[2]);
  int numberOfConsumers = atoi(argv[3]);
  numberOfRandomNumbers = atoi(argv[4]);
  int queueCapacity = atoi(argv[5]);
  seed = atoi(argv[6]);

  int totalNumberOfThreads = numberOfProducers + numberOfConsumers;
  threads = malloc(totalNumberOfThreads * sizeof(pthread_t));

  int countArray[totalNumberOfThreads];

  initialization();
  cb_init(cb,queueCapacity,4);

  //REMEMBER TO CLOSE THE FILES
  fc = fopen("producers_in.txt", "w");
  if (fc == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fc2 = fopen("consumers_in.txt", "w");
  if (fc == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  /* Create producer threads */
  for (threadCount = 0; threadCount < numberOfProducers; threadCount++){
    printf("Producer thread #%d \n", threadCount);

    countArray[threadCount] = threadCount + 1;

    rc = pthread_create(&threads[threadCount], NULL, producer, &countArray[threadCount]);
    if (rc != 0 ){
      printf("ERROR: return code from pthread_create() is %d \n",rc);
      exit(-1);
    }
  }

  /* Create consumer threads */
  for (threadCount = 0;threadCount < numberOfConsumers; threadCount++){
    printf("Consumer thread #%d \n",threadCount);

    countArray[threadCount + numberOfConsumers] = threadCount + numberOfConsumers + 1;

    rc = pthread_create(&threads[threadCount + numberOfConsumers],NULL, consumer,&countArray[threadCount + numberOfConsumers]);
    if (rc != 0 ){
      printf("ERROR: return code from pthread_create() is %d \n",rc);
      exit(-1);
    }
  }

  if (threads == NULL){
    printf("Mein Gott. Ze memory won't suffice");
    return -1;
  }

  /*Destroy the threads */
  void *status;
  for (threadCount = 0; threadCount < totalNumberOfThreads; threadCount){
    rc = pthread_join(threads[threadCount], &status);
    if (rc != 0 ){
      printf("ERROR: return code from pthread_join() is %d \n" , rc);
    }
    printf("MAIN: Thread %d as status code. \n", countArray[threadCount]);
  }

  /*Destroy mutex*/
  rc = pthread_mutex_destroy(&mutex);
  if (rc != 0){
    printf("ERROR: return code from pthread_mutex_destroy() is %d \n", rc);
    exit(-1);
  }

  fclose(fc);
  fclose(fc2);

  free(threads);
  return 1;
}
