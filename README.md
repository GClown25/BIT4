# BIT4
BIT4 is a 4Bit microcontroller - fully programmable with only three buttons.

![pinout](hardware/v1.0/pinout.png)

# Programming guide:

The BIT4 microcontroller has 256 Bytes of programm memory, split in 16 pages with each 16 instructions.
A instruction is 8bit wide and consists of an command and data:
\
![instruction](img/instruction.png)
\
To enter the programming mode, hold BTN2 down, press the RESET button once and then release BTN2.
In programming mode, step through the instructions with BTN2. When going to a new instruction, 
the leds will show the current address for a fraktion of a second.
With BTN1 the value of the command and data of the current instruction can be changed.
When stepping to a new address with BTN2, the instuction of the address above gets saved in the EEPROM.
At the end, press RESET to leave the programming mode and to start the programm execution.

## Instruction set explained:

- command 0x0: load a constant value to doutB
- command 0x1: load a constant value to doutA
- command 0x2: delay
- command 0x3: do a relative jump back
- command 0x4: load constant value to variable A
- command 0x5: load something with the value of variable A
- command 0x6: load the value of something into the variable A
- command 0x7: do arithmetic and logic operations with variable A (and B)
- command 0x8: set the high nibble of the address to jump to in absolute jump instructions
- command 0x9: do an absolute jump to the specified address
- command 0xA, 0xB: acts like a for loop. Every time this command is executed,
  if the variable C (or D) is greater than zero, an absolute jump to the specified address is done.
  Then the variable C (or D) gets decremented. 
- command 0xC: skip the next instruction if the argument is true
- command 0xD: call a function at the specified address
- command 0xE: return from the function call
- command 0xF: sets the address of the virtual 4bit RAM. 
  The value stored in this address can be read with 0x6e and written with 0x50.

## Examples:


**Blinki:**

| address | instruction |                    comment |
| :------ | :---------- | -------------------------: |
| 0       | 1f          |  turn all leds on doutA on |
| 1       | 28          |             wait for 500ms |
| 2       | 10          | turn all leds on doutB off |
| 3       | 28          |             wait for 500ms |
| 4       | 34          |           relative jump -4 |

\
**Binary counter:**
\
https://www.youtube.com/watch?v=6CpZszKlP7Y

| address | instruction |          comment |
| :------ | :---------- | ---------------: |
| 0       | 5B          |        DoutB = A |
| 1       | 59          |         PWM1 = A |
| 2       | 71          |              A++ |
| 3       | 28          |   wait for 500ms |
| 4       | 34          | relative jump -4 |

\
**Blinki with adjustable speed:**
\
https://www.youtube.com/watch?v=YPoedGGZs4g

| address   | instruction |               comment |
| :-------- | :---------- | --------------------: |
| **main:** |
| 0         | 10          |           DoutA = 0x0 |
| 1         | d5          |           call myWait |
| 2         | 1f          |           DoutA = 0xf |
| 3         | d5          |           call myWait |
| 4         | 34          |      relative jump -4 |
| **myWait:** |
| 5         | 64          |               A = Din |
| 6         | 52          |                 C = A |
| 7         | 25          |         wait for 50ms |
| 8         | a7          | for(C > 0; C--) jmp 7 |
| 9         | e0          |                return |

\
**Credits:**\
The idea for this project came from the german microcontroller learning plattform "TPS" (http://elektronik-labor.de/Lernpakete/TPS/TPS0.html).
