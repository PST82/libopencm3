/** @addtogroup wwdg_file WWDG peripheral API
@ingroup peripheral_apis

@author @htmlonly &copy; @endhtmlonly 2010 Thomas Otto <tommi@viadmin.org>

This library supports the Window Watchdog Timer System in the STM32 series
of ARM Cortex Microcontrollers by ST Microelectronics.

The window watchdog (WWDG) is a free-running downcounter with a configurable
time-window for refresh. If the watchdog is refreshed outside this window
or if it reaches zero, a system reset is triggered. An early wakeup interrupt
can be enabled to warn before the reset occurs.

Unlike the Independent Watchdog (IWDG), the WWDG runs from the APB1 clock,
can be stopped in debug/standby modes, and must be refreshed within a
specific window value.

Example usage:
@code
	// Enable WWDG clock
	rcc_periph_clock_enable(RCC_WWDG);
	
	// Configure prescaler and window
	wwdg_set_prescaler(WWDG_CFR_WDGTB_CK_DIV8);
	wwdg_set_window(0x50);
	
	// Start watchdog with initial counter value
	wwdg_start(0x7F);
	
	// In your main loop, refresh the watchdog
	wwdg_reset();
@endcode

*/
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/

#include <libopencm3/stm32/wwdg.h>

/*---------------------------------------------------------------------------*/
/** @brief WWDG Set Prescaler

Sets the WWDG prescaler. The prescaler divides the PCLK1 clock before it
reaches the WWDG down-counter.

@param[in] prescaler WWDG Prescaler value (WWDG_CFR_WDGTB_CK_DIVn):
- WWDG_CFR_WDGTB_CK_DIV1: PCLK1 / 4096 / 1
- WWDG_CFR_WDGTB_CK_DIV2: PCLK1 / 4096 / 2
- WWDG_CFR_WDGTB_CK_DIV4: PCLK1 / 4096 / 4
- WWDG_CFR_WDGTB_CK_DIV8: PCLK1 / 4096 / 8

@note Use only the predefined WWDG_CFR_WDGTB_CK_DIVn macros as values.
*/
void wwdg_set_prescaler(uint32_t prescaler)
{
	uint32_t reg32 = WWDG_CFR;
	reg32 &= ~(WWDG_CFR_WDGTB_MASK << WWDG_CFR_WDGTB_LSB);
	reg32 |= (prescaler << WWDG_CFR_WDGTB_LSB);
	WWDG_CFR = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief WWDG Set Window Value

Sets the window value. The watchdog must be refreshed when the counter is
between the window value and 0x40 (the minimum allowed value). Refreshing
outside this window triggers a reset.

@param[in] window uint8_t Window value (0x40 to 0x7F).
*/
void wwdg_set_window(uint8_t window)
{
	uint32_t reg32 = WWDG_CFR;
	reg32 &= ~(WWDG_CFR_W_MASK << WWDG_CFR_W_LSB);
	reg32 |= WWDG_CFR_W(window & WWDG_CFR_W_MASK);
	WWDG_CFR = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief WWDG Start Watchdog

Starts the WWDG with the specified counter value. Once started, the watchdog
is always on and can only be reset by a system reset.

@param[in] counter uint8_t Initial counter value (0x40 to 0x7F).
*/
void wwdg_start(uint8_t counter)
{
	WWDG_CR = WWDG_CR_WDGA | WWDG_CR_T(counter & WWDG_CR_T_MASK);
}

/*---------------------------------------------------------------------------*/
/** @brief WWDG Reset/Refresh Watchdog

Resets the WWDG counter by writing a new counter value. This must be done
when the counter is between the window value and 0x40.

The counter should always be greater than 0x40 to avoid immediate reset.
Typically called with 0x7F to reset to maximum value.
*/
void wwdg_reset(void)
{
	/* Refresh with max counter value while preserving WDGA bit */
	WWDG_CR = (WWDG_CR & WWDG_CR_WDGA) | WWDG_CR_T(0x7F);
}

/*---------------------------------------------------------------------------*/
/** @brief WWDG Enable Early Wakeup Interrupt

Enables the early wakeup interrupt. This interrupt is triggered when the
counter reaches 0x40, giving the application a chance to refresh the watchdog
or perform cleanup before the system reset occurs at 0x3F.
*/
void wwdg_enable_early_wakeup_interrupt(void)
{
	WWDG_CFR |= WWDG_CFR_EWI;
}

/*---------------------------------------------------------------------------*/
/** @brief WWDG Clear Early Wakeup Interrupt Flag

Clears the early wakeup interrupt flag. This must be called in the WWDG
interrupt handler to prevent repeated interrupts.

Note: The WWDG_SR register only contains the EWIF flag (bit 0), and all other
bits are reserved. Writing 0 clears the flag as per STM32 reference manuals.
*/
void wwdg_clear_early_wakeup_interrupt_flag(void)
{
	/* Write 0 to clear the EWIF flag. This is the documented method
	 * in STM32 reference manuals for clearing the EWIF bit. */
	WWDG_SR = 0;
}

/*---------------------------------------------------------------------------*/
/** @brief WWDG Get Early Wakeup Interrupt Flag Status

@returns bool: true if the early wakeup interrupt flag is set.
*/
bool wwdg_get_early_wakeup_interrupt_flag(void)
{
	return (WWDG_SR & WWDG_SR_EWIF) != 0;
}

/**@}*/

