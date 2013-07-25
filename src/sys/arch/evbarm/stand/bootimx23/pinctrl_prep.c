/* pinctrl_prep.c,v 1.3 2013/02/07 21:56:36 jkunz Exp */

/*
 * Copyright (c) 2012 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Petri Laakso.
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

#include <sys/param.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <arm/imx/imx23_pinctrlreg.h>

#include <lib/libsa/stand.h>

#include "common.h"

static void configure_emi_mux(void);
static void configure_emi_drive(int);
static void disable_emi_padkeepers(void);
static void configure_ssp_mux(void);
static void configure_ssp_drive(int);
static void configure_ssp_pullups(void);
static void configure_dbuart_mux(void);

/* EMI pins output drive strengths */
#define DRIVE_04_MA	0x0	/* 4 mA */
#define DRIVE_08_MA	0x1	/* 8 mA */
#define DRIVE_12_MA	0x2	/* 12 mA */
#define DRIVE_16_MA	0x3	/* 16 mA */

/*
 * Configure external EMI pins.
 */
int
pinctrl_prep(void)
{

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_CTRL_CLR,
	    (HW_PINCTRL_CTRL_SFTRST | HW_PINCTRL_CTRL_CLKGATE));

	/* EMI. */
	configure_emi_mux();
	configure_emi_drive(DRIVE_12_MA);
	disable_emi_padkeepers();

	/* SSP. */
	configure_ssp_mux();
	configure_ssp_drive(DRIVE_16_MA);
	configure_ssp_pullups();

	/* Debug UART. */
	configure_dbuart_mux();

	return 0;
}

/*
 * Configure external EMI pins to be used for DRAM.
 */
static void
configure_emi_mux(void)
{

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_MUXSEL4_CLR, (
	    HW_PINCTRL_MUXSEL4_BANK2_PIN15 |	/* Pin 108, EMI_A06 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN14 |	/* Pin 107, EMI_A05 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN13 |	/* Pin 109, EMI_A04 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN12 |	/* Pin 110, EMI_A03 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN11 |	/* Pin 111, EMI_A02 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN10 |	/* Pin 112, EMI_A01 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN09)	/* Pin 113, EMI_A00 */
	);

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_MUXSEL5_CLR, (
	    HW_PINCTRL_MUXSEL5_BANK2_PIN31 |	/* Pin 114, EMI_WEN  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN30 |	/* Pin  98, EMI_RASN */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN29 |	/* Pin 115, EMI_CKE  */
#if 0
	/* 169-Pin BGA Package */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN26 |	/* Pin  99, EMI_CE1N */
#endif
	    HW_PINCTRL_MUXSEL5_BANK2_PIN25 |	/* Pin 100, EMI_CE0N */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN24 |	/* Pin  97, EMI_CASN */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN23 |	/* Pin 117, EMI_BA1  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN22 |	/* Pin 116, EMI_BA0  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN21 |	/* Pin 101, EMI_A12  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN20 |	/* Pin 102, EMI_A11  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN19 |	/* Pin 104, EMI_A10  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN18 |	/* Pin 103, EMI_A09  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN17 |	/* Pin 106, EMI_A08  */
	    HW_PINCTRL_MUXSEL5_BANK2_PIN16)	/* Pin 105, EMI_A07  */
	);

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_MUXSEL6_CLR, (
	    HW_PINCTRL_MUXSEL6_BANK3_PIN15 |	/* Pin 95, EMI_D15 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN14 |	/* Pin 96, EMI_D14 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN13 |	/* Pin 94, EMI_D13 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN12 |	/* Pin 93, EMI_D12 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN11 |	/* Pin 91, EMI_D11 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN10 |	/* Pin 89, EMI_D10 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN09 |	/* Pin 87, EMI_D09 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN08 |	/* Pin 86, EMI_D08 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN07 |	/* Pin 85, EMI_D07 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN06 |	/* Pin 84, EMI_D06 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN05 |	/* Pin 83, EMI_D05 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN04 |	/* Pin 82, EMI_D04 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN03 |	/* Pin 79, EMI_D03 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN02 |	/* Pin 77, EMI_D02 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN01 |	/* Pin 76, EMI_D01 */
	    HW_PINCTRL_MUXSEL6_BANK3_PIN00)	/* Pin 75, EMI_D00 */
	);

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_MUXSEL7_CLR, (
	    HW_PINCTRL_MUXSEL7_BANK3_PIN21 |	/* Pin 72, EMI_CLKN */
	    HW_PINCTRL_MUXSEL7_BANK3_PIN20 |	/* Pin 70, EMI_CLK  */
	    HW_PINCTRL_MUXSEL7_BANK3_PIN19 |	/* Pin 74, EMI_DQS1 */
	    HW_PINCTRL_MUXSEL7_BANK3_PIN18 |	/* Pin 73, EMI_DQS0 */
	    HW_PINCTRL_MUXSEL7_BANK3_PIN17 |	/* Pin 92, EMI_DQM1 */
	    HW_PINCTRL_MUXSEL7_BANK3_PIN16)	/* Pin 81, EMI_DQM0 */
	);

	return;
}

