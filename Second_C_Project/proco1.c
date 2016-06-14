/*First Attempt at a POSIX program in C*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int seed; //global args


void *insertRoutine(void *threadId) {
  int *tid;
  tid = (int *)threadId;
  printf("Hello Darness my old friend");
}

void *producer(void *producerId,int numberOfRandomNumbers) {
  int *prodId;
  int r;
  prodId = (int *)producerId;
  printf("Hello darkness my old friend \n");
  printf("Hello from producer(thread) %d \n",*tid);
  int i;
  for(i = 0; i < numberOfRandomNumbers; i++){
    r = rand_r(seed);
    cb_push_back();

  }
}

void *consumer(void *consumerId){
  int *consId;
  consId = (int *)consumerId;
  cb_pop_front();
}


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


int main(int argc, char *argv[]){
  pthread_t *threads;
  circular_buffer *cb;
  int threadCount;

  if (argc != 6)}{
    printf("Incorrect number of arguments.\n");
    exit(-1);
  }

  /*converts argv elements to integers */
  int numberOfProducers = atoi(argv[2]);
  int numberOfConsumers = atoi(argv[3]);
  int numberOfRandomNumbers = atoi(argv[4]);
  int queueCapacity = atoi(argv[5]);
  seed = atoi(argv[6]);

  int rc, threadCount;
  int countArray[totalNumberOfThreads];

  int totalNumberOfThreads = numberOfProducers + numberOfConsumers;
  threads = malloc(totalNumberOfThreads * sizeof(pthread_t));

  cb_init(circBuffer,queueCapacity,);

  for (threadCount = 0; threadCount < numberOfProducers; threadCount++){
    printf("Producer thread %d \n", threadCount);

    rc = pthread_create(&threads, NULL, producer, NULL);
  }


  if (threads == NULL){
    printf("Mein Gott. Ze memory won't suffice");
    return -1;
  }




}
