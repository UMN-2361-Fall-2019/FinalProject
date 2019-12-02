/**
 * \file	C:\Source\FinalProject\final_WaterSystem_001.X\watersystem.c.
 *
 * \brief	Watersystem class
 */

#include "xc.h"
#include "display.h"
#include "motor.h"
#include "sensor.h"
#include "serial.h"
#include "defs.h"
#include "stdio.h"
#include "string.h"

loggerCallback loggerFunction = NULL;

/**
 * \fn	void LogMessage(char* message)
 *
 * \brief	Logs a message
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param [in,out]	message	If non-null, the message.
 */

void LogMessage(char* message)
{
    LOGMESSAGE(loggerFunction, message);
}

/**
 * \fn	int initWaterSystem(void)
 *
 * \brief	Initializes the water system
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	An int.
 */

int initWaterSystem(void) {
    // loggerCallback loggerFunction = NULL;
    int retVal = 0;
    retVal = initSerial();
    if(retVal != 0){
        loggerFunction = (loggerCallback)getLogger();
    }
    LogMessage("Initializing Sensor");
    retVal |= initSensor(loggerFunction)<<1;
    LogMessage("Initializing Motor control");
    retVal |= initMotor(loggerFunction)<<2;
    LogMessage("Initializing Display");
    retVal |= initDisplay(loggerFunction) << 3;
    if(retVal == 0b1111){
        LogMessage("Initialization completed successfully");
    }
    else{
        char output[100];
        sprintf(output,"Error #0x%02X initializing system",retVal);
        LogMessage(output);
    }
    
    return retVal;
}

/**
 * \fn	int ProcessMessages()
 *
 * \brief	This is the eternal pump for the Watersystem, can be used for the eternal loop
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	An int.
 */

int ProcessMessages(){
    processMotorMessages();
    processSensorMessages();
    processDisplayMessages();
    processSerialMessages();
    return 1;
}