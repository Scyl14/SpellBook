#pragma once
#include <stdio.h>
#include <Windows.h>

VOID Decrypt(IN PBYTE pShellcode, IN SIZE_T sShellcodeSize, IN OPTIONAL PBYTE pbKey, IN OPTIONAL SIZE_T sKeySize) {

	for (SIZE_T i = 0x00, j = 0x00; i < sShellcodeSize; i++, j++) {

		if (j == sKeySize)
			j = 0x00;

		if (i % 2 == 0)
			pShellcode[i] = pShellcode[i] ^ pbKey[j];
		else
			pShellcode[i] = pShellcode[i] ^ pbKey[j] ^ j;
	}
}

VOID Encrypt(IN PBYTE pShellcode, IN SIZE_T sShellcodeSize, IN OPTIONAL PBYTE pbKey, IN OPTIONAL SIZE_T sKeySize) {

	for (SIZE_T i = 0x00, j = 0x00; i < sShellcodeSize; i++, j++) {

		if (j == sKeySize)
			j = 0x00;

		if (i % 2 == 0)
			pShellcode[i] = pShellcode[i] ^ pbKey[j];
		else
			pShellcode[i] = pShellcode[i] ^ pbKey[j] ^ j;
	}
}