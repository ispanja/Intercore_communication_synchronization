#include <FreeRTOS.h>
#include <task.h>
#include "sys_init.h"
#include "smphr.h"
#include "queue_fifo.h"


static TaskHandle_t task_handler;

/* Main task running libmetal demo */
static void processing_task(void *pvParameters)
{
	//int status;
	//LPRINTF("Core0: Task starts.\r\n");

	status = sys_init();

//	if (status) {
//		//LPERROR("Core0: Failed to initialize system.\r\n");
//		/* Terminate this task */
//		vTaskDelete(NULL);
//	}
//
//	status = shared_memory_run();
//	if (status){
//		//LPERROR("Core0: Shared memory demo failed.\r\n");
//		/* Terminate this task */
//		vTaskDelete(NULL);
//	}

	LPRINTF("Init structures.\r\n");

	//Init semaphore for serial
	semaphore_init(0);

	//Init semaphore for queue resource
	semaphore_init(1);

	//Init queue for tasks
	//queue_init(0, 5);

	CRITICAL_PRINTF("CORE0: take semaphore\r\n");
	//Take semaphore for writting in queue
	semaphore_take(1, 0);

	//CRITICAL_PRINTF("CORE0 write\r\n");
	//Write in the queue
	//queue_send(0, 7);

	taskENTER_CRITICAL();
	for(int i = 0; i < 10; i++)
	{
			CRITICAL_PRINTF("Core0: Critical section.\r\n");
	}

	taskEXIT_CRITICAL();
	//CRITICAL_PRINTF("CORE0 end write\r\n");

	//Release semaphore for writting in the queueu
	semaphore_release(1, 0);
	CRITICAL_PRINTF("CORE0: semaphore release\r\n");


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
		//LPERROR("Core0: Cannot create task\n");
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
