#pragma once
#ifndef FETCHFROMURL
#define FETCHFROMURL
#include <stdio.h>
#include <Windows.h>
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")

BOOL FetchFileFromURLA(IN LPCSTR cFileDownloadUrl, OUT PBYTE* ppFileBuffer, OUT PDWORD pdwFileSize) {


	HINTERNET	hInternet = NULL,
		hInternetFile = NULL;

	PBYTE 		pTmpPntr = NULL,
		pFileBuffer = NULL;

	DWORD		dwTmpBytesRead = 0x00,
		dwFileSize = 0x00;

	if (!ppFileBuffer || !pdwFileSize)
		return FALSE;

	if (!(hInternet = InternetOpenA(NULL, 0x00, NULL, NULL, 0x00))) {
		printf("[!] InternetOpenA Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if (!(hInternetFile = InternetOpenUrlA(hInternet, cFileDownloadUrl, NULL, 0x00, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0x00))) {
		printf("[!] InternetOpenUrlA Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}

	if (!(pTmpPntr = (PBYTE)LocalAlloc(LPTR, 1024))) {
		printf("[!] LocalAlloc Failed With Error: %d \n", GetLastError());
		goto _END_OF_FUNC;
	}


	while (TRUE) {

		if (!InternetReadFile(hInternetFile, pTmpPntr, 1024, &dwTmpBytesRead)) {
			printf("[!] InternetReadFile Failed With Error: %d \n", GetLastError());
			goto _END_OF_FUNC;
		}

		dwFileSize += dwTmpBytesRead;

		if (!pFileBuffer)
			pFileBuffer = (PBYTE)LocalAlloc(LPTR, dwTmpBytesRead);
		else
			pFileBuffer = (PBYTE)LocalReAlloc(pFileBuffer, dwFileSize, LMEM_MOVEABLE | LMEM_ZEROINIT);

		if (!pFileBuffer) {
			printf("[!] LocalAlloc/LocalReAlloc [%d] Failed With Error: %d \n", __LINE__, GetLastError());
			goto _END_OF_FUNC;
		}

		memcpy(pFileBuffer + (dwFileSize - dwTmpBytesRead), pTmpPntr, dwTmpBytesRead);
		memset(pTmpPntr, 0x00, dwTmpBytesRead);

		if (dwTmpBytesRead < 1024)
			break;
	}

	*ppFileBuffer = pFileBuffer;
	*pdwFileSize = dwFileSize;

_END_OF_FUNC:
	if (pTmpPntr)
		LocalFree(pTmpPntr);
	if ((!*ppFileBuffer || !*pdwFileSize) && pFileBuffer)
		LocalFree(pFileBuffer);
	if (hInternetFile)
		InternetCloseHandle(hInternetFile);
	if (hInternet)
		InternetCloseHandle(hInternet);
	if (hInternet)
		InternetSetOptionA(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	return (*ppFileBuffer != NULL && *pdwFileSize != 0x00) ? TRUE : FALSE;
}

#endif
