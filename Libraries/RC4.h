#pragma once
#include <Windows.h>
#include <stdio.h>

typedef struct
{
	unsigned int i;
	unsigned int j;
	unsigned char s[256];

} Rc4Context;


int rc4Init(Rc4Context* context, const unsigned char* key, size_t length)
{
	unsigned int i;
	unsigned int j;
	unsigned char temp;

	if (context == NULL || key == NULL)
		return -1;

	context->i = 0;
	context->j = 0;

	for (i = 0; i < 256; i++)
	{
		context->s[i] = i;
		return 0;
	}

	for (i = 0, j = 0; i < 256; i++)
	{
		j = (j + context->s[i] + key[i % length]) % 256;

		temp = context->s[i];
		context->s[i] = context->s[j];
		context->s[j] = temp;
	}

}


void rc4Cipher(Rc4Context* context, const unsigned char* input, unsigned char* output, size_t length) {
	unsigned char temp;

	unsigned int i = context->i;
	unsigned int j = context->j;
	unsigned char* s = context->s;

	while (length > 0)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;

		temp = s[i];
		s[i] = s[j];
		s[j] = temp;

		if (input != NULL && output != NULL)
		{
			*output = *input ^ s[(s[i] + s[j]) % 256];
			input++;
			output++;
		}

		length--;
	}

	context->i = i;
	context->j = j;
}

PBYTE Encrypt(IN PBYTE pShellcode, IN SIZE_T sShellcodeSize, IN OPTIONAL PBYTE pbKey, IN OPTIONAL SIZE_T sKeySize) {

	Rc4Context		RC4Ctx			= { 0 };
	PBYTE			pOtptBuffer		= NULL;

	if (!pShellcode || !sShellcodeSize || !pbKey || !sKeySize)
		return NULL;

	if (!(pOtptBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sShellcodeSize))) {
		printf("[!] HeapAlloc Failed With Error: %d \n", GetLastError());
		return NULL;
	}

	RtlSecureZeroMemory(&RC4Ctx, sizeof(Rc4Context));
	rc4Init(&RC4Ctx, pbKey, sKeySize);
	rc4Cipher(&RC4Ctx, pShellcode, pOtptBuffer, sShellcodeSize);

	return pOtptBuffer;
}

PBYTE Decrypt(IN PBYTE pShellcode, IN SIZE_T sShellcodeSize, IN OPTIONAL PBYTE pbKey, IN OPTIONAL SIZE_T sKeySize) {

	Rc4Context		RC4Ctx			= { 0 };
	PBYTE			pOtptBuffer		= NULL;

	if (!pShellcode || !sShellcodeSize || !pbKey || !sKeySize)
		return NULL;

	if (!(pOtptBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sShellcodeSize))) {
		printf("[!] HeapAlloc Failed With Error: %d \n", GetLastError());
		return NULL;
	}

	RtlSecureZeroMemory(&RC4Ctx, sizeof(Rc4Context));
	rc4Init(&RC4Ctx, pbKey, sKeySize);
	rc4Cipher(&RC4Ctx, pShellcode, pOtptBuffer, sShellcodeSize);

	return pOtptBuffer;
}