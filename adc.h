#ifndef ADC_H_
#define ADC_H_

#include "stdint.h"

/*******************************************************************************
 *                         Definitions                                         *
 *******************************************************************************/

#define ADC0_ID 0
#define ADC1_ID 1
#define ADC_PORT_ID 4
#define AN0_PIN_ID 3
#define SS3_ID 3 /* Sample Sequencer 3 */
#define EM3_ID 3 /* SS3 Trigger Select: send a trigger mode number and shift it left with this */
#define END0_ID 1 /* End of Sequence bit; take one sample at a time */
#define IE0_ID 2 /* Sample Interrupt Enable: raises flag when sampling is done */
#define IN3_ID 3 /* SS3 Interrupt Status flag: cleared by a logical one */

#define ADC_MAXIMUM_VALUE    4095
#define ADC_REF_VOLT_VALUE   3.3

/*******************************************************************************
 *                         Function Prototypes                                 *
 *******************************************************************************/
 /*Description:
   Initializes ADC0 and PE3 to convert analog input though channel 0 on Sample Sequencer 3.
	
 */
uint16_t ADC0_readChannel(void);

void ADC0_init(void);
#endif