#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include "linkedlist.h"
#include "coursework.h"

// the number of items have been produced / consumed
int iItemsProduced = 0;
int iItemsConsumed = 0;

// flag for consumer threads end
int consumerExit = 0;

sem_t mutex;
sem_t processJobMutex; // mutex for processJob()
sem_t empty;
sem_t full;

// array of queues stroing processes with different priority
struct element *processQueueFront[MAX_PRIORITY];
struct element *processQueueRear[MAX_PRIORITY];

// average respond time and average turn around time
double dAverageResponseTime = 0, dAverageTurnAroundTime = 0;

// prints the output and calculates the average response time & average turn around time
struct process *processJob(int iConsumerId, struct process *pProcess, struct timeval oStartTime, struct timeval oEndTime);

// prints semaphore values for debugging
void printSemValue();

void *producer(void *id);

void *consumer(void *id);

int main()
{
	// initializes pointers to NULL
	for (int i = 0; i < MAX_PRIORITY; i++)
	{
		processQueueRear[i] = NULL;
		processQueueFront[i] = NULL;
	}

	// initial value of semaphores
	sem_init(&empty, 0, MAX_BUFFER_SIZE); // MAX_BUFFER_SIZE for initial value of empty semaphore
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
	sem_init(&processJobMutex, 0, 1);

	// array of the producer thread
	pthread_t producerArray[NUMBER_OF_PRODUCERS];
	// array of the producer's id
	int producerIds[NUMBER_OF_PRODUCERS];

	// array of consumer threads
	pthread_t consumerArray[NUMBER_OF_CONSUMERS];
	// array of consumers' id
	int consumerIds[NUMBER_OF_CONSUMERS];

	// creates the producer thread with a unique id
	for (int i = 0; i < NUMBER_OF_PRODUCERS; i++)
	{
		producerIds[i] = i;
		pthread_create(&producerArray[i], NULL, producer, &producerIds[i]);
	}

	// creates consumer threads with a unique id
	for (int i = 0; i < NUMBER_OF_CONSUMERS; i++)
	{
		consumerIds[i] = i;
		pthread_create(&consumerArray[i], NULL, consumer, &consumerIds[i]);
	}

	// waits for the producer thread to terminate
	for (int i = 0; i < NUMBER_OF_PRODUCERS; i++)
	{
		pthread_join(producerArray[i], NULL);
	}

	// waits for consumer threads to terminate
	for (int i = 0; i < NUMBER_OF_CONSUMERS; i++)
	{
		pthread_join(consumerArray[i], NULL);
	}

	// printSemValue();		// prints value in semaphores

	// prints avergae response time and average turn around time
	printf("Average response time = %.6f\n", dAverageResponseTime / NUMBER_OF_JOBS);
	printf("Average turn around time = %.6f\n", dAverageTurnAroundTime / NUMBER_OF_JOBS);

	return 0;
}

struct process *processJob(int iConsumerId, struct process *pProcess, struct timeval oStartTime, struct timeval oEndTime)
{
	int iResponseTime;
	int iTurnAroundTime;
	if (pProcess->iPreviousBurstTime == pProcess->iInitialBurstTime && pProcess->iRemainingBurstTime > 0)
	{
		iResponseTime = getDifferenceInMilliSeconds(pProcess->oTimeCreated, oStartTime);
		// mutex for time addup and printf IO
		sem_wait(&processJobMutex);
		dAverageResponseTime += iResponseTime;
		printf("Consumer %d, Process Id = %d (%s), Priority = %d, Previous Burst Time = %d, Remaining Burst Time = %d, Response Time = %d\n", iConsumerId, pProcess->iProcessId, pProcess->iPriority < MAX_PRIORITY / 2 ? "FCFS" : "RR", pProcess->iPriority, pProcess->iPreviousBurstTime, pProcess->iRemainingBurstTime, iResponseTime);
		sem_post(&processJobMutex);
		return pProcess;
	}
	else if (pProcess->iPreviousBurstTime == pProcess->iInitialBurstTime && pProcess->iRemainingBurstTime == 0)
	{
		iResponseTime = getDifferenceInMilliSeconds(pProcess->oTimeCreated, oStartTime);
		iTurnAroundTime = getDifferenceInMilliSeconds(pProcess->oTimeCreated, oEndTime);
		// mutex for time addup and printf IO
		sem_wait(&processJobMutex);
		dAverageResponseTime += iResponseTime;
		dAverageTurnAroundTime += iTurnAroundTime;
		printf("Consumer %d, Process Id = %d (%s), Priority = %d, Previous Burst Time = %d, Remaining Burst Time = %d, Response Time = %d, Turnaround Time = %d\n", iConsumerId, pProcess->iProcessId, pProcess->iPriority < MAX_PRIORITY / 2 ? "FCFS" : "RR", pProcess->iPriority, pProcess->iPreviousBurstTime, pProcess->iRemainingBurstTime, iResponseTime, iTurnAroundTime);
		sem_post(&processJobMutex);
		free(pProcess);
		return NULL;
	}
	else if (pProcess->iPreviousBurstTime != pProcess->iInitialBurstTime && pProcess->iRemainingBurstTime > 0)
	{
		// mutex for time addup and printf IO
		sem_wait(&processJobMutex);
		printf("Consumer %d, Process Id = %d (%s), Priority = %d, Previous Burst Time = %d, Remaining Burst Time = %d\n", iConsumerId, pProcess->iProcessId, pProcess->iPriority < MAX_PRIORITY / 2 ? "FCFS" : "RR", pProcess->iPriority, pProcess->iPreviousBurstTime, pProcess->iRemainingBurstTime);
		sem_post(&processJobMutex);
		return pProcess;
	}
	else if (pProcess->iPreviousBurstTime != pProcess->iInitialBurstTime && pProcess->iRemainingBurstTime == 0)
	{
		iTurnAroundTime = getDifferenceInMilliSeconds(pProcess->oTimeCreated, oEndTime);
		// mutex for time addup and printf IO
		sem_wait(&processJobMutex);
		dAverageTurnAroundTime += iTurnAroundTime;
		printf("Consumer %d, Process Id = %d (%s), Priority = %d, Previous Burst Time = %d, Remaining Burst Time = %d, Turnaround Time = %d\n", iConsumerId, pProcess->iProcessId, pProcess->iPriority < MAX_PRIORITY / 2 ? "FCFS" : "RR", pProcess->iPriority, pProcess->iPreviousBurstTime, pProcess->iRemainingBurstTime, iTurnAroundTime);
		sem_post(&processJobMutex);
		free(pProcess);
		return NULL;
	}
}