/*
 * Configure EMI pins voltages to 1.8/2.5V operation and drive strength
 * to "ma".
 */
static void
configure_emi_drive(int ma)
{
	uint32_t drive;

	/* DRIVE 9 */
	drive = REG_RD(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE9);
	drive &= ~(
	    HW_PINCTRL_DRIVE9_BANK2_PIN15_V |	/* Pin 108, EMI_A06 */
	    HW_PINCTRL_DRIVE9_BANK2_PIN15_MA |
	    HW_PINCTRL_DRIVE9_BANK2_PIN14_V |	/* Pin 107, EMI_A05 */
	    HW_PINCTRL_DRIVE9_BANK2_PIN14_MA |
	    HW_PINCTRL_DRIVE9_BANK2_PIN13_V |	/* Pin 109, EMI_A04 */
	    HW_PINCTRL_DRIVE9_BANK2_PIN13_MA |
	    HW_PINCTRL_DRIVE9_BANK2_PIN12_V |	/* Pin 110, EMI_A03 */
	    HW_PINCTRL_DRIVE9_BANK2_PIN12_MA |
	    HW_PINCTRL_DRIVE9_BANK2_PIN11_V |	/* Pin 111, EMI_A02 */
	    HW_PINCTRL_DRIVE9_BANK2_PIN11_MA |
	    HW_PINCTRL_DRIVE9_BANK2_PIN10_V |	/* Pin 112, EMI_A01 */
	    HW_PINCTRL_DRIVE9_BANK2_PIN10_MA |
	    HW_PINCTRL_DRIVE9_BANK2_PIN09_V |	/* Pin 113, EMI_A00 */
	    HW_PINCTRL_DRIVE9_BANK2_PIN09_MA |
	    HW_PINCTRL_DRIVE9_RSRVD6 |		/* Always write zeroes */
	    HW_PINCTRL_DRIVE9_RSRVD5 |
	    HW_PINCTRL_DRIVE9_RSRVD4 |
	    HW_PINCTRL_DRIVE9_RSRVD3 |
	    HW_PINCTRL_DRIVE9_RSRVD2 |
	    HW_PINCTRL_DRIVE9_RSRVD1 |
	    HW_PINCTRL_DRIVE9_RSRVD0
	);
	drive |= (
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE9_BANK2_PIN15_MA) |	/* EMI_A06 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE9_BANK2_PIN14_MA) |	/* EMI_A05 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE9_BANK2_PIN13_MA) |	/* EMI_A04 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE9_BANK2_PIN12_MA) |	/* EMI_A03 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE9_BANK2_PIN11_MA) |	/* EMI_A02 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE9_BANK2_PIN10_MA) |	/* EMI_A01 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE9_BANK2_PIN09_MA)	/* EMI_A00 */
	);
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE9, drive);

	/* DRIVE 10 */
	drive = REG_RD(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE10);
	drive &= ~(
	    HW_PINCTRL_DRIVE10_BANK2_PIN23_V |	/* Pin 117, EMI_BA1 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN23_MA |
	    HW_PINCTRL_DRIVE10_BANK2_PIN22_V |	/* Pin 116, EMI_BA0 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN22_MA |
	    HW_PINCTRL_DRIVE10_BANK2_PIN21_V |	/* Pin 101, EMI_A12 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN21_MA |
	    HW_PINCTRL_DRIVE10_BANK2_PIN20_V |	/* Pin 102, EMI_A11 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN20_MA |
	    HW_PINCTRL_DRIVE10_BANK2_PIN19_V |	/* Pin 104, EMI_A10 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN19_MA |
	    HW_PINCTRL_DRIVE10_BANK2_PIN18_V |	/* Pin 103, EMI_A09 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN18_MA |
	    HW_PINCTRL_DRIVE10_BANK2_PIN17_V |	/* Pin 106, EMI_A08 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN17_MA |
	    HW_PINCTRL_DRIVE10_BANK2_PIN16_V |	/* Pin 105, EMI_A07 */
	    HW_PINCTRL_DRIVE10_BANK2_PIN16_MA |
	    HW_PINCTRL_DRIVE10_RSRVD6 |		/* Always write zeroes */
	    HW_PINCTRL_DRIVE10_RSRVD5 |
	    HW_PINCTRL_DRIVE10_RSRVD4 |
	    HW_PINCTRL_DRIVE10_RSRVD3 |
	    HW_PINCTRL_DRIVE10_RSRVD2 |
	    HW_PINCTRL_DRIVE10_RSRVD1 |
	    HW_PINCTRL_DRIVE10_RSRVD0
	);
	drive |= (
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN23_MA) |	/* EMI_BA1 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN22_MA) |	/* EMI_BA0 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN21_MA) |	/* EMI_A12 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN20_MA) |	/* EMI_A11 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN19_MA) |	/* EMI_A10 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN18_MA) |	/* EMI_A09 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN17_MA) |	/* EMI_A08 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE10_BANK2_PIN16_MA)	/* EMI_A07 */
	);
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE10, drive);

	/* DRIVE 11 */
	drive = REG_RD(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE11);
	drive &= ~(
	    HW_PINCTRL_DRIVE11_BANK2_PIN31_V |	/* Pin 114, EMI_WEN */
	    HW_PINCTRL_DRIVE11_BANK2_PIN31_MA |
	    HW_PINCTRL_DRIVE11_BANK2_PIN30_V |	/* Pin 98, EMI_RASN */
	    HW_PINCTRL_DRIVE11_BANK2_PIN30_MA |
	    HW_PINCTRL_DRIVE11_BANK2_PIN29_V |	/* Pin 115, EMI_CKE */
	    HW_PINCTRL_DRIVE11_BANK2_PIN29_MA |
#if 0
	/* 169-Pin BGA Package */
	    HW_PINCTRL_DRIVE11_BANK2_PIN26_V |	/* Pin 99, EMI_CE1N */
	    HW_PINCTRL_DRIVE11_BANK2_PIN26_MA |
#endif
	    HW_PINCTRL_DRIVE11_BANK2_PIN25_V |	/* Pin 100, EMI_CE0N */
	    HW_PINCTRL_DRIVE11_BANK2_PIN25_MA |
	    HW_PINCTRL_DRIVE11_BANK2_PIN24_V |	/* Pin 97, EMI_CASN */
	    HW_PINCTRL_DRIVE11_BANK2_PIN24_MA |
	    HW_PINCTRL_DRIVE11_RSRVD6 |		/* Always write zeroes */
	    HW_PINCTRL_DRIVE11_RSRVD5 |
	    HW_PINCTRL_DRIVE11_RSRVD4 |
	    HW_PINCTRL_DRIVE11_RSRVD3 |
	    HW_PINCTRL_DRIVE11_RSRVD2 |
	    HW_PINCTRL_DRIVE11_RSRVD1 |
	    HW_PINCTRL_DRIVE11_RSRVD0
	);
	drive |= (
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE11_BANK2_PIN31_MA) |	/* EMI_WEN  */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE11_BANK2_PIN30_MA) |	/* EMI_RASN */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE11_BANK2_PIN29_MA) |	/* EMI_CKE  */
#if 0
	/* 169-Pin BGA Package */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE11_BANK2_PIN26_MA) |	/* EMI_CE1N */
