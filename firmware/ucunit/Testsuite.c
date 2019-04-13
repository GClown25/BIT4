/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : Testsuite.h                                                *
 *  Description : Sample testsuite                                           *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */

#include "System.h"
#include "uCUnit-v1.0.h"
#include "Testsuite.h"

#include <avr/interrupt.h>
#include "memory/eeprom.h"

static void Test_eeprom_01(void){
	uint8_t data1, data2;
	
    UCUNIT_TestcaseBegin("EEPROM: check write and read");
   
	cli();
   	EEPROM_write(0x0000, 0xAB );
   	EEPROM_write(0x07ff, 0xCD );
   	
   	data1 = EEPROM_read(0x0000);
   	data2 = EEPROM_read(0x07ff);	//dec2047
    sei();
	
	UCUNIT_CheckIsEqual(0xAB, data1);
	UCUNIT_CheckIsEqual(0xCD, data2);
	
    UCUNIT_TestcaseEnd();
}



static void Test_tps_01(void){
	uint8_t instruction1, command1, data1;	
	instruction1 = 0x3b;
	
	 UCUNIT_TestcaseBegin("TPS: tps_splitInstruction");
	
	 tps_splitInstruction(instruction1, &command1, &data1);
	 
	 UCUNIT_CheckIsEqual(0x03, command1);
	 UCUNIT_CheckIsEqual(0x0b, data1);
	 
	 UCUNIT_TestcaseEnd();
	 
}

static void Test_tps_02(void){
	uint8_t instruction1, command1, data1;
	command1 = 0x0a;
	data1 = 0x0e;
	
	UCUNIT_TestcaseBegin("TPS: tps_unifyInstruction");
	
	tps_unifyInstruction(&instruction1, command1, data1);
	
	UCUNIT_CheckIsEqual(0xae, instruction1);

	
	UCUNIT_TestcaseEnd();
}



static void Test_tps_03(void){
	uint8_t term1_1 = 9, term2_1 = 0, sum_1;
	uint8_t term1_2 = 11, term2_2 = 4, sum_2;
	uint8_t term1_3 = 15, term2_3 = 15, sum_3;
	uint8_t term1_4 = 15, term2_4 = 1, sum_4;
	
	UCUNIT_TestcaseBegin("TPS: tps_add");
	
	sum_1 = tps_add(term1_1, term2_1);
	sum_2 = tps_add(term1_2, term2_2);
	sum_3 = tps_add(term1_3, term2_3);
	sum_4 = tps_add(term1_4, term2_4);
	
	
	UCUNIT_CheckIsEqual(9, sum_1);
	UCUNIT_CheckIsEqual(15, sum_2);
	UCUNIT_CheckIsEqual(14, sum_3);
	UCUNIT_CheckIsEqual(0, sum_4);

	
	UCUNIT_TestcaseEnd();
}


static void Test_tps_04(void){
	uint8_t a_1 = 15, b_1 = 11, c_1;
	uint8_t a_2 = 15, b_2 = 15, c_2;
	uint8_t a_3 = 0, b_3 = 1, c_3;
	uint8_t a_4 = 0, b_4 = 15, c_4;
	
	UCUNIT_TestcaseBegin("TPS: tps_subtract");
	
	c_1 = tps_subtract(a_1, b_1);
	c_2 = tps_subtract(a_2, b_2);
	c_3 = tps_subtract(a_3, b_3);
	c_4 = tps_subtract(a_4, b_4);
	
	
	UCUNIT_CheckIsEqual(4, c_1);
	UCUNIT_CheckIsEqual(0, c_2);
	UCUNIT_CheckIsEqual(15, c_3);
	UCUNIT_CheckIsEqual(1, c_4);

	
	UCUNIT_TestcaseEnd();
}

static void Test_tps_05(void){
	uint8_t a_1 = 3, b_1 = 4, c_1;
	uint8_t a_2 = 8, b_2 = 2, c_2;
	uint8_t a_3 = 8, b_3 = 3, c_3;
	uint8_t a_4 = 15, b_4 = 15, c_4;
	
	UCUNIT_TestcaseBegin("TPS: tps_multiply");
	
	c_1 = tps_multiply(a_1, b_1);
	c_2 = tps_multiply(a_2, b_2);
	c_3 = tps_multiply(a_3, b_3);
	c_4 = tps_multiply(a_4, b_4);
	
	
	UCUNIT_CheckIsEqual(12, c_1);
	UCUNIT_CheckIsEqual(0, c_2);
	UCUNIT_CheckIsEqual(8, c_3);
	UCUNIT_CheckIsEqual(1, c_4);

	
	UCUNIT_TestcaseEnd();
}


