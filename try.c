#define Led_Red (0x02U)
#define Led_Blue (0x04U)
#define Led_Green (0x08U)
/*Includes*/
#include "system_TM4C123.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "semphr.h"


void UART0_Transmitter(char data)  
{
    while((	UART0_FR_R& 0x20) != 0); 
    UART0_DR_R = data;                 
}
void main_controller(){

char *intro = " temperature setpoint: ";
	for (i=0;i<25;i++){
		UART0_Transmitter(intro[i]);
	}
	vtaskdelay(800000);
	
	printstring("Hello World \n");
	Delay(10); 
	while(1)
	{
		char c = UART0_Receiver();          /* get a character from UART5 */
		UART0_Transmitter(c); 
	}
}
char UART0_Receiver(void)  
{
    char data;
	  while((UART0->FR & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART0->DR ;  	/* before giving it another byte */
    return (unsigned char) data; 
		printf(data);
}

void uart_controller(){}
void lcd_controller(){}

void buzzer_controller(){}


int main(){
	SYSCTL_RCGCGPIO_R |= 0x01U;
	SYSCTL_RCGCUART_R |= 0x01U; 
	while(SYSCTL_PRGPIO_R & 0x01U == 0x00){}
	UART0_CTL_R &= ~0x001;
	UART0_IBRD_R = 104;
	UART0_FBRD_R = 11;
	UART0_CC_R = 0;
	UART0_LCRH_R = 0x060;
	UART0_CTL_R = 0x0301;
		
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0XFFFFFF00) + 0X011;
	GPIO_PORTA_DEN_R |= 0x03;
	GPIO_PORTA_AMSEL_R &= ~0x03;
	GPIO_PORTF_IS_R &= ~0x10;
	GPIO_PORTF_IBE_R &= ~0x10;
	GPIO_PORTF_IEV_R &= ~0x10;
	GPIO_PORTF_ICR_R |= 0x10;
	GPIO_PORTF_IM_R |= 0x10;
	
	NVIC_PRI7_R = 7<<21;
	NVIC_EN0_R |= 1 <<30;
	
	xMutex = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(xSemaphoreHandler);
	xTaskCreate( main_controller, "MAIN CONTROLLER", 240, NULL, 2, NULL );
	xTaskCreate( uart_controller, "UART CONTROLLER", 240, NULL, 1, NULL );
	xTaskCreate( lcd_controller,"LCD CONTROLLER", 240, NULL, 4, NULL );
	xTaskCreate( buzzer_controller,"buzzer_CONTROLLER", 240, NULL, 4, NULL );
	

	vTaskStartScheduler();
}

void vApplicationIdleHook( void ){
	__asm("WFI");
}