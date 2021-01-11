/*
 * smphr.c

 * Semaphores implemented using Peterson algorithm
 *  Created on: Jan 20, 2020
 *      Author: ivan
 */

/* Implementation Peterson algorithm for 2 processes */
/* Binary sempahore */


#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "smphr.h"

#define SHARED_MEM_BASE_ADDR	0x3ED80000
#define SHERED_MEM_BASE_SIGNAL	0x3ED80800

/*
 * Binary semaphore init
 * Set ready array to zero and turn array to 1
 */
void semaphore_init (int sem_id)
{
	smphr_t *sem;
	sem = (smphr_t*) SHARED_MEM_BASE_ADDR;

	(sem + sem_id)->sem_id = sem_id;

	for(int i = 0; i<N; i++)
	{
		(sem + sem_id)->ready[i] = 0;
	}

	for(int j = 0; j<(N-1); j++)
	{
		(sem + sem_id)->turn[j]	= 1;
	}
}

/*
 * Binary semaphore take in order to start critical section
 * Set ready[i] to the number of current stage
 * Turn shows which process has previously entered the stage
 */
void semaphore_take(int sem_id, int core_number)
{
	smphr_t *sem;
	sem = (smphr_t*) SHARED_MEM_BASE_ADDR;
	int stage_clear;

	for(int j = 1; j<N; j++)
		{
			(sem + sem_id)->ready[core_number] = j;
			(sem + sem_id)->turn[j] = core_number;

			do
			{
				stage_clear = 1;

				for(int k = 0; k<N; k++)
				{
					if (k == core_number)
					{
						continue;
					}

					if((sem + sem_id)->ready[k] >= j)
					{
						stage_clear = 0;
						break;
					}
				}
			} while((stage_clear == 0) && ((sem + sem_id)->turn[j] == core_number));

		}
}

/*
 * Binary semaphore release in oreder to end critical section
 * Set ready[i] to zero where i is the current process
 *
 */
void semaphore_release(int sem_id, int core_number)
{
	smphr_t *sem;
	sem = (smphr_t*) SHARED_MEM_BASE_ADDR;

	(sem + sem_id)->ready[core_number] = 0;
}

/*
 * Signal operation for waiting task, checks if another task has given the signal
 */
void signal_wait(int id)
{
	signal_t *sem;
	sem = (signal_t*) SHERED_MEM_BASE_SIGNAL;
	while((sem + id)->count == 0);
	if((sem + id)->count == 1)
	{
		(sem + id)->count--;
	}

}

/*
 * Signal operation for active task, sends signal to waiting task
 */
void signal_release(int id)
{
	signal_t *sem;
	sem = (signal_t*) SHERED_MEM_BASE_SIGNAL;
	if((sem + id)->count == 0)
	{
		(sem + id)->count++;

	}
}

/*
 * Init function for setting up the signal structure
 */
void signal_init(int id, int count)
{
	signal_t *sem;
	sem = (signal_t*) SHERED_MEM_BASE_SIGNAL;
	(sem + id)->id = id;
	(sem + id)->count = count;
}



