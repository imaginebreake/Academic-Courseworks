#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// the number of elements should be produced
#define NUMBER_OF_JOBS 1000

// the number of items has been produced / consumed
int produced = 0;
int consumed = 0;

// the number of elements in the buffer
int items = 0;

// binary semaphores
sem_t mutex, full;

// printMode: 1 for producer, 0 for consumer
void printJobInfo(int printMode)
{
	printf(printMode ? "Producer, " : "Consumer, ");
	printf("Produced = %d, Consumed = %d: ", produced, consumed);
	for (int i = 0; i < items; i++)
		printf("*");
	printf("\n");
	return;
}

void *producer(void *arg)
{
	while (produced < NUMBER_OF_JOBS)
	{
		sem_wait(&mutex);
		produced++;
		items++;
		printJobInfo(1);

		if (items == 1)
		{
			sem_post(&full);
		}
		
		sem_post(&mutex);
	}
	return NULL;
}

void *consumer(void *arg)
{
	sem_wait(&full);
	int temp;
	while (consumed < NUMBER_OF_JOBS)
	{
		sem_wait(&mutex);
		items--;
		temp = items; // store current number of items to temp
		consumed++;
		printJobInfo(0);
		sem_post(&mutex);

		// use temp to prevent deadlocks
		if (consumed < NUMBER_OF_JOBS && temp == 0)
		{
			sem_wait(&full);
		}
	}
	return NULL;
}

void printSemValue()
{
	int mutexValue, fullValue;
	sem_getvalue(&mutex, &mutexValue);
	sem_getvalue(&full, &fullValue);
	printf("mutex = %d, full = %d\n", mutexValue, fullValue);
}

int main()
{
	// initial value in semaphores
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);

	// run threads
	pthread_t p, c;
	pthread_create(&p, NULL, producer, NULL);
	pthread_create(&c, NULL, consumer, NULL);
	pthread_join(p, NULL);
	pthread_join(c, NULL);

	// print value in semaphores
	printSemValue();
	return 0;
}