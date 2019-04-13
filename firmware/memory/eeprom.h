/*
 * eeprom.h
 *
 * Created: 08.02.2019
 * Author : Martin Frauenschuh
 */ 

#ifndef EEPROM_H
#define EEPROM_H

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);


#endif //EEPROM_H