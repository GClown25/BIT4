/*
 * timer.h
 *
 * Created: 10.4.2019
 * Author : GClown25
 */ 

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>



/*
	@brief: Init Timer TypeB for basic pwm led brightness control.
			Note that timer starts with first call of 'timerb_setDC'.
			
*/
void timerb_initPWM(TCB_t *tcbn);

/*
	@brief: The duty cycle for the timerB is set, according to an
			exponential function to let the led brightness control appear smoother
			
	@param: aDC	should have a value 
				between 0 and 15. (4bit value)
*/
void timerb_setDC(TCB_t *tcbn, uint8_t aDC);


#endif // TIMER_H