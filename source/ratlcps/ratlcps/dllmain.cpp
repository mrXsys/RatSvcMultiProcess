// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"


/*
	
	This DLL contains nothing for now, 
	I will Write some code in the feature. 
	this code is for testing purposes

	Thank you, mrxSys :). 

*/

/* CODE FOR THE DLL INJECTOR */
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	switch (ul_reason_for_call)
	{
    case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "Hacked!", "Hacked title!", MB_OK);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

