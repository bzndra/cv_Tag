/*
 * myadc.c
 *
 *  Created on: Apr 21, 2017
 *      Author: kiran
 */
#include "myadc.h"
#include "myuart.h"
#include <stdio.h>
#define SAMPLES 200
#define MOVAVGSIZE 20
#define MOVARRSIZE SAMPLES-MOVAVGSIZE

char str[30];

float r , result;
int avghold[SAMPLES];
int movavghold[MOVARRSIZE];

void ADCstartConv(){
	ADC12CTL0 |= ADC12ENC | ADC12SC;
}

void ADCstopConv(){
	ADC12CTL0 &= ~(ADC12ENC | ADC12SC);
}

/*configure ADC inputs A12(3.0) and A13(3.1)*/
void setupADC2(){
    P3SEL1 |= BIT0 | BIT1;					// Configure ADC inputs A12 and A13
    P3SEL0 |= BIT0 | BIT1;

    ADC12MCTL0 &= 0xFFE0;
    ADC12MCTL0 |= ADC12INCH_12;				// ADC A12 AND 13 => ADCINCH_12
}

///*configure ADC inputs A14(3.2) and A15(3.3)*/
//void setupADC1(){
//    P3SEL1 |= BIT2 | BIT3;					// Configure ADC inputs A14 and A15
//    P3SEL0 |= BIT2 | BIT3;
//
//    ADC12MCTL0 &= 0xFFE0;
//    ADC12MCTL0 |= ADC12INCH_14;				// ADC A14 AND 15 => ADCINCH_14
//}

/*configure ADC inputs A8(4.0) and A9(4.1)*/
void setupADC1(){
    P4SEL1 |= BIT0 | BIT1;					// Configure ADC inputs A8 and A9
    P4SEL0 |= BIT0 | BIT1;

    ADC12MCTL0 &= 0xFFE0;
    ADC12MCTL0 |= ADC12INCH_8;				// ADC A8 AND 9 => ADCINCH_8
}
void myADCinit(){

	REFCTL0 = REFVSEL_2 | REFON;			//setting 2.5v as ref
	ADC12CTL0 = ADC12SHT0_5| ADC12ON;     // Sampling time, ADC12 on
	ADC12CTL1 = ADC12SHP | ADC12SSEL_2;     // Use sampling timer
	ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
	ADC12MCTL0 |=  ADC12VRSEL_1 | ADC12DIF;   // Channel2 ADC input select; Vref=AVCC
//	ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt
	//__bis_SR_register( GIE);     // LPM0, ADC12_ISR will force exit
	__delay_cycles(5000);

}

int takeSamples(){
	int x,y;
	long int sum=0;

//	for(x = 0; x < SAMPLES ;x++){
//		r = (result - 2048 - 0.5) *2500.0 /2048;
//		r+= 0.610352;
//		avghold[x] = (int)r;
//	}
	for( x = 0; x < MOVARRSIZE ;x++){
		for(y = x;y < x+MOVAVGSIZE;y++){
			sum+=avghold[y];
		}
		movavghold[x]=sum/(MOVAVGSIZE);
	//	sprintf(str,"%d,",movavghold[x]);
	//	myuart_tx_string(str);
		sum = 0;
	}
	sum = 0;
	for( x =0 ; x < MOVARRSIZE ; x++){
		sum+= movavghold[x];

	}
	//sprintf(str,"s%ld,",sum);
	//myuart_tx_string(str);

	return sum/(MOVARRSIZE);

}

