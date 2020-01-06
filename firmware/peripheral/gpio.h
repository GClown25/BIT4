/*
 * gpio.h
 *
 * Created: 09.02.2019
 * Author : GClown25
 */

#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <inttypes.h>
#include <stdbool.h>

#define MASK_SET_4BIT(offset) (0x0f << (offset))
#define MASK_CLEAR_4BIT(offset) (~(0x0f << (offset)))

#define MASK_SET_1BIT(offset) (0x01 << (offset))
#define MASK_CLEAR_1BIT(offset) (~(0x01 << (offset)))



typedef enum{
	NORMAL,
	REVERSED
}GPIO_SEQUENCE;

//4bit output port
typedef struct
{
	volatile uint8_t *out;
	volatile uint8_t *dir;
	GPIO_SEQUENCE sequence;
	uint8_t pin0;
}GPIO_DOUT;

//4bit input port
typedef struct
{
	volatile uint8_t *pinctrl;
	volatile uint8_t *dir;
	volatile uint8_t *in;
	GPIO_SEQUENCE sequence;
	uint8_t pin0;
	
#ifdef BOARD_V11B
	volatile uint8_t *intflags;
	GPIO_DOUT shadowReg;			
#endif
	
}GPIO_DIN;

//Button
typedef struct
{
	volatile uint8_t *pinctrl;
	volatile uint8_t *dir;
	volatile uint8_t *in;
	uint8_t pin;
}GPIO_BTN;



void gpio_dout_init(GPIO_DOUT *aDout);
void gpio_dout_write(GPIO_DOUT *aDout, uint8_t aData);
void gpio_dout_writeBit(GPIO_DOUT *aDout, uint8_t aBit, uint8_t aBitNr);
void gpio_dout_toggleBit(GPIO_DOUT *aDout, uint8_t aBitNr);
uint8_t gpio_dout_read(GPIO_DOUT *aDout);

void gpio_din_init(GPIO_DIN *aDin);
uint8_t gpio_din_read(GPIO_DIN *aDin);
uint8_t gpio_din_readBit(GPIO_DIN *aDin, uint8_t aBitNr);

void gpio_btn_init(GPIO_BTN *aBtn);
bool gpio_btn_read(GPIO_BTN *aBtn);

#endif //GPIO_H