#endif
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE11_BANK2_PIN25_MA) |	/* EMI_CE0N */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE11_BANK2_PIN24_MA)	/* EMI_CASN */
	);
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE11, drive);

	/* DRIVE 12 */
	drive = REG_RD(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE12);
	drive &= ~(
	    HW_PINCTRL_DRIVE12_BANK3_PIN07_V |	/* Pin 85, EMI_D07 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN07_MA |
	    HW_PINCTRL_DRIVE12_BANK3_PIN06_V |	/* Pin 84, EMI_D06 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN06_MA |
	    HW_PINCTRL_DRIVE12_BANK3_PIN05_V |	/* Pin 83, EMI_D05 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN05_MA |
	    HW_PINCTRL_DRIVE12_BANK3_PIN04_V |	/* Pin 82, EMI_D04 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN04_MA |
	    HW_PINCTRL_DRIVE12_BANK3_PIN03_V |	/* Pin 79, EMI_D03 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN03_MA |
	    HW_PINCTRL_DRIVE12_BANK3_PIN02_V |	/* Pin 77, EMI_D02 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN02_MA |
	    HW_PINCTRL_DRIVE12_BANK3_PIN01_V |	/* Pin 76, EMI_D01 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN01_MA |
	    HW_PINCTRL_DRIVE12_BANK3_PIN00_V |	/* Pin 75, EMI_D00 */
	    HW_PINCTRL_DRIVE12_BANK3_PIN00_MA |
	    HW_PINCTRL_DRIVE12_RSRVD6 |		/* Always write zeroes */
	    HW_PINCTRL_DRIVE12_RSRVD5 |
	    HW_PINCTRL_DRIVE12_RSRVD4 |
	    HW_PINCTRL_DRIVE12_RSRVD3 |
	    HW_PINCTRL_DRIVE12_RSRVD2 |
	    HW_PINCTRL_DRIVE12_RSRVD1 |
	    HW_PINCTRL_DRIVE12_RSRVD0
	);
	drive |= (
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN07_MA) |	/* EMI_D07 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN06_MA) |	/* EMI_D06 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN05_MA) |	/* EMI_D05 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN04_MA) |	/* EMI_D04 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN03_MA) |	/* EMI_D03 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN02_MA) |	/* EMI_D02 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN01_MA) |	/* EMI_D01 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE12_BANK3_PIN00_MA)	/* EMI_D00 */
	);
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE12, drive);

	/* DRIVE 13 */
	drive = REG_RD(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE13);
	drive &= ~(
	    HW_PINCTRL_DRIVE13_BANK3_PIN15_V |	/* Pin 95, EMI_D15 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN15_MA |
	    HW_PINCTRL_DRIVE13_BANK3_PIN14_V |	/* Pin 96, EMI_D14 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN14_MA |
	    HW_PINCTRL_DRIVE13_BANK3_PIN13_V |	/* Pin 94, EMI_D13 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN13_MA |
	    HW_PINCTRL_DRIVE13_BANK3_PIN12_V |	/* Pin 93, EMI_D12 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN12_MA |
	    HW_PINCTRL_DRIVE13_BANK3_PIN11_V |	/* Pin 91, EMI_D11 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN11_MA |
	    HW_PINCTRL_DRIVE13_BANK3_PIN10_V |	/* Pin 89, EMI_D10 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN10_MA |
	    HW_PINCTRL_DRIVE13_BANK3_PIN09_V |	/* Pin 87, EMI_D09 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN09_MA |
	    HW_PINCTRL_DRIVE13_BANK3_PIN08_V |	/* Pin 86, EMI_D08 */
	    HW_PINCTRL_DRIVE13_BANK3_PIN08_MA |
	    HW_PINCTRL_DRIVE13_RSRVD6 |		/* Always write zeroes */
	    HW_PINCTRL_DRIVE13_RSRVD5 |
	    HW_PINCTRL_DRIVE13_RSRVD4 |
	    HW_PINCTRL_DRIVE13_RSRVD3 |
	    HW_PINCTRL_DRIVE13_RSRVD2 |
	    HW_PINCTRL_DRIVE13_RSRVD1 |
	    HW_PINCTRL_DRIVE13_RSRVD0
	);
	drive |= (
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN15_MA) |	/* EMI_D15 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN14_MA) |	/* EMI_D14 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN13_MA) |	/* EMI_D13 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN12_MA) |	/* EMI_D12 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN11_MA) |	/* EMI_D11 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN10_MA) |	/* EMI_D10 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN09_MA) |	/* EMI_D09 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE13_BANK3_PIN08_MA)	/* EMI_D08 */
	);
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE13, drive);

	/* DRIVE 14 */
	drive = REG_RD(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE14);
	drive &= ~(
	    HW_PINCTRL_DRIVE14_BANK3_PIN21_V |	/* Pin 72, EMI_CLKN */
	    HW_PINCTRL_DRIVE14_BANK3_PIN21_MA |
	    HW_PINCTRL_DRIVE14_BANK3_PIN20_V |	/* Pin 70, EMI_CLK  */
	    HW_PINCTRL_DRIVE14_BANK3_PIN20_MA |
	    HW_PINCTRL_DRIVE14_BANK3_PIN19_V |	/* Pin 74, EMI_DQS1 */
	    HW_PINCTRL_DRIVE14_BANK3_PIN19_MA |
	    HW_PINCTRL_DRIVE14_BANK3_PIN18_V |	/* Pin 73, EMI_DQS0 */
	    HW_PINCTRL_DRIVE14_BANK3_PIN18_MA |
	    HW_PINCTRL_DRIVE14_BANK3_PIN17_V |	/* Pin 92, EMI_DQM1 */
	    HW_PINCTRL_DRIVE14_BANK3_PIN17_MA |
	    HW_PINCTRL_DRIVE14_BANK3_PIN16_V |	/* Pin 81, EMI_DQM0 */
	    HW_PINCTRL_DRIVE14_BANK3_PIN16_MA |
	    HW_PINCTRL_DRIVE14_RSRVD6 |		/* Always write zeroes */
	    HW_PINCTRL_DRIVE14_RSRVD5 |
	    HW_PINCTRL_DRIVE14_RSRVD4 |
	    HW_PINCTRL_DRIVE14_RSRVD3 |
	    HW_PINCTRL_DRIVE14_RSRVD2 |
	    HW_PINCTRL_DRIVE14_RSRVD1 |
	    HW_PINCTRL_DRIVE14_RSRVD0
	);
	drive |= (
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE14_BANK3_PIN21_MA) |	/* EMI_CLKN */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE14_BANK3_PIN20_MA) |	/* EMI_CLK  */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE14_BANK3_PIN19_MA) |	/* EMI_DQS1 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE14_BANK3_PIN18_MA) |	/* EMI_DQS0 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE14_BANK3_PIN17_MA) |	/* EMI_DQM1 */
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE14_BANK3_PIN16_MA)	/* EMI_DQM0 */
	);
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE14, drive);

	return;
}

