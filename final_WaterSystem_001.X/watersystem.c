/*
 * File:   watersystem.c
 * Author: rfolk
 *
 * Created on November 28, 2019, 2:36 PM
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

void LogMessage(char* message)
{
    LOGMESSAGE(loggerFunction, message);
}

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

// This is the eternal pump for the Watersystem, 
// can be used for the eternal loop
int ProcessMessages(){
    processMotorMessages();
    processSensorMessages();
    processDisplayMessages();
    processSerialMessages();
    return 1;
}