#include <stdlib.h>
#include "coursework.h"
#include "linkedlist.h"

/* The pointer to the front and rear of the queue of process */
struct element *processQueueFront = NULL;
struct element *processQueueRear = NULL;

/**
 * generate NUMBER_OF_JOBS processes and store them in a linked list
 * with initialized info of process printed
 */
void createProcesses(struct element **queueFront, struct element **queueRear)
{
	printf("PROCESS LIST:\n");
	for (int i = 0; i < NUMBER_OF_JOBS; i++)
	{
		struct process *newProcess = generateProcess();
		addLast(newProcess, queueFront, queueRear);
		printf("	 ");
		printf("Process Id = %d, ", newProcess->iProcessId);
		printf("Priority = %d, ", newProcess->iPriority);
		printf("Initial Burst Time = %d", newProcess->iInitialBurstTime);
		printf("Remaining Burst Time = %d\n", newProcess->iRemainingBurstTime);
	}
	printf("END\n");
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

		// run process in preemptive method
		runPreemptiveJob(oTemp, &oStartTime, &oEndTime);

		printf("\nProcess Id = %d, ", oTemp->iProcessId);
		printf("Priority = %d, ", oTemp->iPriority);
		printf("Previous Burst Time = %d, ", oTemp->iPreviousBurstTime);
		printf("Remaining Burst Time = %d", oTemp->iRemainingBurstTime);

		// print respond time for first run
		if (oTemp->iInitialBurstTime == TIME_SLICE + oTemp->iRemainingBurstTime)
		{
			long int singleResponseTime = getDifferenceInMilliSeconds(oTemp->oTimeCreated, oStartTime);
			printf(", Response Time = %ld", singleResponseTime);
			totalResponseTime += singleResponseTime;
		}

		// check remain time
		if (oTemp->iRemainingBurstTime == 0)
		{
			// print turn around time for last run
			long int singleTurnaroundTime = getDifferenceInMilliSeconds(oTemp->oTimeCreated, oEndTime);
			printf(", Turnaround Time = %ld", singleTurnaroundTime);
			totalTurnaroundTime += singleTurnaroundTime;
			free(oTemp);
		}
		else
		{
			// add current process to the tail of list
			addLast(oTemp, &processQueueFront, &processQueueRear);
		}
	}

	printf("\nAverage response time = %.6f\n", (double)totalResponseTime / (double)NUMBER_OF_JOBS);
	printf("Average turn around time = %.6f\n", (double)totalTurnaroundTime / (double)NUMBER_OF_JOBS);

	return 0;
}