/*
 * Disable internal gate keepers on EMI pins.
 */
static void
disable_emi_padkeepers(void)
{

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_PULL3_SET, (
	    HW_PINCTRL_PULL3_BANK3_PIN17 |	/* EMI_DQM1 */
	    HW_PINCTRL_PULL3_BANK3_PIN16 |	/* EMI_DQM0 */
	    HW_PINCTRL_PULL3_BANK3_PIN15 |	/* EMI_D15 */
	    HW_PINCTRL_PULL3_BANK3_PIN14 |	/* EMI_D14 */
	    HW_PINCTRL_PULL3_BANK3_PIN13 |	/* EMI_D13 */
	    HW_PINCTRL_PULL3_BANK3_PIN12 |	/* EMI_D12 */
	    HW_PINCTRL_PULL3_BANK3_PIN11 |	/* EMI_D11 */
	    HW_PINCTRL_PULL3_BANK3_PIN10 |	/* EMI_D10 */
	    HW_PINCTRL_PULL3_BANK3_PIN09 |	/* EMI_D09 */
	    HW_PINCTRL_PULL3_BANK3_PIN08 |	/* EMI_D08 */
	    HW_PINCTRL_PULL3_BANK3_PIN07 |	/* EMI_D07 */
	    HW_PINCTRL_PULL3_BANK3_PIN06 |	/* EMI_D06 */
	    HW_PINCTRL_PULL3_BANK3_PIN05 |	/* EMI_D05 */
	    HW_PINCTRL_PULL3_BANK3_PIN04 |	/* EMI_D04 */
	    HW_PINCTRL_PULL3_BANK3_PIN03 |	/* EMI_D03 */
	    HW_PINCTRL_PULL3_BANK3_PIN02 |	/* EMI_D02 */
	    HW_PINCTRL_PULL3_BANK3_PIN01 |	/* EMI_D01 */
	    HW_PINCTRL_PULL3_BANK3_PIN00)	/* EMI_D00 */
	);

	return;
}

