/*
 * freertos_hello_world.c
 *
 *  Created on: 	10 September 2020
 *      Author: 	Von Kaukeano
 *     Version:		1.0
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Xilinx includes. */
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"						/* loads functions for gpio */
#include "xstatus.h"
#include <stdlib.h>

/* Definitions and Global variables */
#define LED_DELAY 10000000							/* Software delay length */
#define GPIO_LED_ID  XPAR_AXI_GPIO_1_DEVICE_ID	   	/* GPIO device that LEDs are connected to */
#define GPIO_SW_ID   XPAR_AXI_GPIO_0_DEVICE_ID		/* GPIO device that SWs are connected to */
XGpio LEDInst, SWInst;							/* GPIO Device driver instance */
/*-----------------------------------------------------------*/

/* The LED and SW tasks as described at the top of this file. */
static void vTaskLED( void *pvParameters );
static void vTaskSW( void *pvParameters );
int Delay( void );
/*-----------------------------------------------------------*/


int main( void )
{
	int Status;
	  Status = XGpio_Initialize(&LEDInst, GPIO_LED_ID);			/* Initialize led gpio */
	  if(Status != XST_SUCCESS) return XST_FAILURE;				/* Check the initialization */

	  Status = XGpio_Initialize(&SWInst, GPIO_SW_ID);			/* Initialize switches gpio */
	  if(Status != XST_SUCCESS) return XST_FAILURE;				/* Check the initialization */


	  XGpio_SetDataDirection(&LEDInst, 1, 0x00);				/* Leds are outputs */
	  XGpio_SetDataDirection(&SWInst, 1, 0xFF);					/* Switches are inputs */

	TaskHandle_t LED_Handle;											/* Handle for LED Task */
	TaskHandle_t SW_Handle;												/* Handle for SW Task */

	xTaskCreate( vTaskLED, 					/* The function that implements the task. */
				( const char * ) "LED Task", 		/* Text name for the task, provided to assist debugging only. */
				configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
				NULL, 						/* The task parameter is not used, so set to NULL. */
				tskIDLE_PRIORITY + 1,			/* The task runs at the idle priority. */
				&LED_Handle );

	xTaskCreate( vTaskSW,
				 ( const char * ) "SW Task",
				 configMINIMAL_STACK_SIZE,
				 LED_Handle,
				 tskIDLE_PRIORITY + 1,
				 &SW_Handle );

	vTaskStartScheduler();												/* Start the scheduler */


	return 0;
}


/*---------------------LED Task------------------------------------*/
static void vTaskLED( void *pvParameters )
{
	uint32_t SW[] =  {0x00,0x09,0x06,0xA,0x01,0x05,0x08,0x04,0x0c, 0x0e, 0x02,0x0b,0x0d,0x03,0x07,0x0f};
	uint32_t i = 0;
	uint32_t led_count = 0x00;

	for( ;; ){
		for(i = 0; i <= sizeof(SW); i++){
			led_count = SW[i];
			Delay();
			XGpio_DiscreteWrite(&LEDInst, 1,led_count);
		}
	}
}

/*----------------------SW Task------------------------------------*/
static void vTaskSW( void *pvParameters )
{
	uint32_t switchVal;													/* Holds value of input from switches */

	TaskHandle_t xHandle = (TaskHandle_t)pvParameters;					/* Reads the passed in parameters as a handle (this is the LEDTask handle) */


	for( ;; ){
		switchVal = XGpio_DiscreteRead(&SWInst, 1);						/* Read  */
		if(switchVal == 0x01){											/* If only Switch 0 is activated */
			vTaskSuspend(xHandle);										/* Suspend the LED task */
		}else if(switchVal== 0x02){										/* If only Switch 1 is activated */
			vTaskResume(xHandle);										/* resume the LED Task */
		}
	}
}

int Delay(void){
	volatile uint32_t Delay;
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	return 0;
}
