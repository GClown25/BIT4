/*
* main.c
*
* Created: 09.02.2019
* Author : GClown25
*/

#define F_CPU 5000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "core/tps.h"
#include "memory/eeprom.h"
#include "peripheral/gpio.h"
#include "peripheral/timer.h"
#include "peripheral/adc.h"
#include "common/randomSeed.h"

#define MEMORY_SIZE 256

GPIO_BTN mcpStat;

GPIO_DOUT doutA, doutB;
GPIO_DIN dinA;
GPIO_BTN btn1, btn2;

#define PWM1 TCB0
#define PWM2 TCB1

uint16_t progCounter = 0;
uint8_t page = 0;
uint8_t retVector = 0;
uint8_t instruction, command, data;
uint8_t buffer[256];

int8_t varA = 0, varB = 0, varC = 0, varD = 0;

uint8_t ram[16] = {0};
uint8_t ramAddr = 0;

//Lookup Table for Wait command
const uint16_t waitCounterNr[] = {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 30000, 60000};

void programmInstruction();
void  command0();
void  command1();
void  command2();
void  command3();
void  command4();
void  command5();
void  command6();
void  command7();
void  command8();
void  command9();
void  commandA();
void  commandB();
void  commandC();
void  commandD();
void  commandE();
void  commandF();

int main(void){
	
	//Clock init
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSC20M_gc;
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_4X_gc | CLKCTRL_PEN_bm;
	
	//turn on POWER incicator led
	PORTC.DIRSET = PIN1_bm;
	PORTC.OUTSET = PIN1_bm;
	
	//Init MCP73831 charging indicator led
	PORTC.DIRSET = PIN2_bm;
	
	//Init MCP73831 Charge status sensing
	mcpStat.in = &PORTC.IN;
	mcpStat.dir = &PORTC.DIR;
	mcpStat.pinctrl = &PORTC.PIN0CTRL;
	mcpStat.pin = PIN0_bp;
	gpio_btn_init(&mcpStat);
	
	CPUINT.CTRLA |= CPUINT_LVL0RR_bm; // Set the Round-robin Scheduling Enable bit
	CPUINT.LVL0PRI = PORTC_PORT_vect_num; //Set Interrupt priority
	PORTC.PIN0CTRL |= 0x01;	//Enable interrupt on both edges
	
	if(gpio_btn_read(&mcpStat))
		PORTC.OUTCLR = PIN2_bm;
	else
		PORTC.OUTSET = PIN2_bm;
	
	sei();
	
	
	
	doutA.out = &PORTA.OUT;
	doutA.dir = &PORTA.DIR;
	doutA.pin0 = PIN4_bp;
	doutA.sequence = NORMAL;
	gpio_dout_init(&doutA);
	
	doutB.out = &PORTB.OUT;
	doutB.dir = &PORTB.DIR;
	doutB.pin0 = PIN0_bp;
	doutB.sequence = NORMAL;
	gpio_dout_init(&doutB);
	
	btn1.in = &PORTD.IN;
	btn1.dir = &PORTD.DIR;
	btn1.pinctrl = &PORTD.PIN0CTRL;
	btn1.pin = PIN0_bp;
	gpio_btn_init(&btn1);
	
	btn2.in = &PORTD.IN;
	btn2.dir = &PORTD.DIR;
	btn2.pinctrl = &PORTD.PIN5CTRL;
	btn2.pin = PIN5_bp;
	gpio_btn_init(&btn2);
	
	dinA.in = &PORTD.IN;
	dinA.dir = &PORTD.DIR;
	dinA.pinctrl = &PORTD.PIN1CTRL;
	dinA.pin0 = PIN1_bp;
	dinA.sequence = REVERSED;
	gpio_din_init(&dinA);
	
	
	timerb_initPWM(&PWM1);
	timerb_initPWM(&PWM2);
	
	adcInit();
	srand(getSeed());
	adcDeInit();
	
	//Enter program mode
	if(gpio_btn_read(&btn2) == 0){
		
		//blink twice to visualize entering of prog mode
		gpio_dout_write(&doutA, 0xf);
		gpio_dout_write(&doutB, 0xf);
		_delay_ms(150);
		gpio_dout_write(&doutA, 0x0);
		gpio_dout_write(&doutB, 0x0);
		_delay_ms(150);
		gpio_dout_write(&doutA, 0xf);
		gpio_dout_write(&doutB, 0xf);
		_delay_ms(150);
		gpio_dout_write(&doutA, 0x0);
		gpio_dout_write(&doutB, 0x0);
		_delay_ms(150);
		
		while(progCounter < MEMORY_SIZE){
			programmInstruction();
			progCounter++;
		}
	
		
	//Enter intepretor mode
	}else{
		
		//Load instructions in buffer
		for(int i = 0; i < MEMORY_SIZE; i++){
			buffer[i] = EEPROM_read(i);
		}
		
		while(progCounter < MEMORY_SIZE){	
			tps_splitInstruction(buffer[progCounter], &command, &data);
			
			switch(command){
				case 0x0:
					command0();
				break;	
				case 0x1:
					command1();
				break;	
				case 0x2:
 					command2();
				break;				
				case 0x3: 
					command3();
				break;				
				case 0x4:
					command4();
				break;				
				case 0x5:
					command5();
				break;
				case 0x6:
					command6();
				break;
				case 0x7:
					command7();
				break;
				case 0x8:
					command8();
				break;
				case 0x9:
					command9();
				break;
				case 0xA:
					commandA();
				break;
				case 0xB:
					commandB();
				break;
				case 0xC:
					commandC();
				break;
				case 0xD:
					commandD();
				break;
				case 0xE:
					commandE();
				break;
				case 0xF:
					commandF();
				break;
			}	
		}
		
	//Go in Safestate
		
	}
	
}


