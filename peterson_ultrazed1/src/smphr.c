/*
 * smphr.c

 * Semaphores implemented using Peterson algorithm
 *  Created on: Jan 20, 2020
 *      Author: ivan
 */

/* Implementation Peterson algorithm for 2 processes */
/* Binary sempahore */

#include "smphr.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SHARED_MEM_BASE_ADDR	0x3ED80000
#define CORE 					0x1		//Core number must be 0 or 1

/*
 * Binary semahore init
 * Set Q[] to zero and turn to zero for appropriate semaphore id
 */
void semaphore_init (int sem_id)
{
	smphr_t *sem;
	sem = (smphr_t*) SHARED_MEM_BASE_ADDR;

	(sem + sem_id)->sem_id = sem_id;
	(sem + sem_id)->turn = 0;
	(sem + sem_id)->ready[0] = 0;
	(sem + sem_id)->ready[1] = 0;
}

/*
 * Binary semaphore take in order to start critical section
 * Set Q[i] to 1 where i is the current task
 * Turn shows which process has previously entered
 */
void semaphore_take(int sem_id, int core_number)
{
	smphr_t *sem;
	sem = (smphr_t*) SHARED_MEM_BASE_ADDR;

	(sem + sem_id)->ready[core_number] = 1;
	(sem + sem_id)->turn = CORE - core_number;

	while((sem + sem_id)->ready[CORE - core_number] == 1 && ((sem + sem_id)->turn == CORE - core_number));
}

/*
 * Binary semaphore release in oreder to end critical section
 * Set Q[i] to zero where i is the current process
 *
 */
void semaphore_release(int sem_id, int core_number)
{
	smphr_t *sem;
	sem = (smphr_t*) SHARED_MEM_BASE_ADDR;

	(sem + sem_id)->ready[core_number] = 0;
}



