/*
 * tps.h
 *
 * Created: 09.02.2019
 * Author : Martin Frauenschuh
 */ 


#ifndef TPS_H
#define TPS_H

#include <stdint.h>

void tps_splitInstruction(uint8_t aInstruction, uint8_t *aCommand, uint8_t *aData);
void tps_unifyInstruction(uint8_t *aInstruction, uint8_t aCommand, uint8_t aData);


uint8_t tps_add(uint8_t term1, uint8_t term2);
uint8_t tps_subtract(uint8_t minuend, uint8_t subtrahend);
uint8_t tps_multiply(uint8_t multiplier1, uint8_t multiplier2);
uint8_t tps_divide(uint8_t dividend, uint8_t divisor);

uint8_t tps_mod(uint8_t a, uint8_t b);

uint8_t tps_shiftLeft(uint8_t x, uint8_t n);
uint8_t tps_shiftRight(uint8_t x, uint8_t n);



#endif //TPS_H