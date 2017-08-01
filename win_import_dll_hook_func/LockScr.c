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

/* LockScr.c: Defines the entry point for the DLL. */

#include "LockScr.h"

#pragma data_seg(".hdata")
HHOOK hKeyHook = NULL;
HHOOK hMouseHook = NULL;
#pragma data_seg()

static char szInput[15];
static char szPassword[15] = "UNL0CK";

void LOCKSCR_API SetKeyboardHook(HHOOK hHook)
{
	hKeyHook = hHook;
}

void LOCKSCR_API SetMouseHook(HHOOK hHook)
{
	hMouseHook = hHook;
}

LRESULT LOCKSCR_API CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char chInput = (char) wParam;
	char *szSearchResult;

	//char temp[20];

	if (nCode == HC_ACTION && lParam & 0x80000000)
	{
		//MessageBox(NULL, &chInput, NULL, MB_OK);
		//MessageBox(NULL, strchr(szPassword, chInput), NULL, MB_OK);

		if (strchr(szPassword, chInput) != NULL)
		{	
			//MessageBox(NULL, "in test", NULL, MB_OK);
			
			lstrcat(szInput, &chInput);

			//MessageBox(NULL, szInput, "Result of lstrcat", MB_OK);

			if (lstrcmpi(szInput, szPassword) == 0)
			{
				UnhookWindowsHookEx(hKeyHook);
				UnhookWindowsHookEx(hMouseHook);
				//MessageBox(NULL, "Finished hook", NULL, MB_OK);
				return 0;
			}
			else
			{
				//MessageBox(NULL, "Compare failed", NULL, MB_OK);
				
				szSearchResult = strstr(szPassword, szInput);
				
				//MessageBox(NULL, szSearchResult, "Search result", MB_OK);				
				//wsprintf(temp,"position: %d",(szSearchResult - szPassword+ 1));
				//MessageBox(NULL, temp, NULL, MB_OK);

				if (szSearchResult == NULL || (szSearchResult - szPassword + 1) > 1)
				{
					//MessageBox(NULL, "not equal", NULL, MB_OK);					
					memset(szInput, '\0', sizeof(szInput));
				}
			}
		}
		else
		{
			//MessageBox(NULL, "in test 1", NULL, MB_OK);
			memset(szInput, '\0', sizeof(szInput));
		}
		//UnhookWindowsHookEx(hKeyHook);
		//MessageBox(NULL, "Finished hook", NULL, MB_OK);
		//return 0;
	}
	
	return nCode < 0 ? CallNextHookEx(hKeyHook, nCode, wParam, lParam) : -1;
}

LRESULT LOCKSCR_API CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_LBUTTONDOWN)
			Beep(500, 200);
	}
	return nCode < 0 ? CallNextHookEx(hKeyHook, nCode, wParam, lParam) : -1;
}

BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to the DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
)
{
	UNREFERENCED_PARAMETER(lpvReserved);
	
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hinstDLL);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			if (hKeyHook != NULL)
				UnhookWindowsHookEx(hKeyHook);
			if (hMouseHook != NULL)
				UnhookWindowsHookEx(hMouseHook);
			break;
	}

	return TRUE;
}
