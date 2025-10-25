/** @addtogroup wwdg_defines

@author @htmlonly &copy; @endhtmlonly 2010 Thomas Otto <tommi@viadmin.org>

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

/* THIS FILE SHOULD NOT BE INCLUDED DIRECTLY, BUT ONLY VIA WWDG.H
The order of header inclusion is important. wwdg.h includes the device
specific memorymap.h header before including this header file.*/

/** @cond */
#ifdef LIBOPENCM3_WWDG_H
/** @endcond */
#ifndef LIBOPENCM3_WWDG_COMMON_ALL_H
#define LIBOPENCM3_WWDG_COMMON_ALL_H

/**@{*/

/* --- WWDG registers ------------------------------------------------------ */

/** Control Register (WWDG_CR) */
#define WWDG_CR				MMIO32(WWDG_BASE + 0x00)

/** Configuration Register (WWDG_CFR) */
#define WWDG_CFR			MMIO32(WWDG_BASE + 0x04)

/** Status Register (WWDG_SR) */
#define WWDG_SR				MMIO32(WWDG_BASE + 0x08)

/* --- WWDG_CR values ------------------------------------------------------ */

/* Bits [31:8]: Reserved */

/** WDGA: Activation bit */
#define WWDG_CR_WDGA			(1 << 7)

/** T[6:0]: 7-bit counter (MSB to LSB) */
#define WWDG_CR_T_LSB			0
#define WWDG_CR_T_MASK			0x7F
#define WWDG_CR_T(n)			((n) << WWDG_CR_T_LSB)

/* --- WWDG_CFR values ----------------------------------------------------- */

/* Bits [31:10]: Reserved */

/** EWI: Early wakeup interrupt enable */
#define WWDG_CFR_EWI			(1 << 9)

/** WDGTB[8:7]: Timer base */
#define WWDG_CFR_WDGTB_LSB		7
#define WWDG_CFR_WDGTB_MASK		0x3
/** @defgroup wwdg_prescaler WWDG Prescaler
@ingroup wwdg_defines

@{*/
#define WWDG_CFR_WDGTB_CK_DIV1		(0x0 << WWDG_CFR_WDGTB_LSB)
#define WWDG_CFR_WDGTB_CK_DIV2		(0x1 << WWDG_CFR_WDGTB_LSB)
#define WWDG_CFR_WDGTB_CK_DIV4		(0x2 << WWDG_CFR_WDGTB_LSB)
#define WWDG_CFR_WDGTB_CK_DIV8		(0x3 << WWDG_CFR_WDGTB_LSB)
/**@}*/

/** W[6:0]: 7-bit window value */
#define WWDG_CFR_W_LSB			0
#define WWDG_CFR_W_MASK			0x7F
#define WWDG_CFR_W(n)			((n) << WWDG_CFR_W_LSB)

/* --- WWDG_SR values ------------------------------------------------------ */

/* Bits [31:1]: Reserved */

/** EWIF: Early wakeup interrupt flag */
#define WWDG_SR_EWIF			(1 << 0)

/* --- WWDG function prototypes---------------------------------------------- */

BEGIN_DECLS

void wwdg_reset(void);
void wwdg_set_prescaler(uint32_t prescaler);
void wwdg_set_window(uint8_t window);
void wwdg_start(uint8_t counter);
void wwdg_enable_early_wakeup_interrupt(void);
void wwdg_clear_early_wakeup_interrupt_flag(void);
bool wwdg_get_early_wakeup_interrupt_flag(void);

END_DECLS

#endif
/** @cond */
#else
#warning "wwdg_common_all.h should not be included explicitly, only via wwdg.h"
#endif
/** @endcond */
/**@}*/

