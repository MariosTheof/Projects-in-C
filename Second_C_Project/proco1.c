/*First Attempt at a POSIX program in C*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "proco1.h"

//global variables
int seed,rc,numberOfRandomNumbers;
int totalNumbers;						//συνολικό πλήθος αριθμών που θα παραχθούν
int endsignal = 0;

pthread_t* producersThreads;				//πίνακας με τα thread των παραγωγών
pthread_t* consumersThreads;				//πίνακας με τα thread των καταναλωτών

  

pthread_mutex_t mutex;
pthread_mutex_t consumersMutex;
pthread_mutex_t producersMutex;
pthread_mutex_t mutexToPrintThread;	


pthread_cond_t bufferFull;			//condition variable για το πότε δεν είναι γεμάτος ο buffer
pthread_cond_t bufferEmpty;			//condition variable για το πότε δεν είναι άδειος ο buffer

circular_buffer* cb;
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
      printf("Access violation. Buffer is empty\n");
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
}


int cb_isEmpty(circular_buffer* cb){
	if(cb->count == 0){
		return 1;
	}else{
		return 0;
	}
}

int cb_isFull(circular_buffer* cb){
	if(cb->count == cb->capacity){
		return 1;
	}else{
		return 0;
	}
}



void *producer(void *producerId) {
  int *prodId;
  int r,i;

  prodId = (int *)producerId;

 // printf("Hello from *producer(thread) %d. \n Let's generate some numbers.\n",*prodId);

  for(i = 0; i < numberOfRandomNumbers; i++){

    /*Locking mutex*/
    rc = pthread_mutex_lock(&mutex);
    if (rc != 0){
      printf("ERROR: return code from pthread_mutex_lock*() is %d \n",rc);
      pthread_exit(&rc);
    }
		

		//ελέγχει αν ο buffer είναι γεμάτος, αν είναι περιμένει να ελευθερωθεί θέση
		while(cb_isFull(cb) == 1){
			rc = pthread_cond_wait(&bufferFull, &mutex);		//περιμένει μέχρι να ελευθερωθεί θέση στον πίνακα
			if (rc != 0) {	
				printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);
				pthread_exit(&rc);
			}
		} 

		r = rand_r(&seed) % 256;

    cb_push_back(cb, &r); // places the random number insider the buffer


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
	int r; 
  consId = (int *)consumerId;
 // printf("Hello from *consumer(thread) %d \n",*consId);

	
	//while(totalNumbers > 0  && endsignal == 0){
 	int i;
	for(i = 0; i < numberOfRandomNumbers; i++){

  /*Locking mutex*/
  rc = pthread_mutex_lock(&mutex);
  if (rc != 0){
    printf("ERROR: retrn code from pthread_mutex_lock*() is %d \n",rc);
    pthread_exit(&rc);
  }

	//αν ο buffer είναι άδειος, περιμένει να εισαχθεί αριθμός
		while(cb_isEmpty(cb) == 1){
		
			if(endsignal == 1){	//αν έχει σταλεί το σήμα πως τελείωσαν οι αριθμοί, σπάει
				break;
			}else{
				rc = pthread_cond_wait(&bufferEmpty, &mutex);		//περιμένει μέχρι να λάβει το σήμα, είτε επειδή έχει μπει αριθμός στον 
																		//buffer είτε επειδή τελείωσαν οι αριθμοί προς εξαγωγή
				
				if (rc != 0) {	
					printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);
					pthread_exit(&rc);
				}
			}
		//}
	}

  int *tmp_read = (int*)malloc(4);
	
  cb_pop_front(cb, &r);
	totalNumbers--;				//μειώνει τους αριθμούς που απομένουν για εξαγωγή
		if(totalNumbers == 0 && endsignal == 0){	//αν οι αριθμοί τελείωσαν στέλνει σήμα στα thread
			endsignal = 1;
			pthread_cond_broadcast(&bufferEmpty);
		}

  
	
  free(tmp_read);

  /*Unlocking mutex*/
  rc = pthread_mutex_unlock(&mutex);
  if (rc != 0){
    printf("ERROR: retrn code from pthread_mutex_lock*() is %d \n",rc);
    pthread_exit(&rc);
  }
	fprintf(fc2,"Consumer %d : %d \n",*consId, r);
	}
  pthread_exit(consId);
}





