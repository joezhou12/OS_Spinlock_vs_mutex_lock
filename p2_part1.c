/*
 * Perform producer-consumer program
 * due 12/03/2019
 * Chunhei Yuen & Yichun Zhou
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Global variables
pthread_mutex_t mutex;
sem_t full, empty;
int number = 0;
int count, in, out;
int buffer[0];

// Function prototypes
void *consumer(void *param);
void *producer(void *param);

struct v {
    int TID;
	  int BUFFER_SIZE;
    int UPPER_LIMIT;
    int NUM_PRODUCERS;
    int NUM_CONSUMERS;
};

int main(int argc, char **argv){
  if (argc != 5){
    printf("ERROR: Please provide BUFFER_SIZE, num_producer, num_consumer, upper_limit.\n");
    exit(1);
  }

  // Retrieve command line arguments
  //const long int stime = strtol(argv[1], NULL, 0);
  const long int buffer_size = strtol(argv[1], NULL, 0);
  const long int num_producer = strtol(argv[2], NULL, 0);
  const long int num_consumer = strtol(argv[3], NULL, 0);
  const long int upper_limit = strtol(argv[4], NULL, 0);

  // Initialize

  struct v *data = (struct v *) malloc (sizeof(struct v));
  data->TID = 0;
  data->BUFFER_SIZE = buffer_size;
  data->UPPER_LIMIT = upper_limit;
  data->NUM_PRODUCERS = num_producer;
  data->NUM_CONSUMERS = num_consumer;

  buffer[buffer_size];
  int i; // for forloop
  pthread_mutex_init(&mutex, NULL);

  //init elapse_time
  double elapse_time;
  clock_t start, end;
  


  sem_init(&empty, 0, buffer_size); // All of buffer is empty
  sem_init(&full, 0, 0);
  count = in = out = 0;

  // Create the producer and consumer threads
  pthread_t tid_producer[num_producer];
  pthread_t tid_consumer[num_consumer];
  pthread_attr_t attr_producer[num_producer];
  pthread_attr_t attr_consumer[num_consumer];

  start = clock();//start the clock
  for(i = 0; i < num_producer; i++) {
    pthread_attr_init(&attr_producer[i]);
    data->TID = tid_producer[i];
    pthread_create(&tid_producer[i], &attr_producer, producer, data);
  }
  for(i = 0; i < num_consumer; i++) {
    pthread_attr_init(&attr_consumer[i]);
    pthread_create(&tid_consumer[i], &attr_consumer, consumer, data);
  }
  	//join all threads (wait to finish)
	for (i = 0; i < num_producer; i++) {
		pthread_join(tid_producer[i], NULL);
	}
	for (i = 0; i < num_consumer; i++) {
		pthread_join(tid_consumer[i], NULL);
	}
  end = clock();//end the clock
  elapse_time = ((double)(end - start))/ CLOCKS_PER_SEC;//caculate elapse time
  printf("elapse_time %f\n", elapse_time);//print elapse time
  return 0;
}

void *producer(void *param) {
  struct v *data;
  data = (struct v*) param;
	int isMax = 0;
	while(!isMax) {
		sem_wait(&empty);
		sem_wait(&mutex);
		// critical section
		if (number <= data->UPPER_LIMIT) {
			buffer[in] = number;
			number++;

			in = (in + 1) % data->BUFFER_SIZE;
		} else {
			isMax = 1;
		}
		sem_post(&mutex);
		sem_post(&full);
	}
}

void *consumer(void *param) {
  int j;
  struct v *data;
  data = (struct v*) param;
	int isMax = 0;

	while(!isMax) {
		sem_wait(&full);
		sem_wait(&mutex);
		// critical section
    //for(j=0; j <1e6; j++);
    int tid = pthread_self();
		int o = buffer[out];
		if (o < data->UPPER_LIMIT) {
			out = (out + 1) % data->BUFFER_SIZE;
		} else {
			isMax = 1;
		}
    
		sem_post(&mutex);
		sem_post(&empty);

		// consume the item in nextConsumed
		printf(" %i\t %i \n", o, tid);
    
	}
}