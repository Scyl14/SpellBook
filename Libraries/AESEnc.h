#include "TinyAES.h"
#include <stdio.h>
#include <Windows.h>

unsigned char pIv[] = {
    0x00, 0xB8, 0x80, 0x7E, 0xF0, 0x09, 0x65, 0x8B, 0xD6, 0x6E, 0x2D, 0x8B, 0x0C, 0x6A, 0xA2, 0x34 };

BOOL Encrypt(IN PBYTE pRawDataBuffer, IN SIZE_T sRawBufferSize, IN PBYTE pAesKey, IN OPTIONAL SIZE_T sAesKeySize) {

	if (!pRawDataBuffer || !sRawBufferSize || !pAesKey || !pIv)
		return FALSE;
    
	pRawDataBuffer;
	sRawBufferSize;
	struct AES_ctx AesCtx	= { 0x00 };
    PBYTE pAesIv = (PBYTE)pIv;

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

	if (!pCipherTextBuffer || !sCipherTextSize || !pAesKey || !pIv)
		return FALSE;

    PBYTE pAesIv = (PBYTE)pIv;
	RtlSecureZeroMemory(&AesCtx, sizeof(AesCtx));
	AES_init_ctx_iv(&AesCtx, pAesKey, (const uint8_t *)pAesIv);
	AES_CBC_decrypt_buffer(&AesCtx, pCipherTextBuffer, sCipherTextSize);

	return TRUE;
}