//load const val to doutB port
void command0(){
	gpio_dout_write(&doutB, data);
	progCounter++;
}

//load const val to doutA port
void command1(){
	gpio_dout_write(&doutA, data);
	progCounter++;
}

//delay
void command2(){
	for(uint16_t i = 1; i <= waitCounterNr[data]; i++)
	_delay_ms(1);
	progCounter++;
}

//relative jump back
void command3(){
	if(progCounter >= data )
	progCounter -= data;
	else
	progCounter = 0;
}

//load const val to A
void command4(){
	varA = data;
	progCounter++;
}

//load smth with the val of A
void command5(){
	switch(data){
		case 0x0:
			ram[ramAddr] = varA;
		break;
		case 0x1:
			varB = varA;
		break;		
		case 0x2:
			varC = varA;
		break;		
		case 0x3:
			varD = varA;
		break;		
		case 0x4:
			gpio_dout_write(&doutA, varA);
		break;		
		case 0x5:
			gpio_dout_writeBit(&doutA, varA, 0);
		break;	
		case 0x6:
			gpio_dout_writeBit(&doutA, varA, 1);
		break;		
		case 0x7:
			gpio_dout_writeBit(&doutA, varA, 2);
		break;	
		case 0x8:
			gpio_dout_writeBit(&doutA, varA, 3);
		break;	
		case 0x9:
			timerb_setDC(&PWM1, varA);
		break;
		case 0xA:
			timerb_setDC(&PWM2, varA);
		break;
		case 0xB:
			gpio_dout_write(&doutB, varA);
		break;
		case 0xC:
			gpio_dout_writeBit(&doutB, varA, 0);
		break;
		case 0xD:
			gpio_dout_writeBit(&doutB, varA, 1);
		break;
		case 0xE:
			gpio_dout_writeBit(&doutB, varA, 2);
		break;
		case 0xF:
			gpio_dout_writeBit(&doutB, varA, 3);
		break;
	}
	progCounter++;
}

//load smth into A
void command6(){
	switch(data){
		case 0x1:
			varA = varB;
		break;
		case 0x2:
			varA = varC;
		break;
		case 0x3:
			varA = varD;
		break;
		case 0x4:
			varA = gpio_din_read(&dinA);
		break;
		case 0x5:
			varA = gpio_din_readBit(&dinA, 0);
		break;
		case 0x6:
			varA = gpio_din_readBit(&dinA, 1);
		break;
		case 0x7:
			varA = gpio_din_readBit(&dinA, 2);
		break;
		case 0x8:
			varA = gpio_din_readBit(&dinA, 3);
		break;
		case 0xB:
			varA = rand() % 16;
		break;
		case 0xC:
			varA = gpio_dout_read(&doutA);
		break;
		case 0xD:
			varA = gpio_dout_read(&doutB);
		break;
		case 0xE:
			varA = ram[ramAddr] & 0xf;
		break;
		case 0xF:
			varA = ram[varB] & 0xf;
		break;
	}
	progCounter++;
}

//Do calculations with A (and B)
void command7(){
switch(data){
	case 0x1:
		if(varA < 15)
			varA++;
		else
			varA = 0;
	break;
	case 0x2:
		if(varA > 0)
			varA--;
		else
			varA = 15;
	break;
	case 0x3:
		varA = tps_add(varA, varB);
	break;
	case 0x4:
		varA = tps_subtract(varA, varB);
	break;
	case 0x5:
		varA = tps_multiply(varA, varB);
	break;
	case 0x6:
		varA = tps_divide(varA, varB);
	break;
	case 0x7:
		varA = (varA & varB);
	break;
	case 0x8:
		varA = (varA | varB);
	break;
	case 0x9:
		varA = (varA ^ varB) & 0xf;
	break;
	case 0xA:
		varA = (~varA) & 0xf;
	break;
	case 0xB:
		varA = tps_mod(varA, varB);
	break;
	case 0xC:
		varA = tps_shiftLeft(varA, 1);
	break;
	case 0xD:
		varA = tps_shiftRight(varA, 1);
	break;
	case 0xE:
		varA = tps_shiftLeft(varA, varB);
	break;
	case 0xF:
		varA = tps_shiftRight(varA, varB);
	break;
}
progCounter++;
}

//Set page for absolute jumps
void command8(){
	page = data;
	progCounter++;
}

//Do an absolute jump
void command9(){
	progCounter = (page << 4) | data;
}

