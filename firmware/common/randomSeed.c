/*
 * randomSeed.c
 *
 * Created: 24.4.2019
 * Author : GClown25
 */ 

#include "randomSeed.h"
#include "../peripheral/adc.h"
#include <inttypes.h>

uint16_t getSeed(){
	uint16_t seed = 0;
	uint16_t adcResult;
	
	for(int x = 0; x < 5; x++){
		for(int i = 0; i <= 15; i++){
			adcResult = adcRead();
			seed |= ((adcResult&0x01) << i);
		}
		if(seed != 0)break;
	}
	
	return seed;
}