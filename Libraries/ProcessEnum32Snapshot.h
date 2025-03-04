#pragma once
#include "api.h"
#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>

using namespace std;

#include <tlhelp32.h>

BOOL GetRemoteProcess(IN LPWSTR szProcessName, OUT PDWORD pdwProcessID, OUT OPTIONAL PHANDLE phProcess) {

	PROCESSENTRY32  ProcEntry						= { .dwSize = sizeof(PROCESSENTRY32) };
	WCHAR			wcUpperCaseProcName[MAX_PATH]	= { 0x00 };
	HANDLE			hSnapShot						= INVALID_HANDLE_VALUE;

	if (!szProcessName || !pdwProcessID || lstrlenW(szProcessName) >= MAX_PATH)
		return FALSE;

	for (int i = 0; i < lstrlenW(szProcessName); i++){
		if (szProcessName[i] >= 'a' && szProcessName[i] <= 'z')
			wcUpperCaseProcName[i] = szProcessName[i] - 'a' + 'A';
		else
			wcUpperCaseProcName[i] = szProcessName[i];
	}

	if ((hSnapShot = pCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) == INVALID_HANDLE_VALUE) {
		printf("[!] CreateToolhelp32Snapshot Failed With Error: %d \n", GetLastError());
		return FALSE;
	}


	if (!Process32First(hSnapShot, &ProcEntry)) {
		printf("[!] Process32First Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	do {

		WCHAR	szUprProcName[MAX_PATH] = { 0x00 };

		if (ProcEntry.szExeFile && lstrlen(ProcEntry.szExeFile) < MAX_PATH) {

			RtlSecureZeroMemory(szUprProcName, sizeof(szUprProcName));

			for (int i = 0; i < lstrlen(ProcEntry.szExeFile); i++) {
				if (ProcEntry.szExeFile[i] >= 'a' && ProcEntry.szExeFile[i] <= 'z')
					szUprProcName[i] = ProcEntry.szExeFile[i] - 'a' + 'A';
				else
					szUprProcName[i] = ProcEntry.szExeFile[i];
			}

		}

		if (wcscmp(szUprProcName, wcUpperCaseProcName) == 0x00) {
			if (phProcess)
				*phProcess	= pOpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcEntry.th32ProcessID);

			*pdwProcessID	= ProcEntry.th32ProcessID;

			break;
		}

	} while (Process32Next(hSnapShot, &ProcEntry));


_END_OF_FUNC:
	if (hSnapShot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapShot);
	return (*pdwProcessID) ? TRUE : FALSE;
}