/*
 * smphr.h

 *
 *  Created on: Jan 20, 2020
 *      Author: ivan
 */

#ifndef SRC_SMPHR_H_
#define SRC_SMPHR_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>

void semaphore_init (int sem_id);
void semaphore_take (int sem_id, int core_number);
void semaphore_release (int sem_id, int core_number);

void signal_wait(int id);
void signal_release(int id);
void signal_init(int id, int count);

#define N						4		//Total number of cores

/*
 * Semaphore structure
 * i current process
 * N number of processes
 * j stage number
 */
typedef struct
{
	int sem_id;
	int turn[N];
	int ready[N];	//Q[N]
} smphr_t;

/*
 * Signal structure
 */
typedef struct
{
	int id;
	int count;
} signal_t;

#endif /* SRC_SMPHR_H_ */
