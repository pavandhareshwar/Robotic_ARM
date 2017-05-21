/*
 * MotorDriver.h
 *
 *  Created on: May 1, 2017
 *      Author: sharatrp
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include "stdio.h"
#include "stdint.h"

#define EM0								0
#define EM1								1
#define EM2								2
#define EM3								3
#define EM4								4
#define energyMode						EM2
#define selfcalibrate 					0
#define LFXOfreq 						32768
#define ULFRCOfreq 						1000
#define exciteOff 						0.04
#define exciteOn 						1.5
#define LETIMER0_Max_Count 				65536
float LFAselfcal;
int flag;
static int sleep_block_counter[4] 		= {0,0,0,0};

#define LEDPort 						gpioPortF
#define LED0Pin 						4
#define LED1Pin 						5
#define MotorPort						gpioPortA
#define Motor0Pin0						0						/*Positive of Motor0*/
#define Motor0Pin1						1						/*Negative of Motor0*/
#define Motor1Pin0						2						/*Positive of Motor1*/
#define Motor1Pin1						3						/*Negative of Motor1*/

void parse_flex_sensor_data(uint32_t value);

void motor_control(void);

void ULFRCOCalibrate(void);

#endif /* MOTORDRIVER_H_ */
