/*=============================================================================
*	
*	Copyright (c) 2000 Arthur Boynagryan (a.k.a. LayZee_Cat).
*		All rights reserved.
*    
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer as
*	   the first lines of this file unmodified.
*	2. Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*
*	THIS SOFTWARE IS PROVIDED BY ARTHUR BOYNAGRYAN "AS IS" AND ANY EXPRESS OR
*	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*	IN NO EVENT SHALL ARTHUR BOYNAGRYAN BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
*	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
*	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*	The author of this program may be contacted at boynagar@armentel.com
*
*=============================================================================
*/

#ifndef _LOCKSCR_H_
#define _LOCKSCR_H_

#pragma once
#pragma warning(disable: 4711)	// disable Level 4 warning: automatic inline expansion

/* Insert your headers here */
#define WIN32_LEAN_AND_MEAN		/* Exclude rarely-used stuff from Windows headers */
#include <windows.h>
#include <string.h>

#ifdef LOCKSCR_EXPORTS
#define LOCKSCR_API __declspec(dllexport)
#else
#define LOCKSCR_API __declspec(dllimport)
#endif

void LOCKSCR_API SetKeyboardHook(HHOOK hHook);
void LOCKSCR_API SetMouseHook(HHOOK hHook);
LRESULT LOCKSCR_API CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT LOCKSCR_API CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif /* _LOCKSCR_H_ */