#define _POSIX_C_SOURCE	199309L // for declaration of function 'nanosleep'

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include "linkedlist.h"
#include "coursework.h"

const int NUMBER_OF_BOOSTERS = 1; // number of boosters
const long PRODUCE_DELAY_TIME = MAX_BURST_TIME / 3; // in milli second

// the number of items have been produced / consumed
int iItemsProduced = 0;
int iItemsConsumed = 0;

// flag for consumer threads end
int consumerExit = 0;

sem_t mutex;
sem_t processJobMutex; // mutex for processJob()
sem_t empty;
sem_t full;

// current running process array for corresponding consumer
struct process *currentProcessList[NUMBER_OF_CONSUMERS];
// new process array from preempting for corresponding consumer
struct process *preemptProcessList[NUMBER_OF_CONSUMERS];

// array of queues stroing processes with different priority
struct element *processQueueFront[MAX_PRIORITY];
struct element *processQueueRear[MAX_PRIORITY];

// average respond time and average turn around time
double dAverageResponseTime = 0, dAverageTurnAroundTime = 0;

// prints semaphore values for debugging
void printSemValue();

// prints the output and calculates the average response time & average turn around time
struct process *processJob(int iConsumerId, struct process *pProcess, struct timeval oStartTime, struct timeval oEndTime);

void *producer(void *id);

// prepare generated process for interrupt or further extension
struct process *prepareProcess(struct process *pTemp);

// Function returning 1 if the newly generated process preempts successfully, 0 otherwise
int tryPreempt(struct process *pNew);

void *consumer(void *id);

// searches available process in queues to consume
struct process *searchAvailableProcess();

// runs a FCFS job
struct process *runFCFSJob(int consumerId, struct process *pFCFS);

// runs a RR job
struct process *runRRJob(int consumerId, struct process *pRR);

// booster for RR processes
void *booster(void *p);

