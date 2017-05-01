/*
 * MotorDriver.h
 *
 *  Created on: May 1, 2017
 *      Author: sharatrp
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#define EM0								0
#define EM1								1
#define EM2								2
#define EM3								3
#define EM4								4
#define energyMode						EM2
#define selfcalibrate 					1
#define LFXOfreq 						32768
#define ULFRCOfreq 						1000
#define exciteOff 						0.04
#define exciteOn 						1.5
#define LETIMER0_Max_Count 				65536
float LFAselfcal;
int flag;
static int sleep_block_counter[4] 		= {0,0,0,0};

#define LED0Port 						gpioPortF
#define LED0Pin 						4
#define LED1Port 						gpioPortF
#define LED1Pin 						5


#endif /* MOTORDRIVER_H_ */
