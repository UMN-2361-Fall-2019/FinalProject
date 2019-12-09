/**
 * \file	C:\Source\FinalProject\final_WaterSystem_001.X\serial.c.
 *
 * \brief	Serial class
 */

#include "xc.h"

/**
 * \fn	static int putChar(int c)
 *
 * \brief	Puts a character
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param	c	An int to process.
 *
 * \return	An int.
 */

static int putChar(int c){
    // while(U1CTS);
    while(U1STAbits.UTXBF);
    U1TXREG = c;
    return c;
}

/**
 * \fn	static void Log(char* message)
 *
 * \brief	Logs the given message
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param [in,out]	message	If non-null, the message.
 */

static void Log(char* message){
    // Push string into log buffer for transmit
    while(putChar(*message++));
    putChar('\r');
    putChar('\n');
}

/**
 * \fn	int initSerial(void)
 *
 * \brief	Initializes the serial
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	An int.
 */

int initSerial(void) {
    U1BRG=103; //Set Baudrate - set for 9600
    IPC3bits.U1TXIP2 = 1; //Set Uart TX Interrupt Priority
    IPC3bits.U1TXIP1 = 0;
    IPC3bits.U1TXIP0 = 0;
    IPC2bits.U1RXIP2 = 1; //Set Uart RX Interrupt Priority
    IPC2bits.U1RXIP1 = 0;
    IPC2bits.U1RXIP0 = 0;
    U1STA = 0;
    U1STAbits.ADDEN = 1; //Address detect enabled
    U1MODE = 0x8080; //Enable Uart for 8-bit data
    //no parity,1 STOP bit,wake enabled
    U1STAbits.UTXEN = 1; //Enable Transmit
    // IEC0bits.U1TXIE = 1; //Enable Transmit Interrupt
    // IEC0bits.U1RXIE = 1; //Enable Receive Interrupt
    // U1MODEbits.BRGH = 0;
    // U1MODEbits.UARTEN = 1;
    Log("Initialized Serial communication at 9600 baud");
    return 1;
}

/**
 * \fn	void* getLogger(void)
 *
 * \brief	Gets the logger
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	Null if it fails, else the logger.
 */

void* getLogger(void){
    return (void*)Log;
}

/**
 * \fn	int processSerialMessages()
 *
 * \brief	Process the serial messages - Nothing to do here
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	An int.
 */

int processSerialMessages(){
    // Nothing to do here
    return 1;
}