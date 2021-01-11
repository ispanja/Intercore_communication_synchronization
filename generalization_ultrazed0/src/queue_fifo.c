/*
 * queue_fifo.c

 *
 *  Created on: Jan 21, 2020
 *      Author: ivan
 */

#include "queue_fifo.h"
#include "smphr.h"
#include "sys_init.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/* Init queue elements, tail, head, and other common stuff */
void queue_init(int id, int len)
{
	int i;
	que_t *que = (que_t*) QUEUE_BASE_ADDR;
	(que + id)->id = id;
	(que + id)->head = 0;
	(que + id)->tail = 0;
	(que + id)->len = len;
	(que + id)->full = 0;
	for(i = 0; i<len; i++)
	{
		(que + id)->arr[i] = 0;
	}
}

/* Send number into queue, shift tail */
void queue_send(int id, int new_number)
{
	que_t *que = (que_t*) QUEUE_BASE_ADDR;
	if(!(que + id)->full)
	{
		if(queue_full(id))
		{
			(que + id)->arr[(que + id)->tail] = new_number;
			CRITICAL_PRINTF("Red je postao pun..\r\n");
			(que + id)->full = 1;
		}
		else
		{
			CRITICAL_PRINTF("Enqueu: %d\n", new_number);
			(que + id)->arr[(que + id)->tail] = new_number;
			(que + id)->tail = ((que + id)->tail + 1) % ((que + id)->len);
		}
	}
	else
	{
		CRITICAL_PRINTF("Red je pun...\r\n");
	}
}

/* Get number from the queue, shift head for 1 */
int queue_get(int id)
{
	que_t *que = (que_t*) QUEUE_BASE_ADDR;
	int x;
	if(!queue_empty(id))
	{
		x = (que + id)->arr[(que + id)->head];
		CRITICAL_PRINTF("Dequeu: %d\r\n", x);
		(que + id)->head = ((que + id)->head + 1) % ((que + id)->len);

		if((que + id)->full)
		{
			(que + id)->full = 0;
			(que + id)->tail = ((que + id)->tail + 1) % ((que + id)->len);
		}

		if(queue_empty(id))
		{
			CRITICAL_PRINTF("Red je postao prazan..\r\n");
		}
	}
	else
	{
		CRITICAL_PRINTF("Red je prazan...\r\n");
	}
	return x;
}

/* Check if queue is empty, return 1 for empty, 0 for non-empty */
int queue_empty(int id)
{
	que_t *que = (que_t*) QUEUE_BASE_ADDR;
	return (que + id)->tail == (que + id)->head;
}

/* Check if queue is full, return 1 for ful, 0 for non-full */
int queue_full(int id)
{
	que_t *que = (que_t*) QUEUE_BASE_ADDR;
	return ((que + id)->tail - (que + id)->head == (que + id)->len - 1);
}

