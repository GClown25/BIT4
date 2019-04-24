/*
 * tps.c
 *
 * Created: 09.02.2019
 * Author : GClown25
 */ 

#include "tps.h"

#include <stdint.h>

void tps_splitInstruction(uint8_t aInstruction, uint8_t *aCommand, uint8_t *aData){
	*aCommand = (aInstruction & 0xf0) >> 4;
	*aData = aInstruction & 0x0f;
}

void tps_unifyInstruction(uint8_t *aInstruction, uint8_t aCommand, uint8_t aData){
	*aInstruction = ((aCommand & 0x0f) << 4) | (aData & 0x0f);
}


uint8_t tps_add(uint8_t term1, uint8_t term2){
	uint8_t sum;
	
	term1 = term1 & 0xf;
	term2 = term2 & 0xf;
	
	sum = term1 + term2;
	
	if(sum > 15){
		sum -= 16;
	}
		
	return sum;
}

uint8_t tps_subtract(uint8_t minuend, uint8_t subtrahend){
	int8_t difference;
	
	minuend = minuend & 0xf;
	subtrahend = subtrahend & 0xf;
	
	difference = minuend - subtrahend;
	
	if (difference < 0){
		difference += 16;
	}
	
	return difference;
}

uint8_t tps_multiply(uint8_t multiplier1, uint8_t multiplier2){
	uint8_t product;
	
	multiplier1 = multiplier1 & 0xf;
	multiplier2 = multiplier2 & 0xf;
	
	product = multiplier1 * multiplier2;
	
	while(product > 15){
		product -= 16;	
	}
	
	return product;
}

uint8_t tps_divide(uint8_t dividend, uint8_t divisor){
	uint8_t quotient;
	
	dividend = dividend & 0xf;
	divisor = divisor & 0xf;
	
	quotient = dividend/divisor;
	
	quotient = quotient & 0xf;
	
	return quotient;
}

uint8_t tps_mod(uint8_t a, uint8_t b){
	a = a & 0xf;
	b = b & 0xf;
	
	a = a % b;
	
	b = b & 0xf;
	
	return a;
}

uint8_t tps_shiftLeft(uint8_t x, uint8_t n){
	x = x & 0xf;
	n = n & 0xf;
	
	x = x << n;
	
	x = x & 0xf;
	
	return x;
}

uint8_t tps_shiftRight(uint8_t x, uint8_t n){
	x = x & 0xf;
	n = n & 0xf;
	
	x = x >> n;
	
	x = x & 0xf;
	
	return x;
}