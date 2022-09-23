#include "adc.h"
#include "tm4c123gh6pm.h"


void ADC0_init(void){
	/******* Clock initializations *******/
	
	/*PORTE clock enable*/
	SYSCTL_RCGCGPIO_R|=(1<<ADC_PORT_ID);
	while((SYSCTL_PRGPIO_R&(1<<ADC_PORT_ID))==0);
	
	/* ADC0 clock enable */
	SYSCTL_RCGCADC_R|=(1<<ADC0_ID);
	while((SYSCTL_PRADC_R&(1<<ADC0_ID))==0);
	
	/******* PE3 initialization *******/
	
	/* AN0/PE3 alternate function */
	GPIO_PORTE_AFSEL_R|=(1<<AN0_PIN_ID);
	
	/* Disable digital function of AN0/PE3 since it will recieve an analog input */
	GPIO_PORTE_DEN_R&=~(1<<AN0_PIN_ID);
	
	/* AN0/PE3 Analog mode */
	GPIO_PORTE_AMSEL_R|=(1<<AN0_PIN_ID);
	
	
	/******* SS3 initialization *******/
	
	/* Deactivate SS3 during initialization */
	ADC0_ACTSS_R&=~(1<<SS3_ID);
	
	/* Continuously sample from SS3 (set the four bits of EM3 on Event Multiplexer (EMUX) ) */
	ADC0_EMUX_R &=~ (0x0f<<EM3_ID);
	
	/* SS3 takes a single sample and raises flag when done */
	ADC0_SSCTL3_R|=(1<<END0_ID)|(1<<IE0_ID);
	
	/* Reactivate SS3 */
	ADC0_ACTSS_R|=(1<<SS3_ID);
}

/* This function does not take a channel number since SS3 has only one channel */
uint16_t Analog_read(void){
	uint16_t ADC_value;
	/* Start sampling from SS3 */
	ADC0_PSSI_R|=(1<<SS3_ID);
	
	/* Poll interrupt flag */
	while((ADC0_RIS_R&(1<<IN3_ID))==0);
	
	/* Get ADC value after conversion, SSFIFOx returns 11 bit ADC Data from Sample Sequencer x */
	ADC_value=ADC0_SSFIFO3_R;
	ADC0_ISC_R=(1<<IN3_ID);
	
	return ADC_value;
}