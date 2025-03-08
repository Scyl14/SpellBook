#ifdef NO_WINTERNL
#define _WINTERNL_
#endif

#pragma once
#include <stdio.h>
#include <Windows.h>
//#include <winternl.h>
#include "api.h"
#include "../UtilitySpells/Structs.h"

typedef NTSTATUS (NTAPI* fnNtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

BOOL GetRemoteProcess(IN LPWSTR szProcessName, OUT PDWORD pdwProcessID, OUT OPTIONAL PHANDLE phProcess, OUT OPTIONAL HANDLE* hThread) {

	GetTickCount();

	NTSTATUS						STATUS							= 0x00;
	fnNtQuerySystemInformation		pNtQuerySystemInformation		= NULL;
	WCHAR							wcUpperCaseProcName[MAX_PATH]	= { 0x00 };
	ULONG							uArrayLength					= 0x00;
	PSYSTEM_PROCESS_INFORMATION		pSystemProcInfo					= NULL;
	PBYTE							pTmpPntrVar						= NULL;

	if (!szProcessName || !pdwProcessID || lstrlenW(szProcessName) >= MAX_PATH)
		return FALSE;

	for (int i = 0; i < lstrlenW(szProcessName); i++) {
		if (szProcessName[i] >= 'a' && szProcessName[i] <= 'z')
			wcUpperCaseProcName[i] = szProcessName[i] - 'a' + 'A';
		else
			wcUpperCaseProcName[i] = szProcessName[i];
	}

	if (!(pNtQuerySystemInformation = (fnNtQuerySystemInformation)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtQuerySystemInformation"))) {
		printf("[!] GetProcAddress Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	STATUS = pNtQuerySystemInformation(SystemProcessInformation, NULL, NULL, &uArrayLength);

	if (!(pTmpPntrVar = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, uArrayLength))) {
		printf("[!] HeapAlloc Failed With Error: 0x%0.8X \n", STATUS);
		goto _END_OF_FUNC;
	}

	if (!NT_SUCCESS((STATUS = pNtQuerySystemInformation(SystemProcessInformation, pTmpPntrVar, uArrayLength, NULL)))) {
		printf("[!] NtQuerySystemInformation Failed With Error: 0x%0.8X \n", STATUS);
		goto _END_OF_FUNC;
	}

	pSystemProcInfo = (PSYSTEM_PROCESS_INFORMATION)pTmpPntrVar;

	GetTickCount();

	while (pSystemProcInfo->NextEntryOffset) {

		WCHAR	szUprProcName[MAX_PATH] = { 0x00 };

		if (!pSystemProcInfo->ImageName.Length || pSystemProcInfo->ImageName.Length >= MAX_PATH)
			goto _NEXT_ELEMENT;

		for (int i = 0; i < pSystemProcInfo->ImageName.Length; i++) {
			if (pSystemProcInfo->ImageName.Buffer[i] >= 'a' && pSystemProcInfo->ImageName.Buffer[i] <= 'z')
				szUprProcName[i] = pSystemProcInfo->ImageName.Buffer[i] - 'a' + 'A';
			else
				szUprProcName[i] = pSystemProcInfo->ImageName.Buffer[i];
		}

		GetTickCount();

		if (wcscmp(wcUpperCaseProcName, szUprProcName) == 0x00) {
			if (phProcess)
				*phProcess = pOpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD64)pSystemProcInfo->UniqueProcessId);
			
			*pdwProcessID = (DWORD64)pSystemProcInfo->UniqueProcessId;
			

			break;
		}

		GetTickCount();

_NEXT_ELEMENT:
		pSystemProcInfo = (PSYSTEM_PROCESS_INFORMATION)((ULONG_PTR)pSystemProcInfo + pSystemProcInfo->NextEntryOffset);
	}

_END_OF_FUNC:
	if (pTmpPntrVar)
		HeapFree(GetProcessHeap(), 0x00, pTmpPntrVar);
	return (*pdwProcessID) ? TRUE : FALSE;
}