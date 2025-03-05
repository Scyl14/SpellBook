#include "TinyAES.h"
#include <stdio.h>
#include <Windows.h>

unsigned char AesIv[] = { 0xB4, 0x2C, 0x3D, 0x4E, 0x5F, 0x6A, 0x7B, 0x8C, 0x9D, 0x0E, 0x1F, 0x2A, 0x3B, 0x4C, 0x5D, 0x6E };

BOOL Encrypt(IN PBYTE pRawDataBuffer, IN SIZE_T sRawBufferSize, IN PBYTE pAesKey, IN OPTIONAL SIZE_T sAesKeySize) {

	if (!pRawDataBuffer || !sRawBufferSize || !pAesKey || !AesIv)
		return FALSE;
    
	pRawDataBuffer;
	sRawBufferSize;
	struct AES_ctx AesCtx	= { 0x00 };
    PBYTE pAesIv = (PBYTE)AesIv;

	if (sRawBufferSize % 16 != 0x00) {

		sRawBufferSize		= sRawBufferSize + 16 - (sRawBufferSize % 16);
		pRawDataBuffer		= (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sRawBufferSize);

		if (!pRawDataBuffer) {
			printf("[!] HeapAlloc Failed With Error: %d \n", GetLastError());
			return FALSE;
		}

		memcpy(pRawDataBuffer, pRawDataBuffer, sRawBufferSize);
	}

	RtlSecureZeroMemory(&AesCtx, sizeof(AesCtx));
	AES_init_ctx_iv(&AesCtx, pAesKey, pAesIv);
	AES_CBC_encrypt_buffer(&AesCtx, pRawDataBuffer, sRawBufferSize);

	return TRUE;
}

BOOL Decrypt(IN PBYTE pCipherTextBuffer, IN SIZE_T sCipherTextSize, IN PBYTE pAesKey, IN OPTIONAL SIZE_T pAesKeySize) {

	struct	AES_ctx AesCtx = { 0x00 };

	if (!pCipherTextBuffer || !sCipherTextSize || !pAesKey || !AesIv)
		return FALSE;

    PBYTE pAesIv = (PBYTE)AesIv;
	RtlSecureZeroMemory(&AesCtx, sizeof(AesCtx));
	AES_init_ctx_iv(&AesCtx, pAesKey, (const uint8_t *)pAesIv);
	AES_CBC_decrypt_buffer(&AesCtx, pCipherTextBuffer, sCipherTextSize);

	return TRUE;
}