int main()
{
	// initializes pointers to NULL
	for (int i = 0; i < MAX_PRIORITY; i++)
	{
		processQueueRear[i] = NULL;
		processQueueFront[i] = NULL;
	}

	// initializes pointers to NULL
	for (int i = 0; i < NUMBER_OF_CONSUMERS; i++){
		preemptProcessList[i] = NULL;
		currentProcessList[i] = NULL;
	}	
	
	// initializes value of semaphores
	sem_init(&empty, 0, MAX_BUFFER_SIZE); // MAX_BUFFER_SIZE for initial value of empty semaphore
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
	sem_init(&processJobMutex, 0, 1);

	// array of the producer thread
	pthread_t producerArr[NUMBER_OF_PRODUCERS];
	// array of the producer's id
	int producerIds[NUMBER_OF_PRODUCERS];

	// array of consumer threads
	pthread_t consumerArr[NUMBER_OF_CONSUMERS];
	// array of consumers' id
	int consumer_id[NUMBER_OF_CONSUMERS];

	// the booster thread for RR
	pthread_t b;

	// creates the producer thread with a unique id
	for (int i = 0; i < NUMBER_OF_PRODUCERS; i++)
	{
		producerIds[i] = i;
		pthread_create(&producerArr[i], NULL, producer, &producerIds[i]);
	}

	// creates consumer threads with a unique id
	for (int i = 0; i < NUMBER_OF_CONSUMERS; i++)
	{
		consumer_id[i] = i;
		pthread_create(&consumerArr[i], NULL, consumer, &consumer_id[i]);
	}

	// creates the booster thread
	pthread_create(&b, NULL, booster, NULL);

	// waits for the producer thread to terminate
	for (int i = 0; i < NUMBER_OF_PRODUCERS; i++)
	{
		pthread_join(producerArr[i], NULL);
	}

	// waits for the consumer thread to terminate
	for (int i = 0; i < NUMBER_OF_CONSUMERS; i++)
	{
		pthread_join(consumerArr[i], NULL);
	}

	// waits for the booster thread to terminate
	pthread_join(b, NULL);

	// printSemValue(); 	// print value in semaphores

	// prints average response time and average turn around time
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
		// prevent invalid memory address access during free()
		sem_wait(&mutex);
		free(pProcess);
		currentProcessList[iConsumerId] = NULL;
		sem_post(&mutex);
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
		// prevent invalid memory address access during free()
		sem_wait(&mutex);
		free(pProcess);
		currentProcessList[iConsumerId] = NULL;
		sem_post(&mutex);
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

/* 
 * Checks whether the newly generated process could preempt other process.
 * Returns 1 if the newly generated process preempts successfully, 0 otherwise.
 */
int tryPreempt(struct process *pNew)
{
	/* remember 0 is high and 15 is low */
	int highestPriorityValue = MAX_PRIORITY / 2; // lowest priority for FCFS process + 1
	int highestPriorityConsumer = -1;
	int lowestPriorityValue = -1; // highest priority for FCFS process - 1
	int lowestPriorityConsumer = -1;

	// traverses consumer threads to get highest and lowest priority value in running processes
	for (int i = 0; i < NUMBER_OF_CONSUMERS; i++)
	{
		if (currentProcessList[i] == NULL || preemptProcessList[i] != NULL)
		{
			/* skips this consumer if it's not running or it has been preempted */
			continue;
		}
		else if (currentProcessList[i]->iPriority >= MAX_PRIORITY / 2) {
			/* skips this consumer if current running process is not FCFS */
			continue;
		}
		else {
			int thisPriorityValue = currentProcessList[i]->iPriority;
			if (thisPriorityValue < highestPriorityValue)
			{
				// updates highest priority
				highestPriorityValue = thisPriorityValue;
				highestPriorityConsumer = i;
			}
			if (thisPriorityValue > lowestPriorityValue)
			{
				// updates lowest priority
				lowestPriorityValue = thisPriorityValue;
				lowestPriorityConsumer = i;
			}
		}
	}

	if ((pNew->iPriority < highestPriorityValue) && (highestPriorityValue <= lowestPriorityValue))
	{
		/* if the new process has higher priority than all running processes */

		// finds the running process with lowest priority
		struct process *pOld = NULL;
		pOld = currentProcessList[lowestPriorityConsumer];
		preemptProcessList[lowestPriorityConsumer] = pNew;

		// interrupts it
		preemptJob(pOld);
		printf("Pre-empted job: Pre-empted Precess ID = %d, Pre-empted Priority %d, New Process ID %d, New Priority %d\n", pOld->iProcessId, pOld->iPriority, pNew->iProcessId, pNew->iPriority);
		
		return 1;
	}
	else
	{
		/* the new process does not interrupt other processes */
		return 0;
	}
}

/*
 * Function pre-processing the newly generated process.
 */
struct process *prepareProcess(struct process *pTemp)
{
	if (pTemp->iPriority < MAX_PRIORITY / 2)
	{
		if (tryPreempt(pTemp) == 1)
		{
			return NULL;
		}
	}
	return pTemp;
}

void *producer(void *id)
{
	int producerId = *(int *)id;
	while (iItemsProduced < NUMBER_OF_JOBS)
	{
		// generates process before entering the critical section
		struct process *pTemp = generateProcess();

		sem_wait(&empty);
		sem_wait(&mutex);

		pTemp = prepareProcess(pTemp); 
		/* pre-process the process before adding it to the queue:
		 * - pTemp will be NULL if it preempts other job
		 * - otherwise pTemp will not change
		 */
		if (pTemp != NULL)
		{
			addLast(pTemp, &processQueueFront[pTemp->iPriority], &processQueueRear[pTemp->iPriority]);
		}
		iItemsProduced++;
		
		sem_post(&mutex);
		sem_post(&full);

		// sleep for a while to simulate real running environment
		nanosleep((const struct timespec[]){{0, (PRODUCE_DELAY_TIME * 1000000)}}, NULL);
	}
	return NULL;
}

struct process *processJobInterrupt(int consumerId, struct process *pOld, struct process *pNew)
{
	if (pNew != NULL)
	{
		/* special case: preempted process is already finished while being preempted */
		if (pOld == NULL)
		{
			sem_wait(&full); // if we got interrupt, there will be new one buffer for this consumer
			sem_wait(&mutex);
			iItemsConsumed++; // one process finished
			sem_post(&mutex);
			sem_post(&empty); // one space is freed from the buffer
		}
		else
		{
			sem_wait(&full); // if we got interrupt, there will be new one buffer for this consumer
			sem_wait(&mutex);
			// puts the preempted process back in the queue
			addLast(pOld, &processQueueFront[pOld->iPriority], &processQueueRear[pOld->iPriority]);
			sem_post(&mutex);
			sem_post(&full); // increments the semaphore full because the process is added to the buffer again
		}
	}
	else if (pNew == NULL)
	{
		printf("something wrong for new process empty\n");
		exit(0);
	}
	sem_wait(&mutex);
	currentProcessList[consumerId] = pNew;	// sets current process to new process
	preemptProcessList[consumerId] = NULL; 	// resets preempt process list for further preemption
	sem_post(&mutex);
	return pNew;
}

struct process *runFCFSJob(int consumerId, struct process *pFCFS)
{
	struct process *pTemp = pFCFS;
	struct process *pPreempt = NULL;
	struct timeval oStartTime, oEndTime;
	int isInterrupted = 0;

	// not exiting until finished for a FCFS job
	while (pTemp != NULL)
	{
		// runs current process
		runJob(pTemp, &oStartTime, &oEndTime);

		// checks if current process is interrupted
		sem_wait(&mutex);
		pPreempt = preemptProcessList[consumerId];
		isInterrupted = (pPreempt != NULL);
		sem_post(&mutex);

		// if current process has finished, pTemp will be set to NULL by processJob()
		pTemp = processJob(consumerId, pTemp, oStartTime, oEndTime);

		if (isInterrupted == 1)
		{
			// if current process need to be interrupted, 
			// send interrupted process to handler and get new process
			pTemp = processJobInterrupt(consumerId, pTemp, pPreempt);
		}
	}

	sem_wait(&mutex);
	iItemsConsumed++; // increments the counter
	sem_post(&mutex);

	return pTemp;
};

struct process *runRRJob(int consumerId, struct process *pRR)
{
	struct process *pTemp = pRR;
	struct timeval oStartTime, oEndTime;

	runJob(pTemp, &oStartTime, &oEndTime);
	pTemp = processJob(consumerId, pTemp, oStartTime, oEndTime);

	sem_wait(&mutex);
	currentProcessList[consumerId] = NULL;
	if (pTemp != NULL)
	{
		addLast(pTemp, &processQueueFront[pTemp->iPriority], &processQueueRear[pTemp->iPriority]);
	}
	else
	{
		iItemsConsumed++;
	}
	sem_post(&mutex);

	return pTemp;
};

/* Function returning the first process in queue available with highest priority. */
struct process *searchAvailableProcess()
{
	for (int i = 0; i < MAX_PRIORITY; i++)
	{
		if (processQueueFront[i] != NULL)
		{
			return removeFirst(&processQueueFront[i], &processQueueRear[i]);
		}
	}
	return NULL;
}

void *consumer(void *id)
{
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
		currentProcessList[consumerId] = pTemp = searchAvailableProcess(consumerId);
		sem_post(&mutex);

		/* 
		 * when a new preempting process appears, it do post a full for consumers,
		 * but there can be a case that: there is no avaiable process in queues at sometine,
		 * but one consumer is waken up for the full post for preemting process, but can't
		 * find any process for this wake up, because this full is for processJobInterrupt()
		 * so here we just release the full semapore for processJobInterrupt() to use it.
		 */
		if (pTemp == NULL)
		{
			sem_post(&full);
			continue;
		}

		// runs the process
		if (pTemp->iPriority < MAX_PRIORITY / 2)
		{
			pTemp = runFCFSJob(consumerId, pTemp);
		}
		else
		{
			pTemp = runRRJob(consumerId, pTemp);
		}

		sem_wait(&mutex);
		// checks whether all process have finished
		if ((iItemsConsumed == iItemsProduced) && (iItemsProduced == MAX_NUMBER_OF_JOBS))
		{
			// sets exit flag to 1
			consumerExit = 1;
			// wakes up all blocked consumers and boosters to terminate
			for (int i = 0; i < NUMBER_OF_CONSUMERS + NUMBER_OF_BOOSTERS; i++) sem_post(&full);
		}
		sem_post(&mutex);

		if (pTemp != NULL)
		{
			// increments the semaphore full because the process is added to the buffer again
			sem_post(&full);
		}
		else
		{
			// increments the semaphore empty because one space is freed from the buffer
			sem_post(&empty);
		}
	}
	return NULL;
}

