/*
 * adc.h
 *
 * Created: 24.4.2019
 * Author : GClown25
 */ 

#ifndef ADC_H
#define ADC_H

#include <inttypes.h>

void adcInit(void);
void adcDeInit(void);
uint16_t adcRead(void);


#endif //ADC_H