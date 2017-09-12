/*
 * myadc.h
 *
 *  Created on: Apr 21, 2017
 *      Author: kiran
 */

#include <msp430.h>

#ifndef MYADC_H_
#define MYADC_H_

#define AVGDATA 20
#define SAMPLES 200
//int inline MAXPH(int AVG,int SD);
//int inline MINPH(int AVG,int SD);

void myADCinit();
int takeSamples();

void  ADCstartConv();

void  ADCstopConv();

void setupADC1();
void setupADC2();



#endif /* MYADC_H_ */
