/*
 * datalog.c
 *
 */

#include "datalog.h"
#include "myuart.h"
#include "stdint.h"

#define DATA_WIDTH 16

#define MAX_LOGS 500
#define CHECK_INTERVAL 1

#include <stdio.h>

//#define DEBUG 1

#pragma PERSISTENT (numOfLogsInFram)
extern unsigned int numOfLogsInFram = 0;

#pragma PERSISTENT (ui16nlenhold)
extern unsigned int ui16nlenhold = 0x000A;

#pragma PERSISTENT (ui16plenhold)
extern unsigned int ui16plenhold = 0x0003;

datalog_interval_type interval;

extern uint8_t FileTextE104[];

extern unsigned char check;

unsigned char bufferHold[DATA_WIDTH];
uint16_t log_counter = 0;

void datalog_Init(){
	interval.temp_interval_seconds = CHECK_INTERVAL;
	bufferHold[3] = ',';
	bufferHold[9] = ',';
	bufferHold[15] = 0x0A;
}


void data_buffer(int input_data, int output_data){

/////////////filling the data/////////////////////////
	int temp;

#ifdef DEBUG
			char str[30];
			sprintf(str, "\n\rvoltage: %d ", data);
			myuart_tx_string(str);
#endif

	//LOG THE ADC CONVERTED VALUE TO FRAM with negative sign if any
	if(input_data<0)	{
		bufferHold[4] = '-';
		input_data = input_data*(-1);
	}else
		bufferHold[4] = 0;

	if(output_data<0)	{
		bufferHold[10] = '-';
		output_data = output_data*(-1);
	}else
		bufferHold[10] = 0;

	//save log counter to FRAM
	log_counter++;
	if(log_counter == MAX_LOGS)
	{
		log_counter = 0;
	}
	temp = log_counter/100;
	bufferHold[0] = (char)temp + 48 ;
	temp = (log_counter%100)/10;
	bufferHold[1] = (char)temp + 48 ;
	temp = log_counter%10;
	bufferHold[2] = (char)temp + 48 ;

	//save input voltage to FRAM
	temp = input_data/1000;					//thousandths
	bufferHold[5] = (char)temp + 48 ;
	temp = (input_data%1000)/100;			//hundredths
	bufferHold[6] = (char)temp + 48;
	temp = (input_data%100)/10;				//tenths
	bufferHold[7] = (char)temp + 48 ;
	temp = input_data%10;					//ones
	bufferHold[8] = (char)temp + 48 ;

	//save output voltage to FRAM
	temp = output_data/1000;					//thousandths
	bufferHold[11] = (char)temp + 48 ;
	temp = (output_data%1000)/100;				//hundredths
	bufferHold[12] = (char)temp + 48;
	temp = (output_data%100)/10;				//tenths
	bufferHold[13] = (char)temp + 48 ;
	temp = output_data%10;						//ones
	bufferHold[14] = (char)temp + 48 ;

	if(ui16nlenhold <  DATA_WIDTH*MAX_LOGS){	//maximum data it can hold

		ui16nlenhold += DATA_WIDTH;

		temp = ui16nlenhold;
	/////setting up the length of the ndef record
		FileTextE104[1] = ( char) ui16nlenhold;
		temp >>=8;
		FileTextE104[0] = ( char) temp;
	/////setting up the length of the ndef payload
		ui16plenhold += DATA_WIDTH;
		temp = ui16plenhold;

		FileTextE104[7] = ( char) ui16plenhold;
		temp >>= 8;
		FileTextE104[6] = ( char) temp;

		for( temp = 0 ; temp < DATA_WIDTH ; temp++){
			FileTextE104[12 + temp + numOfLogsInFram*DATA_WIDTH] = bufferHold[temp];
		}

		numOfLogsInFram+=1;

	} else {
#ifdef DEBUG
		myuart_tx_string("\n\r.............Memory full..............\n\r");
#endif
		for(temp = 12 ;temp < MAX_LOGS*DATA_WIDTH - DATA_WIDTH ; temp++){
			FileTextE104[temp] = FileTextE104[temp + DATA_WIDTH];
		}

		for( temp = 0 ; temp < DATA_WIDTH ; temp++){
			FileTextE104[12 + temp + (numOfLogsInFram-1)*DATA_WIDTH] = bufferHold[temp];
		}
	}


#ifdef DEBUG

		sprintf(str,"\n\rTL=%d dumping all\n\r",numOfLogsInFram);
		myuart_tx_string(str);
//		for(temp = 0 ;temp < numOfLogsInFram*DATA_WIDTH;temp++){
//				myuart_tx_byte(FileTextE104[12+temp]);
//		}

#endif
}

void datalog_cleanbuffer()
{
	uint16_t temp;
	for(temp = 12 ;temp < MAX_LOGS*DATA_WIDTH - DATA_WIDTH ; temp++)
	{
		FileTextE104[temp] = 0;
	}
}