void *booster(void *p)
{
	struct timeval currentTime;
	int currentPriority = MAX_PRIORITY - 1;	// from lowest priority
	struct process *pTemp = NULL;
	while (1)
	{
		pTemp = NULL;

		// checks whether to terminate all consumer threads and booster thread - nothing to boost
		sem_wait(&full);
		if (consumerExit)
		{
			return NULL;
		}

		sem_wait(&mutex);
		// gets the first process when there is process in the queue with current priority
		if (processQueueFront[currentPriority] != NULL && processQueueRear[currentPriority] != NULL)
		{
			pTemp = removeFirst(&processQueueFront[currentPriority], &processQueueRear[currentPriority]);
		}
		sem_post(&mutex);

		if (pTemp != NULL)
		{
			gettimeofday(&currentTime, NULL);

			// gets last access time from oTimeCreated or oMostRecentTime
			// according to whether responeded before
			struct timeval oLastAccessTime;
			if (pTemp->iInitialBurstTime == pTemp->iRemainingBurstTime) {
				oLastAccessTime = pTemp->oTimeCreated;
			}
			else
			{
				oLastAccessTime = pTemp->oMostRecentTime;
			}

			// checks boost interval
			if (getDifferenceInMilliSeconds(oLastAccessTime, currentTime) > BOOST_INTERVAL)
			{
				// changes the priority of this process
				pTemp->iPriority = MAX_PRIORITY / 2;
				printf("Boost priority: Process ID = %d, Proiority = %d, New Priority = %d\n", pTemp->iPriority, currentPriority, MAX_PRIORITY / 2);
			}

			// puts the process back in the corresponding queue
			sem_wait(&mutex);
			addFirst(pTemp, &processQueueFront[pTemp->iPriority], &processQueueRear[pTemp->iPriority]);
			sem_post(&mutex);
		}

		sem_post(&full);
		if (currentPriority != (MAX_PRIORITY / 2 + 1))
		{
			currentPriority--; // minus 1 in each loop
		}
		else
		{
			// resets the priority to lowest number
			// note: no need to check process with MAX_PRIORITY / 2
			currentPriority = MAX_PRIORITY - 1;
		}
	}
	return NULL;
}
