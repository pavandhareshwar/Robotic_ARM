/*
 * MotorDriver.c
 *
 *  Created on: May 1, 2017
 *      Author: sharatrp
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_letimer.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "em_core.h"
#include "MotorDriver.h"

uint8_t flex_sensor_data[4];
int8_t motor_out[2];

/******************************************************************
 * This routine sets the energy mode the board should operate on
 *
 * Input: None
 *
 * Global variables used:  sleep_block_counter[4]
 *
 * Return: None
 ******************************************************************/
void sleep(void)
{
	if (sleep_block_counter [0] > 0) {
		return; 											// Blocked everything below EM0, so just return
	}
	else if (sleep_block_counter [1] > 0) {
		EMU_EnterEM1(); 									// Blocked everything below EM1, enter EM1
	}
	else if (sleep_block_counter[2] > 0) {
		EMU_EnterEM2(true);									// Blocked everything below EM2, enter EM2
	}
	else if (sleep_block_counter[3] > 0) {
		EMU_EnterEM3(true); 								// Blocked everything below EM3, enter EM3
	}
	else{
		EMU_EnterEM3(true); 								// Nothing is blocked, enter EM4
	}
	return;
}

/******************************************************************
 * This routine blocks all the energy modes under minimumMode
 *
 * Input: minimumMode
 *
 * Global variables used:  sleep_block_counter[4]
 *
 * Return: None
 ******************************************************************/
void blockSleepMode(uint32_t minimumMode)
{
	//instead of INT_Disable;
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();

	sleep_block_counter[minimumMode]++;

	CORE_EXIT_ATOMIC();
	//instead of INT_Enable;
}

/******************************************************************
 * This routine unblocks all the energy modes under minimumMode
 *
 * Input: minimumMode
 *
 * Global variables used:  sleep_block_counter[4]
 *
 * Return: None
 ******************************************************************/
void unblockSleepMode(uint32_t minimumMode)

{
	//instead of INT_Disable();
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();

	if(sleep_block_counter[minimumMode]>0){
		sleep_block_counter[minimumMode]--;
	}

	CORE_EXIT_ATOMIC();
	//instead of INT_Enable();
}

/******************************************************************
 * This routine starts the timers and the LETIMER.
 *
 * Input: None
 *
 * Global variables used: None
 *
 * Return: None
 ******************************************************************/
void timersEnable(void)
{
	LETIMER_Enable(LETIMER0, true);
	TIMER_Enable(TIMER1, true);
	TIMER_Enable(TIMER0, true);
}

/******************************************************************
 * This routine stops the timers and the LETIMER.
 *
 * Input: None
 *
 * Global variables used: None
 *
 * Return: None
 ******************************************************************/
void timersDisable(void)
{
	LETIMER_Enable(LETIMER0, false);
	TIMER_Enable(TIMER1, false);
	TIMER_Enable(TIMER0, false);
}

/******************************************************************
 * This routine turns on the required LED(s).
 *
 * Input: LED_number
 *
 * Global variables used: LED0Port, LED0Pin, LED1Port, LED1Pin
 *
 * Return: None
 ******************************************************************/
void ledON(int LED_number)
{
	if(LED_number == 0) GPIO_PinOutSet(LED0Port, LED0Pin);
	else GPIO_PinOutSet(LED1Port, LED1Pin);
}

/******************************************************************
 * This routine turns off the required LED(s).
 *
 * Input: LED_number
 *
 * Global variables used: LED0Port, LED0Pin, LED1Port, LED1Pin
 *
 * Return: None
 ******************************************************************/
void ledOFF(int LED_number)
{
	if(LED_number == 0) GPIO_PinOutClear(LED0Port, LED0Pin);
	else GPIO_PinOutClear(LED1Port, LED1Pin);
}

/******************************************************************
 * This routine defines the prescaler for LETIMER
 *
 * Input: None
 *
 * Global variables used:  LFXOfreq, exciteOff
 *
 * Return: LETIMER0_prescaler
 ******************************************************************/
