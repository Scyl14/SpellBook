#pragma once
#include <stdio.h>
#include <Windows.h>

BOOL GetRemoteProcess( IN LPWSTR lpProcessName,OUT DWORD* dwProcessId,OUT HANDLE* hProcess,OUT HANDLE* hThread) {

	WCHAR lpPath   [MAX_PATH * 2];
	WCHAR WnDr     [MAX_PATH];

	STARTUPINFOW            Si    = { 0 };
	PROCESS_INFORMATION    Pi    = { 0 };

	// Cleaning the structs by setting the element values to 0
	RtlSecureZeroMemory(&Si, sizeof(STARTUPINFOW));
	RtlSecureZeroMemory(&Pi, sizeof(PROCESS_INFORMATION));

	// Setting the size of the structure
	Si.cb = sizeof(STARTUPINFOW);

	// Getting the %WINDIR% environment variable path (That is generally 'C:\Windows')
	if (!GetEnvironmentVariableW(L"WINDIR", WnDr, MAX_PATH)) {
		printf("[!] GetEnvironmentVariableA Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	// Creating the target process path 
	_swprintf(lpPath, L"%s\\System32\\%s", WnDr, lpProcessName);
	printf("\n\t[i] Running : \"%s\" ... ", lpPath);

	// Creating the process
	if (!CreateProcessW(
		NULL,
		lpPath,
		NULL,
		NULL,
		FALSE,
		DEBUG_PROCESS,		// Instead of CREATE_SUSPENDED		
		NULL,
		NULL,
		&Si,
		&Pi)) {
		printf("[!] CreateProcessA Failed with Error : %d \n", GetLastError());
		return FALSE;
	}

	printf("[+] DONE \n");

	// Filling up the OUTPUT parameter with CreateProcessA's output
	*dwProcessId        = Pi.dwProcessId;
	*hProcess           = Pi.hProcess;
	*hThread            = Pi.hThread;

	// Doing a check to verify we got everything we need
	if (*dwProcessId != NULL && *hProcess != NULL && *hThread != NULL)
		return TRUE;
	
	return FALSE;
}