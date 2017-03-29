/*
 * main.cpp -- Paddle Controller DS Sample Program
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

#include <nds.h>
#include <stdio.h>
#include "paddlelib.h"

SpriteEntry sprites[128];

void init(void) {
	
	powerON(POWER_ALL_2D);

	irqInit();
	irqEnable(IRQ_VBLANK);
	
	vramSetMainBanks(	VRAM_A_MAIN_SPRITE,
						VRAM_B_MAIN_SPRITE,
						VRAM_C_MAIN_BG_0x06000000,
						VRAM_D_LCD); 
	videoSetMode(	MODE_0_2D |
					DISPLAY_SPR_ACTIVE |
					DISPLAY_BG0_ACTIVE |
					DISPLAY_SPR_1D |
					DISPLAY_SPR_1D_BMP);
	
	BG_PALETTE[0]   = RGB15( 0,  0,  0);
	BG_PALETTE[255] = RGB15(31, 31, 31);
	BG0_CR = BG_MAP_BASE(31);
	
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK(31), (u16*)CHAR_BASE_BLOCK(0), 16);
	
}

void initSprites(void) {
	
	for (int i = 0; i < 128; i++) {
		sprites[i].attribute[0] = ATTR0_DISABLED;
		sprites[i].attribute[1] = 0;
		sprites[i].attribute[2] = 0;
		sprites[i].attribute[3] = 0;
	}
	
	sprites[0].attribute[0] = ATTR0_COLOR_256 | 0;
	sprites[0].attribute[1] = ATTR1_SIZE_8 | 0;
	sprites[0].attribute[2] = 64;
	SPRITE_PALETTE[1] = RGB15(31,31,31);
	for (int i = 0; i < 32*16; i++) {
		SPRITE_GFX[i+64*16] = (1<<8) | 1;
	}
	
}


void updateOAM(void) {
	
	DC_FlushRange(sprites, 128*sizeof(SpriteEntry));
	dmaCopy(sprites, OAM, 128 * sizeof(SpriteEntry));
	
}


void setSpriteXY(int sprite, int x, int y) {
	
	sprites[sprite].attribute[0] = (sprites[sprite].attribute[0] & ~0xFF) | y;
	sprites[sprite].attribute[1] = (sprites[sprite].attribute[1] & ~0xFF) | x;
	
}


int main(void) {

	init();
	initSprites();

	iprintf("\x1b[1;1HPaddleController Check Program");
	iprintf("\x1b[2;1H                   by eagle0wl");

	Paddle_Init();

	if (!Paddle_IsExist()) {
	iprintf("\x1b[4;1HPaddleController is not Found.");
		while (1) {
			swiWaitForVBlank();
		}
	}

	Paddle_Enable();
	while (1) {
		if (!Paddle_IsExist()) {
			iprintf("\x1b[7;1HPullout!");
			break;
		}
		
		s16 pos = Paddle_ReadEx();
		s16 val = Paddle_Read();
		
		iprintf("\x1b[4;1Hpos = %04X", pos);
		iprintf("\x1b[5;1Hval = %04d", val);
		
		int x = 128 + val - 4;
		if (256 - 8 < x) x = 256 - 8;
		if (      0 > x) x =   0;
		
		setSpriteXY(0, x, 96);
		updateOAM();
		
		swiWaitForVBlank();
	}
	
	return 0;
}
