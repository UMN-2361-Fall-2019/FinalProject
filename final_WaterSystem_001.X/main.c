/**
 * \file	C:\Source\FinalProject\final_WaterSystem_001.X\main.c.
 *
 * \brief	Main class
 */

#include "xc.h"
#include "defs.h"
#include "watersystem.h"

#pragma config ICS = PGx1 // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clock switching is enabled,
                                // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

/**
 * \fn	void setup(void)
 *
 * \brief	Setups this object
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 */

void setup(void){
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9ffe; //all digital inputs and outputs
    TRISA = 0x0001;
 
    initWaterSystem();
}

/**
 * \fn	int main(void)
 *
 * \brief	Main entry-point for this application
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	Exit-code for the process - 0 for success, else an error code.
 */

int main(void) {
    setup();
    while(ProcessMessages());
    return 0;
}
