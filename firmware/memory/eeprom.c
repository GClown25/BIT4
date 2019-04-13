/*
 * eeprom.c
 *
 * Created: 08.02.2019
 * Author : Martin Frauenschuh
 */ 

#include "eeprom.h"

#include <avr/io.h>
#include <avr/eeprom.h>

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	eeprom_write_byte(uiAddress, ucData);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	return eeprom_read_byte(uiAddress);
}