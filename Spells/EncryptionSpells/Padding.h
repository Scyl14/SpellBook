#pragma once
#include <Windows.h>
#include <stdio.h>

BOOL PaddBuffer(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* OutputPaddedBuffer, OUT SIZE_T* OutputPaddedSize) {

	PBYTE	PaddedBuffer        = NULL;
	SIZE_T	PaddedSize          = NULL;

	PaddedSize = InputBufferSize + 16 - (InputBufferSize % 16);
	PaddedBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, PaddedSize);
	if (!PaddedBuffer){
		return FALSE;
	}
	ZeroMemory(PaddedBuffer, PaddedSize);
	memcpy(PaddedBuffer, InputBuffer, InputBufferSize);
	*OutputPaddedBuffer = PaddedBuffer;
	*OutputPaddedSize   = PaddedSize;

	return TRUE;
}