/*
 * Configure external SSP pins to be used for SD/MMC.
 */
static void
configure_ssp_mux(void)
{
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_MUXSEL4_CLR, (
	    HW_PINCTRL_MUXSEL4_BANK2_PIN00 |	/* Pin 121, SSP1_CMD */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN02 |	/* Pin 122, SSP1_DATA0 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN03 |	/* Pin 123, SSP1_DATA1 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN04 |	/* Pin 124, SSP1_DATA2 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN05 |	/* Pin 125, SSP1_DATA3 */
	    HW_PINCTRL_MUXSEL4_BANK2_PIN06)	/* Pin 127, SSP1_SCK */
	);

	return;
}

/*
 * Configure SSP pins drive strength to "ma".
 * Out of reset, all non-EMI pins are configured as 3.3 V.
 */
static void
configure_ssp_drive(int ma)
{
	uint32_t reg = REG_RD(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE8);
	reg &= ~(
	    HW_PINCTRL_DRIVE8_BANK2_PIN06_MA |	/* Pin 127, SSP1_SCK */
	    HW_PINCTRL_DRIVE8_BANK2_PIN05_MA |	/* Pin 125, SSP1_DATA3 */
	    HW_PINCTRL_DRIVE8_BANK2_PIN04_MA |	/* Pin 124, SSP1_DATA2 */
	    HW_PINCTRL_DRIVE8_BANK2_PIN03_MA |	/* Pin 123, SSP1_DATA1 */
	    HW_PINCTRL_DRIVE8_BANK2_PIN02_MA |	/* Pin 122, SSP1_DATA0 */
	    HW_PINCTRL_DRIVE8_BANK2_PIN00_MA	/* Pin 121, SSP1_CMD */
	);

	reg |= __SHIFTIN(ma, HW_PINCTRL_DRIVE8_BANK2_PIN06_MA) |
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE8_BANK2_PIN05_MA) |
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE8_BANK2_PIN04_MA) |
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE8_BANK2_PIN03_MA) |
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE8_BANK2_PIN02_MA) |
	    __SHIFTIN(ma, HW_PINCTRL_DRIVE8_BANK2_PIN00_MA
	);

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_DRIVE8, reg);

	return;
}

