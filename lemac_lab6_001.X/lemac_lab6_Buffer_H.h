/* 
 * File:   lemac_lab6_Buffer_H.h
 * Author: Alex
 *
 * Created on November 13, 2019, 2:46 AM
 */

#ifndef LEMAC_LAB6_BUFFER_H_H
#define	LEMAC_LAB6_BUFFER_H_H

#ifdef	__cplusplus
extern "C" {
#endif

void putVal(int newValue); // add a new value to the buffer
int getAvg();           // average all buffer vals
void initBuffer(); // set all buffer vals to zero
//volatile int currentZ; 

#ifdef	__cplusplus
}
#endif

#endif	/* LEMAC_LAB6_BUFFER_H_H */

