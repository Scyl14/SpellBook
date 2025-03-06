#pragma once
#include <Windows.h>
#include <stdio.h>
#include <winternl.h>


#define        SEED       5

// generate a random key (used as initial hash)
constexpr int RandomCompileTimeSeed(void)
{
	return '0' * -40271 +
		__TIME__[7] * 1 +
		__TIME__[6] * 10 +
		__TIME__[4] * 60 +
		__TIME__[3] * 600 +
		__TIME__[1] * 3600 +
		__TIME__[0] * 36000;
};

constexpr auto g_KEY = RandomCompileTimeSeed() % 0xFF;


// Compile time Djb2 hashing function (WIDE)
constexpr DWORD HashStringDjb2W(const wchar_t* String) {
	ULONG Hash = (ULONG)g_KEY;
	INT c = 0;
	while ((c = *String++)) {
		Hash = ((Hash << SEED) + Hash) + c;
	}

	return Hash;
}

// Compile time Djb2 hashing function (ASCII)
constexpr DWORD HashStringDjb2A(const char* String) {
	ULONG Hash = (ULONG)g_KEY;
	INT c = 0;
	while ((c = *String++)) {
		Hash = ((Hash << SEED) + Hash) + c;
	}

	return Hash;
}


// runtime hashing macros 
#define RTIME_HASHA( API ) HashStringDjb2A((const char*) API)
#define RTIME_HASHW( API ) HashStringDjb2W((const wchar_t*) API)


FARPROC GetProcAddressH(HMODULE hModule, DWORD dwApiNameHash) {

	PBYTE pBase = (PBYTE)hModule;

	PIMAGE_DOS_HEADER           pImgDosHdr        = (PIMAGE_DOS_HEADER)pBase;
	if (pImgDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
		return NULL;

	PIMAGE_NT_HEADERS           pImgNtHdrs        = (PIMAGE_NT_HEADERS)(pBase + pImgDosHdr->e_lfanew);
	if (pImgNtHdrs->Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	IMAGE_OPTIONAL_HEADER       ImgOptHdr         = pImgNtHdrs->OptionalHeader;

	PIMAGE_EXPORT_DIRECTORY     pImgExportDir     = (PIMAGE_EXPORT_DIRECTORY)(pBase + ImgOptHdr.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD      FunctionNameArray     = (PDWORD)(pBase + pImgExportDir->AddressOfNames);
	PDWORD      FunctionAddressArray  = (PDWORD)(pBase + pImgExportDir->AddressOfFunctions);
	PWORD       FunctionOrdinalArray  = (PWORD)(pBase + pImgExportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < pImgExportDir->NumberOfFunctions; i++) {
		CHAR*	pFunctionName       = (CHAR*)(pBase + FunctionNameArray[i]);
		PVOID	pFunctionAddress    = (PVOID)(pBase + FunctionAddressArray[FunctionOrdinalArray[i]]);

		if (dwApiNameHash == RTIME_HASHA(pFunctionName)) { // runtime hash value check 
			return (FARPROC)pFunctionAddress;
		}
	}

	return NULL;
}