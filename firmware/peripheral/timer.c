/*
 * timer.c
 *
 * Created: 10.4.2019
 * Author : GClown25
 */ 

#include "timer.h"

#include <avr/io.h>

//exponential function
const uint8_t brightnessLookup[] = {0, 1, 2, 3, 4, 6, 9, 13, 19, 27, 40, 58, 84, 121, 176, 255};

void timerb_initPWM(TCB_t *tcbn){
	
	tcbn->CTRLA = TCB_CLKSEL_CLKDIV2_gc;
	tcbn->CTRLB = TCB_CCMPEN_bm | TCB_CNTMODE_PWM8_gc;
	
	tcbn->CCMPL = 0xff;	//Set period of the output pulse
}

void timerb_setDC(TCB_t *tcbn, uint8_t aDC){
	
	tcbn->CTRLA &= ~TCB_ENABLE_bm;	//disable timer
	
	tcbn->CCMPL = 0xff;	//Set period of the output pulse
	tcbn->CCMPH = brightnessLookup[aDC & 0xf];	//Set DC
	tcbn->CNT = 0x0000;		
	
	tcbn->CTRLA |= TCB_ENABLE_bm;	//enable timer
}