/*
 * datalog.h
 *
 */

#ifndef DATALOG_H_
#define DATALOG_H_

typedef struct datalog_interval_type {

	unsigned int temp_interval_seconds;
	unsigned int numberOfLogs;

}datalog_interval_type;

void datalog_Init();
void data_buffer(int data1, int data2);
void datalog_cleanbuffer();

#endif /* DATALOG_H_ */
