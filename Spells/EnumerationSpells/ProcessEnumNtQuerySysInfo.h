#pragma once
#include <stdio.h>
#include <Windows.h>
#include "../UtilitySpells/Structs.h"

typedef NTSTATUS (NTAPI* fnNtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

BOOL GetRemoteProcess(IN LPWSTR szProcessName, OUT PDWORD pdwProcessID, OUT OPTIONAL PHANDLE phProcess, OUT OPTIONAL HANDLE* hThread) {

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

	if (!(pNtQuerySystemInformation = (fnNtQuerySystemInformation)GetProcAddress(GetModuleHandle(L"ntdll"), "NtQuerySystemInformation"))) {
		printf("[!] GetProcAddress Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if ((STATUS = pNtQuerySystemInformation(SystemProcessInformation, NULL, NULL, &uArrayLength)) != STATUS_SUCCESS && STATUS != STATUS_INFO_LENGTH_MISMATCH) {
		printf("[!] NtQuerySystemInformation Failed With Error: 0x%0.8X \n", STATUS);
		goto _END_OF_FUNC;
	}

	if (!(pTmpPntrVar = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, uArrayLength))) {
		pSystemProcInfo = (PSYSTEM_PROCESS_INFORMATION)pTmpPntrVar;
		printf("[!] HeapAlloc Failed With Error: 0x%0.8X \n", STATUS);
		goto _END_OF_FUNC;
	}

	if (!NT_SUCCESS((STATUS = pNtQuerySystemInformation(SystemProcessInformation, pSystemProcInfo, uArrayLength, NULL)))) {
		printf("[!] NtQuerySystemInformation Failed With Error: 0x%0.8X \n", STATUS);
		goto _END_OF_FUNC;
	}

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

		if (wcscmp(wcUpperCaseProcName, szUprProcName) == 0x00) {
			if (phProcess)
				*phProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pSystemProcInfo->UniqueProcessId);

			*pdwProcessID = (DWORD)pSystemProcInfo->UniqueProcessId;

			break;
		}

_NEXT_ELEMENT:
		pSystemProcInfo = (PSYSTEM_PROCESS_INFORMATION)((ULONG_PTR)pSystemProcInfo + pSystemProcInfo->NextEntryOffset);
	}

_END_OF_FUNC:
	if (pTmpPntrVar)
		HeapFree(GetProcessHeap(), 0x00, pTmpPntrVar);
	return (*pdwProcessID) ? TRUE : FALSE;
}