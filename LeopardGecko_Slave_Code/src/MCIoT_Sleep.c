/*****************************************************************************
 * @file 	MCIoT_Sleep.c
 * @brief 	This file describes the functions pertaining to sleep routines.
 * @author 	Pavan Dhareshwar
 * @version 1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ************************************************************************************/

/************************************ INCLUDES **************************************/
#include <stdint.h>
#include <stdbool.h>
#include "em_emu.h"
#include "MCIoT_main.h"
#include "MCIoT_Sleep.h"
#include "MCIoT_ADC.h"
#include "MCIoT_CMU.h"
#include "MCIoT_GPIO.h"

/************************************************************************************
 * @function 	blockSleepMode
 * @params 		[in] e_letimer_energy_modes (LETimer energy mode enum)
 * @brief 		Blocks the MCU from sleeping below a certain mode.
 ************************************************************************************/
void blockSleepMode(ENERGY_MODES e_letimer_energy_modes)
{
#ifdef USE_INT
	INT_Disable();
#else
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();
#endif

	sleep_block_counter[e_letimer_energy_modes]++;

#ifdef USE_INT
	INT_Enable();
#else
	CORE_EXIT_ATOMIC();
#endif
}

/************************************************************************************
 * @function 	unblockSleepMode
 * @params 		[in] e_letimer_energy_modes - LETimer energy mode enum
 * @brief 		Unblocks the MCU from sleeping below a certain mode.
 ************************************************************************************/
void unblockSleepMode(ENERGY_MODES e_letimer_energy_modes)
{
#ifdef USE_INT
	INT_Disable();
#else
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_ATOMIC();
#endif

	if (sleep_block_counter[e_letimer_energy_modes] > 0)
	{
		sleep_block_counter[e_letimer_energy_modes]--;
	}

#ifdef USE_INT
	INT_Enable();
#else
	CORE_EXIT_ATOMIC();
#endif
}

/************************************************************************************
 * @function 	Sleep
 * @params 		None
 * @brief 		Sleep routine for LETIMER0.
 *				Credits to Silicon Labs for the following sleep routine.
 * @section 	License (C) Copyright 2015 Silicon Labs, http://www.silabs.com/
 ************************************************************************************/
void Sleep(void)
{
	if (sleep_block_counter[EM0] > 0)
	{
		return; 				/* Block everything below EM0, just return */
	}
	else if (sleep_block_counter[EM1] > 0)
	{
		EMU_EnterEM1(); 	/* Block everything below EM2, enter EM2 */
	}
	else if (sleep_block_counter[EM2] > 0)
	{
		EMU_EnterEM2(true); 	/* Block everything below EM2, enter EM2 */
	}
	else if (sleep_block_counter[EM3] > 0)
	{
		EMU_EnterEM3(true); 	/* Block everything below EM3, enter EM3 */
	}
	else
	{
		EMU_EnterEM3(true);  	/* Nothing is blocked, enter EM3 */
	}
}