// prints semaphore values for debugging
void printSemValue()
{
	int mutexValue, emptyValue, fullValue;
	sem_getvalue(&mutex, &mutexValue);
	sem_getvalue(&empty, &emptyValue);
	sem_getvalue(&full, &fullValue);
	printf("mutex = %d, empty = %d, full = %d\n", mutexValue, emptyValue, fullValue);
	return;
}

void *producer(void *id)
{
	int producerId = *(int *)id;
	struct process *pTemp = NULL;
	while (iItemsProduced < NUMBER_OF_JOBS)
	{
		// generates process before entering the critical section
		pTemp = generateProcess();	

		sem_wait(&empty);
		sem_wait(&mutex);

		// puts the generated process in corresponding queue
		addLast(pTemp, &processQueueFront[pTemp->iPriority], &processQueueRear[pTemp->iPriority]);
		iItemsProduced++;

		sem_post(&mutex);
		sem_post(&full);
	}
	return NULL;
}

void *consumer(void *id)
{
	struct timeval oStartTime, oEndTime;
	int consumerId = *(int *)id;
	struct process *pTemp = NULL;
	while (1)
	{
		sem_wait(&full);

		// checks whether to terminate all consumer threads
		if (consumerExit)
		{
			return NULL;
		}

		sem_wait(&mutex);

		// gets the first process in queues available with highest priority
		for (int i = 0; i < MAX_PRIORITY; i++)
		{
			if (processQueueFront[i] != NULL)
			{ 
				pTemp = removeFirst(&processQueueFront[i], &processQueueRear[i]);
				break;
			}
		}

		sem_post(&mutex);

		// runs the process
		runJob(pTemp, &oStartTime, &oEndTime);
		// prints the output and calculates the average response time & average turn around time
		pTemp = processJob(consumerId, pTemp, oStartTime, oEndTime);

		sem_wait(&mutex);

		if (pTemp != NULL)
		{
			// puts the process back at the end of corresponding queue if it has not finished
			addLast(pTemp, &processQueueFront[pTemp->iPriority], &processQueueRear[pTemp->iPriority]);
		}
		else
		{
			// increments the counter if the process has finished
			iItemsConsumed++;
		}

		// checks whether all process have finished
		if ((iItemsConsumed == iItemsProduced) && (iItemsProduced == MAX_NUMBER_OF_JOBS))
		{
			// sets exit flag to 1
			consumerExit = 1;
			// wakes up blocked consumers to terminate
			for (int i = 0; i < NUMBER_OF_CONSUMERS; i++) 
			{
				sem_post(&full);
			}
		}
		sem_post(&mutex);

		if (pTemp == NULL)
		{
			// increments the semaphore empty because one space is freed from the buffer
			sem_post(&empty);
		}
		else
		{
			// increments the semaphore full because the process is added to the buffer again
			sem_post(&full);
		}
	}

	return NULL;
}