int main(int argc, char *argv[]){
 


  if (argc != 6){
    printf("Incorrect number of arguments.\n");
    exit(-1);
  }

  /*converts argv elements to integers */
  int numberOfProducers = atoi(argv[1]);
  int numberOfConsumers = atoi(argv[2]);
  numberOfRandomNumbers = atoi(argv[3]);
  int queueCapacity = atoi(argv[4]);
  seed = atoi(argv[5]);

  int totalNumberOfThreads = numberOfProducers + numberOfConsumers;
  //threads = malloc(totalNumberOfThreads * sizeof(pthread_t));

	producersThreads = malloc(sizeof(pthread_t) * numberOfProducers);
	consumersThreads = malloc(sizeof(pthread_t) * numberOfConsumers);

	totalNumbers = numberOfRandomNumbers * numberOfProducers;

  int countArray[totalNumberOfThreads];


	int rc;

	//buffer
	cb = (circular_buffer*) malloc(sizeof(struct circular_buffer));
	cb_init(cb, queueCapacity, sizeof(int));	


	
 	/* Opening Producer & Consumer files */
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


  


	/*create buffer_mutex */
	rc = pthread_mutex_init(&mutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	/*create producersMutex */
	rc = pthread_mutex_init(&producersMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	//create consumersMutex
	rc = pthread_mutex_init(&consumersMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}




  /* Create producer threads */
	int i;
  for (i = 0; i < numberOfProducers; i++){
    printf("Producer thread #%d created \n", i + 1);

    countArray[i] = i + 1;

    rc = pthread_create(&producersThreads[i], NULL, producer, &countArray[i]);
    if (rc != 0 ){
      printf("ERROR: return code from pthread_create() is %d \n",rc);
      exit(-1);
    }
  }

  /* Create consumer threads */
  for (i = 0;i < numberOfConsumers; i++){
    printf("Consumer thread #%d created \n",i + 1);

    countArray[i] = i + 1;

    rc = pthread_create(&consumersThreads[i],NULL, consumer,&countArray[i]);
    if (rc != 0 ){
      printf("ERROR: return code from pthread_create() is %d \n",rc);
      exit(-1);
    }
  }
  
  if (consumersThreads == NULL || producersThreads == NULL){
    printf("Mein Gott. Ze memory won't suffice");
    return -1;
  }

  
  void *status;

	/*Destroy/Join the threads */

	/*Join producers*/
	for(i = 0; i < numberOfProducers; i++){
		rc = pthread_join(producersThreads[i], &status);
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d, i = %d\n", rc, i);
			exit(-1);		
		}
	}
	/*Join consumers*/
	for(i = 0; i < numberOfConsumers; i++){
		rc = pthread_join(consumersThreads[i], &status);
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d, i = %d\n", rc, i);
			exit(-1);		
		}
	}


  /*Destroy mutex*/
  rc = pthread_mutex_destroy(&mutex);
  if (rc != 0){
    printf("ERROR: return code from pthread_mutex_destroy() is %d \n", rc);
    exit(-1);
  }

	/*Destroy producersMutex*/
  rc = pthread_mutex_destroy(&producersMutex);
  if (rc != 0){
    printf("ERROR: return code from pthread_mutex_destroy() is %d \n", rc);
    exit(-1);
  }

	/*Destroy consumersMutex*/
  rc = pthread_mutex_destroy(&consumersMutex);
  if (rc != 0){
    printf("ERROR: return code from pthread_mutex_destroy() is %d \n", rc);
    exit(-1);
  }

  fclose(fc);
  fclose(fc2);


  free(consumersThreads);
	free(producersThreads);
  return 1;
}
