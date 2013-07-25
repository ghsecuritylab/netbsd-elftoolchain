/*	omap_dmtimerreg.h,v 1.1 2012/12/11 19:01:18 riastradh Exp	*/

/*
 * TI OMAP Dual-mode timers: Registers
 */

/*-
 * Copyright (c) 2012 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Taylor R. Campbell.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * References: TI AM335x TRM, TI OMAP35x TRM.
 */

#ifndef _ARM_OMAP_OMAP_DMTIMERREG_H_
#define _ARM_OMAP_OMAP_DMTIMERREG_H_

#include <sys/cdefs.h>

#define OMAP_DMTIMER_ID					0x00 /* TIDR */
#define OMAP_DMTIMER_OCP_CFG				0x10 /* TOCP_CFG */

#define   OMAP_DMTIMER_OCP_CFG_IDLEMODE_FORCE_IDLE	0
#define   OMAP_DMTIMER_OCP_CFG_IDLEMODE_NO_IDLE		1
#define   OMAP_DMTIMER_OCP_CFG_IDLEMODE_SMART_IDLE	2
#define   OMAP_DMTIMER_OCP_CFG_IDLEMODE_RESERVED	3

/*
 * DM timer version 1
 */

#define   OMAP_DMTIMER_V1_ID_REVISION_MASK		__BITS(0, 7)
#define   OMAP_DMTIMER_V1_ID_RESERVED_MASK		__BITS(8, 31)

#define   OMAP_DMTIMER_V1_OCP_CFG_AUTOIDLE_MASK		__BITS(0, 0)
#define   OMAP_DMTIMER_V1_OCP_CFG_SOFTRESET_MASK	__BITS(1, 1)
#define   OMAP_DMTIMER_V1_OCP_CFG_WAKEUP_MASK		__BITS(2, 2)
#define   OMAP_DMTIMER_V1_OCP_CFG_IDLEMODE_MASK		__BITS(3, 4)
#define   OMAP_DMTIMER_V1_OCP_CFG_EMUFREE_MASK		__BITS(5, 5)
#define   OMAP_DMTIMER_V1_OCP_CFG_RESERVED0_MASK	__BITS(6, 8)
#define   OMAP_DMTIMER_V1_OCP_CFG_CLOCKACT_MASK		__BITS(9, 8)
#define     OMAP_DMTIMER_V1_OCP_CFG_CLOCKACT_OFF	0
#define     OMAP_DMTIMER_V1_OCP_CFG_CLOCKACT_FUN	1
#define     OMAP_DMTIMER_V1_OCP_CFG_CLOCKACT_L4		2
#define     OMAP_DMTIMER_V1_OCP_CFG_CLOCKACT_L4_FUN	3

#define   OMAP_DMTIMER_INTR_MATCH			__BIT(0)
#define   OMAP_DMTIMER_INTR_OVERFLOW			__BIT(1)
#define   OMAP_DMTIMER_INTR_CAPTURE			__BIT(2)
#define   OMAP_DMTIMER_INTR_ALL				0x7

#define OMAP_DMTIMER_V1_STATUS				0x14 /* TISTAT */
#define   OMAP_DMTIMER_V1_STATUS_RESETDONE		__BIT(0)
#define OMAP_DMTIMER_V1_INTR_STATUS			0x18 /* TISR */
#define OMAP_DMTIMER_V1_INTR_ENABLE			0x1c /* TIER */
#define OMAP_DMTIMER_V1_TIMER_REGS			0x20

/*
 * DM timer version 2
 */

#define   OMAP_DMTIMER_V2_ID_Y_MINOR_MASK		__BITS(0, 5)
#define   OMAP_DMTIMER_V2_ID_CUSTOM_MASK		__BITS(6, 7)
#define   OMAP_DMTIMER_V2_ID_X_MAJOR_MASK		__BITS(8, 10)
#define   OMAP_DMTIMER_V2_ID_R_RTL_MASK			__BITS(11, 15)
#define   OMAP_DMTIMER_V2_ID_FUNC_MASK			__BITS(16, 27)
#define   OMAP_DMTIMER_V2_ID_RESERVED_MASK		__BITS(28, 29)
#define   OMAP_DMTIMER_V2_ID_SCHEME_MASK		__BITS(30, 31)

#define   OMAP_DMTIMER_V2_OCP_CFG_SOFTRESET_MASK	__BITS(0, 0)
#define   OMAP_DMTIMER_V2_OCP_CFG_EMUFREE_MASK		__BITS(1, 1)
#define   OMAP_DMTIMER_V2_OCP_CFG_IDLEMODE_MASK		__BITS(2, 3)

#define OMAP_DMTIMER_V2_INTR_STATUS_RAW			0x24
#define OMAP_DMTIMER_V2_INTR_STATUS			0x28
#define OMAP_DMTIMER_V2_INTR_ENABLE_SET			0x2c
#define OMAP_DMTIMER_V2_INTR_ENABLE_CLEAR		0x30
#define OMAP_DMTIMER_V2_TIMER_REGS			0x34

#define OMAP_DMTIMER_REG_INDEX_MASK			0x00ff
#define OMAP_DMTIMER_REG_POSTED_BIT_MASK		0x0f00
#define OMAP_DMTIMER_REG_POSTED_INDEX(reg)		\
	__SHIFTOUT((reg), OMAP_DMTIMER_REG_INDEX_MASK)