int prescaler(void)
{
	blockSleepMode(energyMode);
	int LETIMER0_prescaler;
	if(energyMode != 3)
	{
		int Desired_Period = LFXOfreq*exciteOff;
		LETIMER0_prescaler = 0;
		int temp = Desired_Period;
		int Prescaled_two_power = 1;
		while (temp > LETIMER0_Max_Count)
		{
			LETIMER0_prescaler++;
			Prescaled_two_power = Prescaled_two_power* 2;
			temp = Desired_Period/ Prescaled_two_power;
		}
	}
	else
	{
		LETIMER0_prescaler = 1;
	}
	unblockSleepMode(energyMode);
	return LETIMER0_prescaler;
}


void LETIMERsetup()
{
	blockSleepMode(2); 							//EM3 if ULFRCO(1kHz) is used
	int value0 = 0;
	int value1 = 0;
	int val = prescaler();
	int temp = val;
	while(temp!=1)
	{
		temp--;
		val=val*2;
	}

	//Initializing the LETIMER
	if(flag == 0)
	{
		const LETIMER_Init_TypeDef letimer0_init_params =
			{
					.enable         = false,                  	/* Don't start counting when init completed. */
					.debugRun       = false,                  	/* Counter shall not keep running during debug halt. */
#if defined(LETIMER_CTRL_RTCC0TEN)
					.rtcComp0Enable = false,                  	/* Don't start counting on RTC COMP0 match. */
					.rtcComp1Enable = false,                  	/* Don't start counting on RTC COMP1 match. */
#endif
					.comp0Top       = true,                   	/* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */
					.bufTop         = false,                  	/* Don't load COMP1 into COMP0 when REP0 reaches 0. */
					.out0Pol        = 0,                      	/* Idle value for output 0. */
					.out1Pol        = 0,                      	/* Idle value for output 1. */
					.ufoa0          = letimerUFOAPwm,         	/* PWM output on output 0 */
					.ufoa1          = letimerUFOANone,       	/* Pulse output on output 1*/
					.repMode        = letimerRepeatFree    		/* Count until stopped */
			};

		LETIMER_Init(LETIMER0,&letimer0_init_params);
	}
	else
	{

		const LETIMER_Init_TypeDef letimerInit =
		{
				.enable = false,
				.debugRun = false,
#if defined(LETIMER_CTRL_RTCC0TEN)
				.rtcComp0Enable = false,
				.rtcComp1Enable = false,
#endif
				.comp0Top = true,
				.bufTop = false,
				.out0Pol = 0,
				.out1Pol = 0,
				.ufoa0 = letimerUFOAPwm,
				.ufoa1 = letimerUFOANone,
				.repMode = letimerRepeatOneshot
		};
		LETIMER_Init(LETIMER0,&letimerInit);
	}


	if((energyMode == 0 )|(  energyMode == 1 )|( energyMode == 2))
	{
		value0 = ((float)LFXOfreq/(float)val)*((float)exciteOff);
		value1 = ((float)LFXOfreq/(float)val)*((float)exciteOn);

	}
	else //energyMode == 3
	{
		value0 = ((float)ULFRCOfreq)*((float)exciteOff);
		value1 = ((float)ULFRCOfreq)*((float)exciteOn);
	}


	LETIMER_CompareSet(LETIMER0, 0, value0); //4ms -> (32768/2)*4/1000 = 65.536
	LETIMER_CompareSet(LETIMER0, 1, value1);//2.5seconds -> (32768/2)*2.5 = 40910

	while(LETIMER0->SYNCBUSY == 1);
	if(flag ==0)
	{

		LETIMER0->IFC = LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1 | LETIMER_IFC_UF; //Clearing the LETIMER0 Interrupt Flag register
		LETIMER0->IEN = LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1; //Enabling COMP0,COMP1
	}
	else
	{
		//LETIMER_RepeatSet(LETIMER0, 0, 1);
	}
	unblockSleepMode(2);
}

/******************************************************************
 * This routine sets up the timers
 *
 * Input: None
 *
 * Global variables used:  None
 *
 * Return: None
 ******************************************************************/
