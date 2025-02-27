#pragma once
#ifndef XOR
#define XOR
#include <stdio.h>
#include <Windows.h>

VOID Encrypt(IN PBYTE pShellcode, IN SIZE_T sShellcodeSize) {
	char bKey[] = { 0x00, 0x11, 0x22 };
	size_t sKeySize = sizeof(bKey);
	PBYTE pbKey = (PBYTE)bKey;

	for (SIZE_T i = 0x00, j = 0x00; i < sShellcodeSize; i++, j++) {

		if (j == sKeySize)
			j = 0x00;

		if (i % 2 == 0)
			pShellcode[i] = pShellcode[i] ^ pbKey[j];
		else
			pShellcode[i] = pShellcode[i] ^ pbKey[j] ^ j;
	}
}

#endif