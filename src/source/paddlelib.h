/*
 * paddlelib.h -- Paddle Controller DS Library
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
#ifndef _PADDLE_H_
#define _PADDLE_H_
#include <nds/jtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void Paddle_Init(void);

bool Paddle_IsExist(void);

void Paddle_Enable(void);

void Paddle_Disable(void);

s16 Paddle_ReadEx(void);

s16 Paddle_Read(void);


#ifdef __cplusplus
}
#endif

#endif // _PADDLE_H_
