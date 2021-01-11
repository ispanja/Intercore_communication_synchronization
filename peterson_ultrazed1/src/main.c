#include <FreeRTOS.h>
#include <task.h>
#include "sys_init.h"
#include "smphr.h"

static TaskHandle_t task_handler;

/* Main task running libmetal demo */
static void processing_task(void *pvParameters)
{
	int status;
	//LPRINTF("Core1: Task starts.\r\n");

	status = sys_init();

	if (status) {
		//LPERROR("Core1: Failed to initialize system.\r\n");
		/* Terminate this task */
		vTaskDelete(NULL);
	}

	status = shared_memory_run();
	if (status){
		//LPERROR("Core1: Shared memory demo failed.\r\n");
		/* Terminate this task */
		vTaskDelete(NULL);
	}

	//LPRINTF("Core1: Task done");

	CRITICAL_PRINTF("Core1: Take Semaphore.\r\n");
	semaphore_take(2, 1);
	for (int i=0; i<10; i++)
	{
		CRITICAL_PRINTF("Core1: Critical section.\r\n");
	}

	CRITICAL_PRINTF("Core1: Release Semaphore.\r\n");
	semaphore_release(2, 1);

	sys_cleanup();

	/* Terminate this task */
	vTaskDelete(NULL);
}

/* Main function */
int main(void)
{
	BaseType_t status;
	//LPRINTF("Core0: Main starts.\r\n");

	/* Create the tasks */
	status = xTaskCreate(processing_task, ( const char * ) "HW", 2048, NULL, 1, &task_handler);
	if (status != pdPASS) {
		LPERROR("Core0: Cannot create task\n");
	} else {
		/* Start running FreeRTOS tasks */
		//LPRINTF("Core0: Start Scheduler\r\n");
		vTaskStartScheduler();
	}

	/* Program will not end up here */
	while (1) {

	}

	return 0;
}