static void Test_tps_06(void){
	uint8_t a_1 = 1, b_1 = 0, c_1;
	uint8_t a_2 = 15, b_2 = 3, c_2;
	uint8_t a_3 = 15, b_3 = 1, c_3;
	
	UCUNIT_TestcaseBegin("TPS: tps_divide");
	
	c_1 = tps_divide(a_1, b_1);
	c_2 = tps_divide(a_2, b_2);
	c_3 = tps_divide(a_3, b_3);
	
	
	UCUNIT_CheckIsEqual(15, c_1);
	UCUNIT_CheckIsEqual(5, c_2);
	UCUNIT_CheckIsEqual(15, c_3);

	
	UCUNIT_TestcaseEnd();
}


static void Test_tps_07(void){
	uint8_t a_1 = 10, b_1 = 1, c_1;
	uint8_t a_2 = 15, b_2 = 0, c_2;
	uint8_t a_3 = 15, b_3 = 4, c_3;
	
	UCUNIT_TestcaseBegin("TPS: tps_shiftLeft");
	
	c_1 = tps_shiftLeft(a_1, b_1);
	c_2 = tps_shiftLeft(a_2, b_2);
	c_3 = tps_shiftLeft(a_3, b_3);
	
	UCUNIT_CheckIsEqual(4, c_1);
	UCUNIT_CheckIsEqual(15, c_2);
	UCUNIT_CheckIsEqual(0, c_3);

	
	UCUNIT_TestcaseEnd();
}

static void Test_tps_08(void){
	uint8_t a_1 = 10, b_1 = 1, c_1;
	uint8_t a_2 = 15, b_2 = 0, c_2;
	uint8_t a_3 = 15, b_3 = 4, c_3;
	
	UCUNIT_TestcaseBegin("TPS: tps_shiftRight");
	
	c_1 = tps_shiftRight(a_1, b_1);
	c_2 = tps_shiftRight(a_2, b_2);
	c_3 = tps_shiftRight(a_3, b_3);
	
	UCUNIT_CheckIsEqual(5, c_1);
	UCUNIT_CheckIsEqual(15, c_2);
	UCUNIT_CheckIsEqual(0, c_3);

	
	UCUNIT_TestcaseEnd();
}

static void Test_tps_09(void){
	uint8_t a_1 = 3, b_1 = 3, c_1;
	uint8_t a_2 = 0, b_2 = 0, c_2;
	uint8_t a_3 = 4, b_3 = 0, c_3;
	uint8_t a_4 = 0, b_4 = 15, c_4;
	
	UCUNIT_TestcaseBegin("TPS: tps_mod");
	
	c_1 = tps_shiftRight(a_1, b_1);
	c_2 = tps_shiftRight(a_2, b_2);
	c_3 = tps_shiftRight(a_3, b_3);
	c_4 = tps_shiftRight(a_4, b_4);
	
	UCUNIT_CheckIsEqual(0, c_1);
	UCUNIT_CheckIsEqual(0, c_2);
	UCUNIT_CheckIsEqual(4, c_3);
	UCUNIT_CheckIsEqual(0, c_4);


	UCUNIT_TestcaseEnd();
}


void Testsuite_RunTests(void)
{
	//Test_eeprom_01();
	Test_tps_01();
	Test_tps_02();
	Test_tps_03();
	Test_tps_04();
	Test_tps_05();
	Test_tps_06();
	Test_tps_07();
	Test_tps_08();
	Test_tps_09();

    UCUNIT_WriteSummary();
}

int main(void)
{
    UCUNIT_Init();
	
    UCUNIT_WriteString("\n**************************************");
    UCUNIT_WriteString("\nName:     ");
    UCUNIT_WriteString("TPS Project UNIT Test");
    UCUNIT_WriteString("\nCompiled: ");
    UCUNIT_WriteString(__DATE__);
    UCUNIT_WriteString("\nTime:     ");
    UCUNIT_WriteString(__TIME__);
    UCUNIT_WriteString("\nVersion:  ");
    UCUNIT_WriteString(UCUNIT_VERSION);
    UCUNIT_WriteString("\n**************************************");
    Testsuite_RunTests();
    UCUNIT_Shutdown();
	
    return 0;
}
