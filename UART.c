#include "system_TM4C123.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

void UART_init(void){
	SYSCTL_RCGCGPIO_R = 0x20U;
	while(SYSCTL_PRGPIO_R & 0x20U == 0x00){}
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x011U;
	GPIO_PORTF_PUR_R = 0x011U;
  GPIO_PORTF_DIR_R = 0x0EU;
  GPIO_PORTF_DEN_R = 0x01FU;
  SYSCTL_RCGCGPIO_R  |= 0x01;
  SYSCTL_RCGCUART_R  |= 0x01;
   while((SYSCTL_PRGPIO_R & 0x01) == 0)
   GPIO_PORTA_DEN_R   &= 0x00;
   GPIO_PORTA_AFSEL_R |= 0x03;
   GPIO_PORTA_DEN_R |= 0x03;
   GPIO_PORTA_AMSEL_R &= ~0x03;
   GPIO_PORTA_PDR_R   &= 0x00;
   GPIO_PORTA_PUR_R   &= 0x00;
   GPIO_PORTA_PCTL_R  |= 0x03;
   GPIO_PORTA_CR_R    |= 0x01;
   GPIO_PORTA_DR2R_R  |= 0x03;
   UART0_CTL_R &= ~0x01;
   UART0_IBRD_R = 104;
   UART0_FBRD_R = 11;
   UART0_LCRH_R |= 0x60;
   UART0_CTL_R |= 0x01;

		
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0XFFFFFF00) + 0X011;
	GPIO_PORTA_DEN_R |= 0x03;
	GPIO_PORTA_AMSEL_R &= ~0x03;
 }

void CharPrint(char x){
	while ((UART0_FR_R & (1<<5)) != 0);
	UART0_DR_R = x ;
}

char readChar (void)
{
	char c;
	while((UART0_FR_R & (1<<4)) != 0);
	c = UART0_DR_R;
	return c;
}

char Input (void)
{
	char d0 = readChar();
	CharPrint(d0);
	char d1 = readChar();
	CharPrint(d1);
	CharPrint('\r');
	CharPrint('\n');
	
return (d0-'0')*10 + (d1-'0');
	
}

void MyPrint(char* string)
{
	while (*string)
	{
		CharPrint(*(string++));
	}
	
	CharPrint('\r');
	CharPrint('\n');
}