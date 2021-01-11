/*
 * sys_init.c

 * Libmetal enviroment setup
 *  Created on: Jan 20, 2020
 *      Author: ivan
 */

#include "sys_init.h"
#include "smphr.h"
#include "queue_fifo.h"
#include <xparameters.h>
#include <xil_cache.h>
#include <xil_exception.h>
#include <xstatus.h>
#include <xscugic.h>
#include <errno.h>

#include <metal/io.h>
#include <metal/device.h>
#include <metal/sys.h>
#include <metal/irq.h>
#include <metal/alloc.h>
#include <metal/irq.h>

/* Default shared memory address */
#define SHM_BASE_ADDR   0x3ED80000

/* Default generic I/O region page shift */
/* Each I/O region can contain multiple pages.
 * In FreeRTOS system, the memory mapping is flat, there is no
 * virtual memory.
 * We can assume there is only one page in the FreeRTOS system.
 */
#define DEFAULT_PAGE_SHIFT (-1UL)
#define DEFAULT_PAGE_MASK  (-1UL)
#define DEVICE_NONSHARED (0x00000010U)
#define PRIV_RW_USER_RW (0x00000003U<<8U)
#define NORM_SHARED_NCACHE (0x0000000CU)

/* Devices names */
#define BUS_NAME        "generic"
#define SHM_DEV_NAME    "3ed80000.shm"

/* Shared memory offsets */
#define SHM_DEMO_CNTRL_OFFSET      0x0
#define DEMO_STATUS_IDLE         0x0
#define DEMO_STATUS_START        0x3 /* Status value to indicate demo start */
#define DEMO_STATUS_WRITE		 0x3

const metal_phys_addr_t metal_phys[] = {
	SHM_BASE_ADDR, /* shared memory base address */
};

/* Define metal devices table for IPI, shared memory and TTC devices. */
static struct metal_device metal_dev_table[] = {
	{
		/* Shared memory management device */
		.name = SHM_DEV_NAME,
		.bus = NULL,
		.num_regions = 1,
		.regions = {
			{
				.virt = (void *)SHM_BASE_ADDR,
				.physmap = &metal_phys[1],
				.size = 0x1000000,
				.page_shift = DEFAULT_PAGE_SHIFT,
				.page_mask = DEFAULT_PAGE_MASK,
				.mem_flags = NORM_SHARED_NCACHE |
						PRIV_RW_USER_RW,
				.ops = {NULL},
			}
		},
		.node = {NULL},
		.irq_num = 0,
		.irq_info = NULL,
	}
};

/**
 * Extern global variables
 */
struct metal_device *shm_dev = NULL;

void enable_caches()
{
	Xil_ICacheEnable();
	Xil_DCacheEnable();
}

void disable_caches()
{
	Xil_DCacheDisable();
	Xil_ICacheDisable();
}

/* This function registers the shared memory and to the libmetal generic bus. */
static int platform_register_metal_device(void)
{
	unsigned int i;
	int status;
	struct metal_device *dev;

	for (i = 0; i < sizeof(metal_dev_table)/sizeof(struct metal_device); i++) {
		dev = &metal_dev_table[i];
		//LPRINTF("Core0: registering: %d, name=%s\r\n", i, dev->name);
		status = metal_register_generic_device(dev);
		if (status)
			return status;
	}
	return 0;
}

/* Open metal devices */
static int open_metal_devices(void)
{
	int status;

	/* Open shared memory device */
	status = metal_device_open(BUS_NAME, SHM_DEV_NAME, &shm_dev);
	if (status) {
		//LPERROR("Core0: Failed to open device %s.\r\n", SHM_DEV_NAME);
		return status;
	}

	return status;
}

/* Close metal devices */
static void close_metal_devices(void)
{
	/* Close shared memory device */
	if (shm_dev){
		metal_device_close(shm_dev);
		//LPRINTF("Core0: Device closed;\r\n");
	}
}

/* Register libmetal devices */
int sys_init()
{
	struct metal_init_params metal_param = METAL_INIT_DEFAULTS;
	int status;

	//enable_caches();

	/* Initialize libmetal environment */
	metal_init(&metal_param);

	/* Register libmetal devices */
	status = platform_register_metal_device();
	if (status) {
		//LPERROR("Core0: %s: failed to register devices: %d\r\n", __func__, status);
		return status;
	}

	/* Open libmetal devices which have been registered */
	status = open_metal_devices();
	if (status) {
		//LPERROR("Core0: %s: failed to open devices: %d\r\n", __func__, status);
		return status;
	}

	return 0;
}

/* Clean libmetal devices */
void sys_cleanup()
{
	/* Close libmetal devices which have been opened */
	close_metal_devices();
	/* Finish libmetal environment */
	metal_finish();
	disable_caches();
}

/* Example using libmetal for writting in the shared memory */
int shared_memory_read_write(struct metal_io_region *shm_io)
{
	/* clear demo status value */
	//metal_io_write32(shm_io, SHM_DEMO_CNTRL_OFFSET, DEMO_STATUS_IDLE);

	//LPRINTF("Core0: Writting in memory.\r\n");
	//metal_io_write32(shm_io, SHM_DEMO_CNTRL_OFFSET, DEMO_STATUS_WRITE);
	//while (metal_io_read32(shm_io, SHM_DEMO_CNTRL_OFFSET) != DEMO_STATUS_START);

	//LPRINTF("Core0: Writting done.\r\n");

	//LPRINTF("Core0: Shared memory test finished\r\n");


	return 0;
}

int shared_memory_run()
{
	struct metal_device *device = NULL;
	struct metal_io_region *io = NULL;
	int status = 0;

	//LPRINTF("Core0: Shared memory run \r\n");

	/* Open the shared memory device */
	status = metal_device_open(BUS_NAME, SHM_DEV_NAME, &device);
	if (status) {
		//LPERROR("Core0: Failed to open device %s.\n", SHM_DEV_NAME);
		return status;
	}

	/* Get shared memory device IO region */
	if (!shm_dev) {
		status = -ENODEV;
		return status;
	}

	io = metal_device_io_region(shm_dev, 0);
	if (!io) {
		//LPERROR("Core0: Failed to map io region for %s.\n", shm_dev->name);
		status = -ENODEV;
		return status;
	}

	status = shared_memory_read_write(io);

	return status;

}


