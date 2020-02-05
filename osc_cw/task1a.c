#include <stdlib.h>
#include "coursework.h"
#include "linkedlist.h"

/* The pointer to the front and rear of the queue of process */
struct element *processQueueFront = NULL;
struct element *processQueueRear = NULL;

/**
 * generate NUMBER_OF_JOBS processes and store them in a linked list
 */
void createProcesses(struct element **queueFront, struct element **queueRear)
{
	for (int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		addLast(generateProcess(), queueFront, queueRear);
	}
}

int main(int argc, char *argv[])
{

	// initial process list
	createProcesses(&processQueueFront, &processQueueRear);

	struct process *oTemp = NULL;
	struct timeval oStartTime, oEndTime;

	long int totalResponseTime = 0;
	long int totalTurnaroundTime = 0;

	// loop until all processes run over
	while (processQueueFront != NULL)
	{
		// get the first process in the queue
		oTemp = (struct process *)removeFirst(&processQueueFront, &processQueueRear);
		
		// run process in non-preemptive method
		runNonPreemptiveJob(oTemp, &oStartTime, &oEndTime);

		printf("Process Id = %d, ", oTemp->iProcessId);
		printf("Previous Burst Time = %d, ", oTemp->iPreviousBurstTime);
		printf("New Burst Time = %d, ", oTemp->iRemainingBurstTime);

		// calculate respond time & turn aroudn time
		long int singleResponseTime = getDifferenceInMilliSeconds(oTemp->oTimeCreated, oStartTime);
		long int singleTurnaroundTime = getDifferenceInMilliSeconds(oTemp->oTimeCreated, oEndTime);

		printf("Response Time = %ld, ", singleResponseTime);
		printf("Turn Around Time = %ld\n", singleTurnaroundTime);

		// add up time for average
		totalResponseTime += singleResponseTime;
		totalTurnaroundTime += singleTurnaroundTime;

		// release memory for oTemp
		free(oTemp);
	}

	printf("Average response time = %.6f\n", (double)totalResponseTime / (double)NUMBER_OF_JOBS);
	printf("Average turn around time = %.6f\n", (double)totalTurnaroundTime / (double)NUMBER_OF_JOBS);

	return 0;
}