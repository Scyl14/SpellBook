def set_payload_location():
    PayloadLocation = input("""\nSet Payload Location:
    1. Local File
    2. Remote Url
                            
    >> """)
    return PayloadLocation

def set_payload_path():
    PayloadPath = input("""\nSet Payload Path: 

    >> """)
    return PayloadPath

def set_payload_encryption():
    PayloadEncryption = input("""\nSet Payload  Obfuscation/Encryption:
    1. XOR
    2. RC4
    3. AES
    4. IPv4Fuscation
    5. MACFuscation
    6. UUIDFuscation
    7. Plain (No Encryption)
                                       
    >> """)
    return PayloadEncryption

def set_url():
    Url = input("""\nSet Payload Url or type local For localhost:8000 (test): 

    >> """)
    if Url == "local":
        Url = "http://localhost:8000/payload.bin"
    return Url

def set_loader_type():
    Loader = input("""\nEnter Injection Techinque: 
    1. Local Injection
    2. Remote Process Injection

    >> """)
    return Loader

def set_local_loader_type():
    Loader = input("""\nEnter Injection Techinque: 
    1. Local Injection
    2. Local Thread Hijacking (Dummy Function)
    3. Local APC Injection

    >> """)
    if Loader == "1":
        Loader = "LocalInjection.h"
    if Loader == "2":
        Loader = "LocalThreadHijacking.h"
    if Loader == "3":
        Loader = "LocalApcInjection.h"
    return Loader

def set_remote_loader_type():
    Loader = input("""\nEnter Injection Techinque:
    1. Remote Process Injection - (CreateRemoteThread)
    2. Remote Thread Hijacking
    3. Early Bird APC Injection 
                       
    >> """)
    if Loader == "1":
        Loader = "RemoteInjection.h"
    if Loader == "2":
        Loader = "RemoteThreadHijacking.h"
    if Loader == "3":
        Loader = "EarlyBirdApcInjection.h"
    return Loader


def set_process_name():
    ProcessName = input("""\nEnter Process Name: 

    >> """)
    return ProcessName

def get_process_type():

    GetProcessType = input("""\nHow do you Want to Get Process ID:
    1. Create Process/Thread (Your Own Process)
    2. Enumerate Processes/Threads  (Remote Existent Process) 
    
    NOTE: DO NOT USE OPTION 2 WITH REMOTE THREAD HIJACKING AND APC INJECTION IF YOU ARE NOT AN ADULT!!

    >> """)
    return GetProcessType

def set_enum_type():
    Enumeration = input("""\nEnter Process/Thread Enumeration Techinque: 
    1. Process Enumeration via Snapshot
    1. Process Enumeration via NtQuerySystemInformation

    >> """)
    if Enumeration == "1":
        Enumeration = "ProcessEnum32Snapshot.h"
    if Enumeration == "2":
        Enumeration = "ProcessEnumNtQuery.h"
    return Enumeration

def set_proc_creation_type():   
    Enumeration = input("""\nEnter Process Creation Type:
    1. Create Suspended Process
    2. Create Debugged Process

    >> """)
    if Enumeration == "1":
        Enumeration = "CreateSuspended.h"
    if Enumeration == "2":
        Enumeration = "CreateDebugged.h"
    return Enumeration

def set_api_mode():
    ApiMode = input("""\nSelect API Hashing Mode:
    1. Plain IAT
    2. IAT Hiding (No Hashing)
    3. Hashing (Rotr32)

    >> """)
    if ApiMode == "1":
        ApiMode = "ApiPlainIat.h"
    elif ApiMode == "2":
        ApiMode = "ApiNoHash.h"
    elif ApiMode == "3":
        ApiMode =  "ApiCtHash.h"
    return ApiMode