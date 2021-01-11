/*
 * queue_fifo.h

 *
 *  Created on: Jan 21, 2020
 *      Author: ivan
 */

#ifndef SRC_QUEUE_FIFO_H_
#define SRC_QUEUE_FIFO_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_BASE_ADDR	0x3ED80400

/* Struct used for message queues */
typedef struct
{
	int id;
	int head;
	int tail;
	int len;
	int full;
	int arr[];
} que_t;

void queue_send(int id, int new_number);
int queue_get(int id);
void queue_init(int id, int len);
int queue_empty(int id);
int queue_full(int id);



#endif /* SRC_QUEUE_FIFO_H_ */