//Do an absolute jump C times 
void commandA(){
	if(varC > 0){
		varC--;
		progCounter = (page << 4) | data;
	}else{
		progCounter++;
	}
}

//Do an absolute jump D times 
void commandB(){
	if(varD > 0){
		varD--;
		progCounter = (page << 4) | data;
	}else{
		progCounter++;
	}
}

//Skip if
void commandC(){
	switch(data){
		case 0x1:
		if(varA > varB)
			progCounter++;
		break;
		case 0x2:
		if(varA < varB)
			progCounter++;
		break;
		case 0x3:
		if(varA == varB)
			progCounter++;
		break;
		case 0x4:
		if(varA != varB)
			progCounter++;
		break;
		case 0x5:
		if(varA == 0)
			progCounter++;		
		break;
		case 0x6:
		if(varA != 0)
			progCounter++;		
		break;
		case 0x7:
		break;
		case 0x8:
		if(gpio_din_readBit(&dinA, 0) == 0)
			progCounter++;		
		break;
		case 0x9:
		if(gpio_din_readBit(&dinA, 1) == 0)
			progCounter++;
		break;
		case 0xA:
		if(gpio_din_readBit(&dinA, 2) == 0)
			progCounter++;
		break;
		case 0xB:
		if(gpio_din_readBit(&dinA, 3) == 0)
			progCounter++;
		break;
		case 0xC:
		if(gpio_btn_read(&btn1) == 0)
			progCounter++;
		break;
		case 0xD:
		if(gpio_btn_read(&btn2) == 0)
			progCounter++;
		break;
		case 0xE:
		if(gpio_btn_read(&btn1) == 1)
			progCounter++;
		break;
		case 0xF:
		if(gpio_btn_read(&btn2) == 1)
			progCounter++;
		break;
	}
	progCounter++;
}

//Call
void commandD(){
	retVector = progCounter;
	progCounter = (page << 4) | data;
}

//Return
void commandE(){
	progCounter = retVector + 1;
}

//Set the RAM r/w Address
void commandF(){
	ramAddr = data;
	progCounter++;
}


void programmInstruction(){
	bool lockCommand = false, lockData = false;
	
	instruction = EEPROM_read(progCounter);
	tps_splitInstruction(instruction, &command, &data);
	
	//************************************************************************************************* Show adress
	gpio_dout_write(&doutA, 0x00);
	gpio_dout_write(&doutB, 0x00);
	_delay_ms(100);
	gpio_dout_write(&doutA, progCounter);		//display low nibble of current address for some ms
	gpio_dout_write(&doutB, progCounter>>4);	//display high nibble of current address for some ms
	_delay_ms(300);
	gpio_dout_write(&doutA, 0x00);
	gpio_dout_write(&doutB, 0x00);
	_delay_ms(100);
	
	//************************************************************************************************* Show and modify command	
	gpio_dout_write(&doutB, command);	//display COMMAND of current address
	
	while(gpio_btn_read(&btn2) == 0);	//wait till button2 released
	_delay_ms(50);	//Debounce

	while(gpio_btn_read(&btn2) == 1){	//while button2 is released
		
		if(gpio_btn_read(&btn1) == 0){	//if button1 is pressed: if it's the first time set command to 0, otherwise increment it
			if(lockCommand == false){
				command = 0;
				lockCommand = true;
				}else{
				command++;
			}
			gpio_dout_write(&doutB, command);
			_delay_ms(50);	//Debounce
			while(gpio_btn_read(&btn1) == 0);	//wait till button1 is released
			_delay_ms(50);	//Debounce
		}
		
	}
	gpio_dout_write(&doutB, 0x00);
		
	//************************************************************************************************* Show and modify data
	gpio_dout_write(&doutA, data);	//display DATA of current address
	
	_delay_ms(50);	//Debounce
	while(gpio_btn_read(&btn2) == 0);	//wait till button2 released
	_delay_ms(50);	//Debounce
	
	while(gpio_btn_read(&btn2) == 1){	//while button2 is released
		
		if(gpio_btn_read(&btn1) == 0){	//if button1 is pressed: if it's the first time set data to 0, otherwise increment it
			if(lockData == false){
				data = 0;
				lockData = true;
				}else{
				data++;
			}
			gpio_dout_write(&doutA, data);
			_delay_ms(50);	//Debounce
			while(gpio_btn_read(&btn1) == 0);	//wait till button1 is released
			_delay_ms(50);	//Debounce
		}
		
	}
	gpio_dout_write(&doutA, 0x00);

	//************************************************************************************************* Optional: write to EEPROM
	if(lockCommand || lockData){
		tps_unifyInstruction(&instruction, command, data);
		EEPROM_write(progCounter, instruction);
		
		gpio_dout_write(&doutA, 0x00);
		gpio_dout_write(&doutB, 0x00);
		_delay_ms(700);	
	}


}


//Init MCP73831 Charge status sensing
ISR(PORTC_PORT_vect){
	if(gpio_btn_read(&mcpStat))
		PORTC.OUTCLR = PIN2_bm;
	else
		PORTC.OUTSET = PIN2_bm;	
		
	PORTC.INTFLAGS = PIN0_bm;
}