void timersSetup()
{
	const TIMER_Init_TypeDef timer0Init =        // Setup Timer initialization
			{
			.enable = false,                  	// Start timer upon configuration
			.debugRun = false,  				// Keep timer running even on debug halt
			.prescale = timerPrescale1, 		// Use /1 prescaler...timer clock = HF clock = 1 MHz
			.clkSel = timerClkSelHFPerClk, 		// Set HF peripheral clock as clock source
			.fallAction = timerInputActionNone, // No action on falling edge
			.riseAction = timerInputActionNone, // No action on rising edge
			.mode = timerModeUp,              	// Use up-count mode
			.dmaClrAct = false,                 // Not using DMA
			.quadModeX4 = false,               	// Not using quad decoder
			.oneShot = false,          			// Using continuous, not one-shot
			.sync = false, 						// Synchronizing timer operation on of other timers
			};

	const TIMER_Init_TypeDef timer1Init =        // Setup Timer initialization
			{
			.enable = false,                  	// Start timer upon configuration
			.debugRun = false,  				// Keep timer running even on debug halt
			.prescale = timerPrescale1, 		// Use /1 prescaler...timer clock = HF clock = 1 MHz
			.clkSel = timerClkSelCascade, 		// Cascade the two timers
			.fallAction = timerInputActionNone, // No action on falling edge
			.riseAction = timerInputActionNone, // No action on rising edge
			.mode = timerModeUp,              	// Use up-count mode
			.dmaClrAct = false,                 // Not using DMA
			.quadModeX4 = false,               	// Not using quad decoder
			.oneShot = false,          			// Using continuous, not one-shot
			.sync = true, 						// Synchronizing timer operation on of other timers
			};
	TIMER_Init(TIMER0, &timer0Init);
	TIMER_Init(TIMER1, &timer1Init);
}


void ULFRCOCalibrate(void)
{
	flag = 1;
	int energymode =3;
	blockSleepMode(energymode);

	LETIMERsetup();
	timersSetup();
	TIMER_CounterSet(TIMER1,0);
	TIMER_CounterSet(TIMER0,0);

	LETIMER0->CNT = ULFRCOfreq;
	while(LETIMER0->SYNCBUSY == 1);

	timersEnable();
	while(LETIMER0->CNT != 0);
	timersDisable();

	unsigned int ULFRCOcount = (TIMER1->CNT<<16) | (TIMER0->CNT);

	TIMER_CounterSet(TIMER1,0);
	TIMER_CounterSet(TIMER0,0);

	CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false);
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	LETIMER0->CNT = LFXOfreq;
	while(LETIMER0->SYNCBUSY == 1);

	timersEnable();
	while(LETIMER0->CNT != 0);
	timersDisable();

	unsigned int LFXOcount = (TIMER1->CNT<<16) | (TIMER0->CNT);

	float OSCratio = (float)LFXOcount/(float)ULFRCOcount;

	LFAselfcal = OSCratio*1;
	unblockSleepMode(energymode);
	flag = 0;
	return;
}

void parse_flex_sensor_data(uint32_t value)
{
	uint8_t idx;
	int i, j;

	for(i = 24; i >= 0; i -= 8)
	{
		idx = (((value >> i) & 0xC0) >> 6);
		flex_sensor_data[idx] = ((value >> i) & 0xC0);
	}

	/* Mapping the flex sensor data received to a single value
	 * representing fixed range */
	for (j = 0; j < 4; j++)
	{
		if ((flex_sensor_data[j] >= 0) && (flex_sensor_data[j] <= 5))
			flex_sensor_data[j] = 3;
		else if ((flex_sensor_data[j] > 5) && (flex_sensor_data[j] <= 10))
			flex_sensor_data[j] = 2;
		else if ((flex_sensor_data[j] > 10) && (flex_sensor_data[j] <= 15))
			flex_sensor_data[j] = 1;
		else
			flex_sensor_data[j] = 0;
	}
}

