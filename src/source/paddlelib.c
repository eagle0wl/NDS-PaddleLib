/*
 * paddlelib.c -- Paddle Controller DS Library
 * 
 * Copyright (C) 2008 eagle0wl (masm0wl@hotmail.com)
 * 
 * ChangeLog:
 * 2008-05-05	eagle0wl	 first release.
 * 
 * The MIT License
 * 
 * Copyright (C) 2008 eagle0wl (masm0wl@hotmail.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */
#include <nds/jtypes.h>
#include <nds/memory.h>
#include <stdlib.h>

#define RAMREGION_CTRLCYCLE_MASK	0x00000003
#define RAMREGION_CTRLCYCLE_10		0x00000000
#define RAMREGION_CTRLCYCLE_8		0x00000001
#define RAMREGION_CTRLCYCLE_6		0x00000002
#define RAMREGION_CTRLCYCLE_18		0x00000003

#define PHITERM_OUTCTRL_MASK		0x00000050
#define PHITERM_OUTCTRL_LOWLEVEL	0x00000020
#define PHITERM_OUTCTRL_4_19MHZ		0x00000030
#define PHITERM_OUTCTRL_8_38MHZ		0x00000040
#define PHITERM_OUTCTRL_16_76MHZ	0x00000050

#define PADDLE_STATUS1 (*(vu8 *)0x0A000000)
#define PADDLE_STATUS2 (*(vu8 *)0x0A000001)

static bool isEnabled = false;
static u16 paddleOld;


void Paddle_Init(void) {
	
	REG_EXMEMCNT &= (~ARM7_OWNS_ROM & ~RAMREGION_CTRLCYCLE_MASK & ~PHITERM_OUTCTRL_MASK);
	// RAM-region access cycle control = 18 cycles
	// PHI-terminal output control = Lowlevel
	REG_EXMEMCNT |= (RAMREGION_CTRLCYCLE_18 | PHITERM_OUTCTRL_LOWLEVEL);
	
	isEnabled = false;
	
}


bool Paddle_IsExist(void) {
	
	if (0xFFFF == *(vu16 *)0x027FFC30) {
		return false;
	}
	
	if (*(vu8 *)0x027FFC35 & 0x02) {
		return false;
	}
	
	if (0x96 == *(vu8 *)0x080000B2) {
		if (*(vu16 *)0x027FFC30 == *(vu16 *)0x080000BE) {
			*(vu8 *)0x027FFC35 |= 0x02;
			return false;
		}
	} else {
		if (*(vu16 *)0x027FFC30 != *(vu16 *)0x0801FFFE) {
			*(vu8 *)0x027FFC35 |= 0x02;
			return false;
		}
	}
	
	if (*(vu32 *)0x027FFC38 != *(vu32 *)0x080000AC) {
		*(vu8 *)0x027FFC35 |= 0x02;
		return false;
	}
	
	if (0xFFFF == *(vu16 *)0x027FFC30
	 || 0x0000 == *(vu16 *)0x027FFC30) {
		return false;
	}
	
	if (0x0F == (*(vu8 *)0x027FFC31 & 0x1F)) {
		return true;
	}
	
	if (*(vu8 *)0x027FFC31 & 0x80) {
		return false;
	}
	
	if (0x0F == *(vu8 *)0x027FFC32) {
		return true;
	}
	
	if (0x0F == *(vu8 *)0x027FFC33) {
		return true;
	}
	
	return false;
	
}


void Paddle_Enable(void) {
	
	paddleOld = PADDLE_STATUS1 + ((PADDLE_STATUS2 & 0x0F) * 256);
	isEnabled = true;
	
}


void Paddle_Disable(void) {
	
	isEnabled = false;
	
}


s16 Paddle_ReadEx(void) {
	
	if (!isEnabled) {
		return 0;
	}
	
	return PADDLE_STATUS1 + ((PADDLE_STATUS2 & 0x0F) * 256);
	
}


s16 Paddle_Read(void) {
	
	u16 paddle;
	s16 retvalue;
	
	if (!isEnabled) {
		return 0;
	}
	
	paddle = PADDLE_STATUS1 + ((PADDLE_STATUS2 & 0x0F) * 256);
	
	if (0x07FF < abs(paddle - paddleOld)) {
		if (paddle < paddleOld) {
			retvalue = 0x1000 - (paddleOld - paddle);
		} else {
			retvalue = (paddle - paddleOld) - 0x1000;
		}
		
	} else {
		retvalue = paddle - paddleOld;
		
	}
	
	paddleOld = paddle;
	
	return retvalue;
	
}
