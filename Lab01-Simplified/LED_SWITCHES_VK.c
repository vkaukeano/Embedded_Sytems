/*
 * LED_SWITCHES_VK.c
 *
 *  Created on: 	23 August 2020
 *      Author: 	Von Kaukeano
 *     Version:		1.0
 */

/********************************************************************************************
 * This file contains an example of using the GPIO driver to provide communication between
 * the Zynq Processing System (PS) and the AXI GPIO block implemented in the Zynq Programmable
 * Logic (PL). The AXI GPIO is connected to the LEDs on the Zybo.
 *
 * The provided code demonstrates how to use the GPIO driver to write to the memory mapped AXI
 * GPIO block, which in turn controls the LEDs.
 ********************************************************************************************/

/* Include Files */
#include "xparameters.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xil_printf.h"

/* Definitions */
#define GPIO_LED_ID  XPAR_AXI_GPIO_0_DEVICE_ID	/* GPIO device that LEDs are connected to */
#define GPIO_SW_ID   XPAR_AXI_GPIO_1_DEVICE_ID  /* GPIO device that SWs are connected to */
#define LED_DELAY 10000000					/* Software delay length */


XGpio LEDInst, SWInst;							/* GPIO Device driver instance */


int Delay(void){
	volatile int Delay;
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	return 0;
}

int Lab01(void)
{

	//int maskVal = 0x01;
	int i = 0;
	//int input= 0;
	int led_count = 0x00;
	int switchVal = 0x00;							// holds switch value
	int SW2[] = {0x00,0x01,0x03,0x07,0x0F};
	int SW3[] = {0x09,0x06,0x10,0x05};
	int error = 0x09;
		/* Loop forever blinking the LED. */
			while (1) {

				switchVal = XGpio_DiscreteRead(&SWInst, 1);		//read switches
				switch(switchVal){
				/* SWITCH 0 */
				case 0x01 :
					led_count = 0;
					XGpio_DiscreteWrite(&LEDInst, 1,led_count);
					break;

				/* SWITCH 1 */
				case 0x02 :
					led_count = led_count;
					XGpio_DiscreteWrite(&LEDInst, 1,led_count);
					break;

				/* SWITCH 2 */
				case 0x04 :
				for(i = 0; i <= 4; i++){
					led_count = SW2[i];
					Delay();
					XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				}
				for(i = 4; i >= 0; i--){
					led_count = SW2[i];
					Delay();
					XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				}
				break;

				/* SWITCH 3 */
				case 0x08:
				for(i = 0; i <= 4; i++){
				led_count = SW3[i];
				Delay();
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				}
				break;

				/* ERROR CODE **/
				case 0x03:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x05:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x06:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x07:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x09:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0xA:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x0B:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x0C:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x0D:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x0E:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				case 0x0F:
				led_count = error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				led_count = ~error;
				XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				Delay();
				break;

				default:
					XGpio_DiscreteWrite(&LEDInst, 1,led_count);
				}



			};

		return XST_SUCCESS; /* Should be unreachable */
}

/* Main function. */
int main(void){

	int Status;
	  Status = XGpio_Initialize(&LEDInst, GPIO_LED_ID);			//initialize led gpio
	  if(Status != XST_SUCCESS) return XST_FAILURE;				//check the initialization

	  Status = XGpio_Initialize(&SWInst, GPIO_SW_ID);			//initialize switches gpio
	  if(Status != XST_SUCCESS) return XST_FAILURE;				//check the initialization


	  XGpio_SetDataDirection(&LEDInst, 1, 0x00);				//leds are outputs
	  XGpio_SetDataDirection(&SWInst, 1, 0xFF);					//switches are inputs

	/* Execute the LED output. */
	Status = Lab01();
	if (Status != XST_SUCCESS) {
		xil_printf("GPIO output to the LEDs failed!\r\n");
	}

	return 0;
}

