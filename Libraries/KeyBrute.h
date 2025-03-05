#pragma once
#include <Windows.h>
#include <stdio.h>

PBYTE        pRealKey        =       NULL;

BYTE BruteForceDecryption(IN BYTE HintByte, IN PBYTE pProtectedKey, IN SIZE_T sKey, OUT PBYTE* ppRealKey) {

    BYTE            b = 0;
    INT             i = 0;
    PBYTE           pRealKey = (PBYTE)malloc(sKey);

    if (!pRealKey)
        return NULL;

    while (1) {

        if (((pProtectedKey[0] ^ b) - i) == HintByte)
            break;
        else
            b++;
    }

    for (int i = 0; i < sKey; i++) {
        pRealKey[i] = (BYTE)((pProtectedKey[i] ^ b) - i);
    }

    *ppRealKey = pRealKey;
    return b;
}