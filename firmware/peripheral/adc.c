/*
 * adc.c
 *
 * Created: 24.4.2019
 * Author : GClown25
 */ 

#include "adc.h"
#include <avr/io.h>
#include <inttypes.h>

/*
*	@brief:	Init ADC to measure the internal temperature sensor
*/
void adcInit(void)
{
	// 	The adc gets initialized with a faster sample time than for the internal
	// 	temp sensor recommended, to get more noise.
	
	VREF.CTRLA |= VREF_ADC0REFSEL_1V1_gc;
	VREF.CTRLB |= VREF_ADC0REFEN_bm;
	
	ADC0.CTRLC = ADC_REFSEL_INTREF_gc | ADC_PRESC_DIV2_gc | ADC_SAMPCAP_bm;
	ADC0.MUXPOS = ADC_MUXPOS_TEMPSENSE_gc;
	
	ADC0.CTRLD = ADC_INITDLY_DLY16_gc;
	ADC0.SAMPCTRL = 0x0;
	
	ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc;
	
}

void adcDeInit(void){
	VREF.CTRLB &= ~VREF_ADC0REFEN_bm;
	ADC0.CTRLA &= ~ADC_ENABLE_bm;
	
}

uint16_t adcRead(void)
{
	/* Start ADC conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
	/* Wait until ADC conversion done */
	while ( !(ADC0.INTFLAGS & ADC_RESRDY_bm) )
	{
		;
	}
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	return ADC0.RES;
	
}
