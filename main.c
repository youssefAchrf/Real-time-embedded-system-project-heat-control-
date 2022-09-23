#define Led_Red (0x02U)
#define Led_Blue (0x04U)
#define Led_Green (0x08U)
#include "system_TM4C123.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lcd.h"
#include "adc.h"
#include "UART.h"

char Temp;
static void MainTask(void * pvParameters);
static void LCDTask(void * pvParameters);
static void setpointmainTask(void * pvParameters);

xQueueHandle sptomain_xqueue;
xQueueHandle temptolcd_xqueue;
xQueueHandle sptolcd_xqueue;

 int main()
 {
 UART_init();
 ADC0_init();
 LCD_init(); 
	

	
  sptomain_xqueue = xQueueCreate (2, sizeof(long));   // queue to send and recieve the setpoint from setpoint task to the main task
	temptolcd_xqueue = xQueueCreate (2, sizeof(long));   // queue to send and recieve the temperature 
	sptolcd_xqueue = xQueueCreate (2, sizeof(long));      // queue to send and recieve the setpoint from the main task to the lcd task
	 
	xTaskCreate( MainTask, "MainTask CONTROLLER", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	xTaskCreate( LCDTask, "LCD CONTROLLER", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	xTaskCreate( setpointmainTask," setpointmain", configMINIMAL_STACK_SIZE, NULL,2, NULL );

	vTaskStartScheduler();
	 while(1){}
}

static void MainTask(void * pvParameters)
{
    int setpointmain =25;   // first initialize the setpoint to be compared with the Adc value
    for (;;)
	{  
		xQueueSendToBack(sptolcd_xqueue,&setpointmain, 10/portTICK_RATE_MS);  // send the setpoint to the lcd task 
		xQueueSendToBack(temptolcd_xqueue,&Temp, 10/portTICK_RATE_MS);       // send the temperature to the lcd task 
		xQueueReceive(sptomain_xqueue,&setpointmain,0);                      // receive the new setpoint from the setpoint task
		int ADC_VALUE ;
		ADC_VALUE = Analog_read();
		int Temp_NOW = ADC_VALUE / 32;
		if (Temp_NOW != Temp)
		{
			Temp = Temp_NOW;
			if (Temp < setpointmain )
			{
				GPIO_PORTF_DATA_BITS_R[Led_Red] |= Led_Red;  

			}
			else if (Temp >= setpointmain)
			{
				GPIO_PORTF_DATA_BITS_R[Led_Red] &=~ Led_Red;

			}	
		}
		
	}
	
}
static void LCDTask(void * pvParameters)
{
	 
	 int setpointlcd;
	 char templcd;
char t;
 for(;;)
   {
		 
		xQueueReceive(temptolcd_xqueue,&templcd,0);    // receive from the main task the temperature
   	xQueueReceive(sptolcd_xqueue,&setpointlcd,0);  // receive from the main task the setpoint
		 
		 
		LCD_Cmd(clear_display);
		LCD_Cmd(cursorOn);
		LCD_Cmd(FirstRow);
		LCD_Cmd(moveCursorLeft);
	  LCD_String("measured: ");
		
		LCD_Write_Integer(templcd);
		 
		LCD_Cmd(SecondRow);
		LCD_Cmd(moveCursorLeft);
	  LCD_String("setpoint: ");
		 
		LCD_Write_Integer(setpointlcd);

		LCD_Cmd(cursorOff);
		delay_ms(300);
    }
}


static void setpointmainTask(void * pvParameters)
{
	int t;
	
 for (;;)
 {
	 MyPrint("Enter the Setpoint Please");
	 t=Input(); // input means that the UART receiver is enabled so you can write on the Putty display
	 xQueueSendToBack(sptomain_xqueue,&t,10/portTICK_RATE_MS);    // send the input value which is converted to integer value to the main task 
 }
}