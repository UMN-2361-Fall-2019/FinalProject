/*
 * File:   lemac_lab6_Buffer.c
 * Author: Alex
 *
 * Created on November 13, 2019, 2:46 AM
 */


#include <stddef.h>

#include "xc.h"
#define BUFFER_SIZE 1024

volatile int theBuffer[BUFFER_SIZE];
volatile int front=0;
   
    

void putVal(int newValue)
{
    theBuffer[front++] = newValue;

    if(front == BUFFER_SIZE)
    { 
        front = 0;
    }

}
int getAvg()
{
    long avg = 0;
    int i=0;
    int offsetPos=front;
    int nonZeroVals=0;
    for(i=0;i<BUFFER_SIZE;i++)
    {
        if(theBuffer[offsetPos] != 0){
            nonZeroVals++;
            avg+=theBuffer[offsetPos];
        }
        if((++offsetPos)==BUFFER_SIZE)
            offsetPos = 0;
    }
    // Should never occur before the first 1k samples, but coded to.
    if(nonZeroVals==0)
        return 0;
    
//    currentZ=nonZeroVals;
    return (int)(avg/nonZeroVals);
    
    /*
        long sum = 0;
    long i;
    for(i = 0; i < BUF_SIZE; i++) {
        sum += buffer[i];
    }
    avg = sum / BUF_SIZE;
    return avg;
     */
}

void initBuffer()
{
    int i = 0;
//    front = 0;
    for( i = 0; i < BUFFER_SIZE; i++) 
    {
        theBuffer[i] = 0;
    }
    //for(counter = 0;counter<BUFFER_SIZE;theBuffer[counter++] = 0);
}



 
