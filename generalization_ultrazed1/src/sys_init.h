/*
 * sys_init.h

 *
 *  Created on: Jan 20, 2020
 *      Author: ivan
 */

#ifndef SRC_SYS_INIT_H_
#define SRC_SYS_INIT_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <xparameters.h>
#include <xil_cache.h>
#include <xil_exception.h>
#include <xstatus.h>
#include <xscugic.h>
#include <metal/io.h>
#include "smphr.h"

/* Devices names */
#define BUS_NAME        "generic"
#define SHM_DEV_NAME    "3ed80000.shm"

#define CRITICAL_PRINTF(format, ...) {\
	  semaphore_take(0, 1);\
  	  xil_printf("\r\nSERVER> " format, ##__VA_ARGS__);\
  	  semaphore_release(0, 1);\
}

#define LPRINTF(format, ...) \
  xil_printf("\r\nSERVER> " format, ##__VA_ARGS__)

#define LPERROR(format, ...) LPRINTF("ERROR: " format, ##__VA_ARGS__)

extern struct metal_device *shm_dev; /* SHM metal device */

/* Running program for writting and reading */
int shared_memory_run();

/*Writing in shared memory using libmetal*/
int shared_memory_read_write(struct metal_io_region *shm_io);

/*Init Libmetal enviroment*/
int sys_init();

/* Libmetal end function */
void sys_cleanup();

#endif /* SRC_SYS_INIT_H_ */
