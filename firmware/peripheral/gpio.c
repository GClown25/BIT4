/*
 * gpio.c
 *
 * Created: 09.02.2019
 * Author : GClown25
 */ 

#include "gpio.h"

#include <avr/io.h>
#include <inttypes.h>
#include <stdbool.h>

const uint8_t lookupReverse4bit[] = {0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf};



void gpio_dout_init(GPIO_DOUT *aDout){
	//Configure the 4 DOUT bits a output
	*aDout->dir |= MASK_SET_4BIT(aDout->pin0);
}

void gpio_din_init(GPIO_DIN *aDin){
	//Configure the 4 DIN bits a input
	*aDin->dir &= MASK_CLEAR_4BIT(aDin->pin0);
	
	//Set the internal Pullup for the input pins
	for(int i = 0; i  <= 3; i++){
		*(aDin->pinctrl + i) = 0x08;
	}
}

void gpio_btn_init(GPIO_BTN *aBtn){
	//Configure button pin as input
	*aBtn->dir &= ~(1 << aBtn->pin); 
	
	//Set the internal pullup for the button
	*aBtn->pinctrl = 0x08;
}



void gpio_dout_write(GPIO_DOUT *aDout, uint8_t aData){
	uint8_t tempReg, tempData;
	
	if(aDout->sequence == REVERSED){
		tempData = lookupReverse4bit[aData];
	}else{
		tempData = aData;
	}
	
	tempReg = *aDout->out;
	
	tempReg |= (MASK_SET_4BIT(aDout->pin0) & (tempData << aDout->pin0));
	tempReg &= (MASK_CLEAR_4BIT(aDout->pin0) | (tempData << aDout->pin0));
	
	*aDout->out = tempReg;
}

void gpio_dout_writeBit(GPIO_DOUT *aDout, uint8_t aBit, uint8_t aBitNr){
	uint8_t tempReg, tempBitNr;
	
	if(aDout->sequence == REVERSED){
		tempBitNr = 3 - aBitNr; 
	}else{
		tempBitNr = aBitNr;
	}
	
	tempReg = *aDout->out;
	
	tempReg |= (MASK_SET_1BIT(aDout->pin0 + tempBitNr) & (aBit << (aDout->pin0 + tempBitNr)));
	tempReg &= (MASK_CLEAR_1BIT(aDout->pin0 + tempBitNr) | (aBit << (aDout->pin0 + tempBitNr)));
	
	*aDout->out = tempReg;
}

uint8_t gpio_dout_read(GPIO_DOUT *aDout){
	uint8_t tempReg;
	
	tempReg = (*aDout->out >> aDout->pin0) & 0xf;
	
	if(aDout->sequence == REVERSED){
		return lookupReverse4bit[tempReg];
		}else{
		return tempReg;
	}	
	
}



uint8_t gpio_din_read(GPIO_DIN *aDin){
	uint8_t tempReg;
	
	tempReg = (*aDin->in >> aDin->pin0) & 0xf;
	
	if(aDin->sequence == REVERSED){
		return lookupReverse4bit[tempReg];
	}else{
		return tempReg;
	}
	
}


uint8_t gpio_din_readBit(GPIO_DIN *aDin, uint8_t aBitNr){
	uint8_t tempReg;
	
	tempReg = gpio_din_read(aDin);
	
	return (tempReg >> aBitNr) & 0x01;
}


bool gpio_btn_read(GPIO_BTN *aBtn){ 
	return (*aBtn->in & (1 << aBtn->pin)) ? true : false;
}