/*
 * Configure SSP pull ups.
 */
static void
configure_ssp_pullups(void)
{
	/* Disable pull ups for SSP and let HW take care of them. */
//	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_PULL2_CLR, (
//	    HW_PINCTRL_PULL2_BANK2_PIN05 |	/* Pin 125, SSP1_DATA3 */
//	    HW_PINCTRL_PULL2_BANK2_PIN04 |	/* Pin 124, SSP1_DATA2 */
//	    HW_PINCTRL_PULL2_BANK2_PIN03 |	/* Pin 123, SSP1_DATA1 */
//	    HW_PINCTRL_PULL2_BANK2_PIN02 |	/* Pin 122, SSP1_DATA0 */
//	    HW_PINCTRL_PULL2_BANK2_PIN00	/* Pin 121, SSP1_CMD */
//	));

	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_PULL2_SET, (
	    HW_PINCTRL_PULL2_BANK2_PIN05 |	/* Pin 125, SSP1_DATA3 */
	    HW_PINCTRL_PULL2_BANK2_PIN04 |	/* Pin 124, SSP1_DATA2 */
	    HW_PINCTRL_PULL2_BANK2_PIN03 |	/* Pin 123, SSP1_DATA1 */
	    HW_PINCTRL_PULL2_BANK2_PIN02 |	/* Pin 122, SSP1_DATA0 */
	    HW_PINCTRL_PULL2_BANK2_PIN00	/* Pin 121, SSP1_CMD */
	));

	return;
}

/*
 * Configure Debug UART MUX.
 */
static
void configure_dbuart_mux(void)
{
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_MUXSEL3_CLR,
	    __SHIFTIN(0x3, HW_PINCTRL_MUXSEL3_BANK1_PIN27) |
	    __SHIFTIN(0x3, HW_PINCTRL_MUXSEL3_BANK1_PIN26));
	REG_WR(HW_PINCTRL_BASE + HW_PINCTRL_MUXSEL3_SET,
	    __SHIFTIN(0x2, HW_PINCTRL_MUXSEL3_BANK1_PIN27) |
	    __SHIFTIN(0x2, HW_PINCTRL_MUXSEL3_BANK1_PIN26));

	return;
}