void timerSetup(uint32_t topVal)
{
	const TIMER_Init_TypeDef timer0Init =        // Setup Timer initialization
			{
			.enable = false,                  	// Start timer upon configuration
			.debugRun = false,  				// Keep timer running even on debug halt
			.prescale = timerPrescale1, 		// Use /1 prescaler...timer clock = HF clock = 1 MHz
			.clkSel = timerClkSelHFPerClk, 		// Set HF peripheral clock as clock source
			.fallAction = timerInputActionNone, // No action on falling edge
			.riseAction = timerInputActionNone, // No action on rising edge
			.mode = timerModeUp,              	// Use up-count mode
			.dmaClrAct = false,                 // Not using DMA
			.quadModeX4 = false,               	// Not using quad decoder
			.oneShot = true,          			// Using continuous, not one-shot
			.sync = false, 						// Synchronizing timer operation on of other timers
			};

	TIMER0->TOP = topVal;

	TIMER_Init(TIMER0, &timer0Init);

	TIMER_Enable(TIMER0, true);
}

void motor_control(void)
{
	if (flex_sensor_data[0] > flex_sensor_data[1])
		motor_out[0] = flex_sensor_data[0] - flex_sensor_data[1];
	else if (flex_sensor_data[1] > flex_sensor_data[0])
		motor_out[0] = flex_sensor_data[1] - flex_sensor_data[0];
	else
		motor_out[0] = flex_sensor_data[0] - flex_sensor_data[1];

	if (flex_sensor_data[2] > flex_sensor_data[3])
		motor_out[1] = flex_sensor_data[2] - flex_sensor_data[3];
	else if (flex_sensor_data[3] > flex_sensor_data[2])
		motor_out[1] = flex_sensor_data[3] - flex_sensor_data[2];
	else
		motor_out[1] = flex_sensor_data[2] - flex_sensor_data[3];

	if (motor_out[0] > 0)
	{
		/* PA0 and PA1 */
		//GPIO_PinOutSet(LED0Port, LED0Pin);
		GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 1);
		GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);
		GPIO_DriveStrengthSet(gpioPortA, GPIO_P_CTRL_DRIVESTRENGTH_STRONG);
		GPIO_PinOutSet(gpioPortA, 0);
		GPIO_PinOutClear(gpioPortA, 1);
	}
	else if (motor_out[0] < 0)
	{
		/* PA0 and PA1 */
		//GPIO_PinOutSet(LED0Port, LED0Pin);
		GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 1);
		GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);
		GPIO_DriveStrengthSet(gpioPortA, gpioDriveStrengthStrongAlternateStrong);
		GPIO_PinOutSet(gpioPortA, 1);
		GPIO_PinOutClear(gpioPortA, 0);
	}
	else
	{
		//GPIO_PinOutClear(LED0Port, LED0Pin);
		/* PA0 and PA1 */
		//GPIO_PinOutSet(LED0Port, LED0Pin);
		GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 1);
		GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);
		GPIO_DriveStrengthSet(gpioPortA, gpioDriveStrengthStrongAlternateStrong);
		GPIO_PinOutClear(gpioPortA, 0);
		GPIO_PinOutClear(gpioPortA, 1);
	}

	if (motor_out[1] > 0)
	{
		/* PA2 and PA3 */
		//GPIO_PinOutSet(LED0Port, LED0Pin);
		GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);
		GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1);
		GPIO_DriveStrengthSet(gpioPortA, gpioDriveStrengthStrongAlternateStrong);
		GPIO_PinOutSet(gpioPortA, 2);
		GPIO_PinOutClear(gpioPortA, 3);
	}
	else if (motor_out[1] < 0)
	{
		/* PA2 and PA3 */
		//GPIO_PinOutSet(LED0Port, LED0Pin);
		GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);
		GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1);
		GPIO_DriveStrengthSet(gpioPortA, gpioDriveStrengthStrongAlternateStrong);
		GPIO_PinOutSet(gpioPortA, 3);
		GPIO_PinOutClear(gpioPortA, 2);
	}
	else
	{
		//GPIO_PinOutClear(LED1Port, LED1Pin);
		/* PA2 and PA3 */
		//GPIO_PinOutSet(LED0Port, LED0Pin);
		GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);
		GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1);
		GPIO_DriveStrengthSet(gpioPortA, gpioDriveStrengthStrongAlternateStrong);
		GPIO_PinOutClear(gpioPortA, 2);
		GPIO_PinOutClear(gpioPortA, 3);
	}

}
