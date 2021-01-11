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

/*
 * Semaphore structure
 */
typedef struct
{
	int sem_id;
	int turn;
	int ready[2];	//Q[2]
} smphr_t;


#endif /* SRC_SMPHR_H_ */