#define OMAP_DMTIMER_REG_POSTED_BIT(reg)		\
	__SHIFTOUT((reg), OMAP_DMTIMER_REG_POSTED_BIT_MASK)
#define OMAP_DMTIMER_REG_POSTED_MASK(reg)		\
	__BIT(OMAP_DMTIMER_REG_POSTED_BIT(reg))
#define OMAP_DMTIMER_REG_POSTED_P(reg)			\
	(((reg) & 0x1000) == 0x1000)

#define OMAP_DMTIMER_TIMER_INTR_WAKEUP			0x0000 /* TWER */
#define OMAP_DMTIMER_TIMER_CTRL				0x1004 /* TCLR */
#define   OMAP_DMTIMER_TIMER_CTRL_START			__BIT(0)
#define   OMAP_DMTIMER_TIMER_CTRL_AUTORELOAD		__BIT(1)
#define   OMAP_DMTIMER_TIMER_CTRL_PRESCALE_MASK		__BITS(2, 4)
#define   OMAP_DMTIMER_TIMER_CTRL_PRESCALE_ENABLE	__BIT(5)
#define   OMAP_DMTIMER_TIMER_CTRL_COMPARE_ENABLE	__BIT(6)
#define   OMAP_DMTIMER_TIMER_CTRL_PWM_SIGN		__BIT(7)
#define   OMAP_DMTIMER_TIMER_CTRL_TCM_MASK		__BITS(8, 9)
#define     OMAP_DMTIMER_TIMER_CTRL_TCM_NONE		0
#define     OMAP_DMTIMER_TIMER_CTRL_TCM_LOW_HIGH	1
#define     OMAP_DMTIMER_TIMER_CTRL_TCM_HIGH_LOW	2
#define     OMAP_DMTIMER_TIMER_CTRL_TCM_BOTH		3
#define   OMAP_DMTIMER_TIMER_CTRL_TRG_MASK		__BITS(10, 11)
#define     OMAP_DMTIMER_TIMER_CTRL_TRG_NONE		0
#define     OMAP_DMTIMER_TIMER_CTRL_TRG_OVERFLOW	1
#define     OMAP_DMTIMER_TIMER_CTRL_TRG_OVERFLOW_MATCH	2
#define     OMAP_DMTIMER_TIMER_CTRL_TRG_RESERVED	3
#define   OMAP_DMTIMER_TIMER_CTRL_PWM_PT		__BIT(12)
#define     OMAP_DMTIMER_TIMER_CTRL_PWM_PULSE		0
#define     OMAP_DMTIMER_TIMER_CTRL_PWM_TOGGLE		1
#define   OMAP_DMTIMER_TIMER_CTRL_CAPTURE_MODE		__BIT(13)
#define     OMAP_DMTIMER_TIMER_CTRL_CAPTURE_MODE_FIRST	0
#define     OMAP_DMTIMER_TIMER_CTRL_CAPTURE_MODE_SECOND	1
#define   OMAP_DMTIMER_TIMER_CTRL_GPO_CFG		__BIT(14)
#define OMAP_DMTIMER_TIMER_COUNTER			0x1108 /* TCRR */
#define OMAP_DMTIMER_TIMER_LOAD				0x120c /* TLDR */
#define OMAP_DMTIMER_TIMER_TRIGGER			0x1310 /* TTGR */
#define OMAP_DMTIMER_TIMER_WRITE_POST			0x0014 /* TWPS */
#define   OMAP_DMTIMER_TIMER_WRITE_POST_CTRL		__BIT(0)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_COUNTER		__BIT(1)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_LOAD		__BIT(2)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_TRIGGER		__BIT(3)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_MATCH		__BIT(4)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_POS_INCR	__BIT(5)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_NEG_INCR	__BIT(6)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_COUNTER_VALUE	__BIT(7)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_INTR_MASK_SET	__BIT(8)
#define   OMAP_DMTIMER_TIMER_WRITE_POST_INTR_MASK_COUNT	__BIT(9)
#define OMAP_DMTIMER_TIMER_MATCH			0x1418 /* TMAR */
#define OMAP_DMTIMER_TIMER_CAPTURE1			0x001c /* TCAR1 */
#define OMAP_DMTIMER_TIMER_SYNC_INT_CTRL		0x0020 /* TSICR */
#define   OMAP_DMTIMER_TIMER_SYNC_INT_CTRL_SOFTRESET	__BIT(1)
#define   OMAP_DMTIMER_TIMER_SYNC_INT_CTRL_POSTED	__BIT(2)
#define OMAP_DMTIMER_TIMER_CAPTURE2			0x0024 /* TCAR2 */
#define OMAP_DMTIMER_TICK_POS_INCR			0x1528 /* TPIR */
#define OMAP_DMTIMER_TICK_NEG_INCR			0x162c /* TNIR */
#define OMAP_DMTIMER_TICK_COUNTER_VALUE			0x1730 /* TCVR */
#define OMAP_DMTIMER_TICK_INTR_MASK_SET			0x1834 /* TOCR */
#define OMAP_DMTIMER_TICK_INTR_MASK_COUNT		0x1938 /* TOWR */

#endif	/* _ARM_OMAP_OMAP_DMTIMERREG_H_ */