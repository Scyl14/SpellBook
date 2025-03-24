import os
import re
import subprocess

KeyGuard = None

def get_keyguard():
    global KeyGuard
    if KeyGuard:
        return KeyGuard
    os.system(f"g++ --static -w -o Cast\\KeyGuard Cast\\KeyGuard.c ")
    pattern = re.compile(r'unsigned char (\w+)\[\] = \{[^}]+\};', re.DOTALL)
    hint_pattern = re.compile(r'\[\+\] Using "(0x[0-9A-Fa-f]+)" As A Hint Byte')
    keys = {}
    HintByte = ""
    stdout_text = os.popen("Cast\\KeyGuard.exe 32").read()

    hint_match = hint_pattern.search(stdout_text)
    if hint_match:
        HintByte = hint_match.group(1)

    for match in pattern.finditer(stdout_text):
        key_name = match.group(1)
        key_value = match.group(0)  
        keys[key_name] = key_value

    OriginalKey = keys.get("OriginalKey", "")
    ProtectedKey = keys.get("ProtectedKey", "")
    os.remove("Cast\\KeyGuard.exe")
    KeyGuard = OriginalKey, ProtectedKey, HintByte
    return KeyGuard

def build_encryptor (Encryption, Payload):

    Payload_Str = "static const unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), Payload)) + "};"

    f = open ("Cast\\encryptor.cpp", "a")
    f.write(f"""
#include <iostream>
#include <string> 
#include <Windows.h>
#include <stdio.h>
#include <cassert>
#include "../Spells/EncryptionSpells/{Encryption}"
#include "../Spells/EncryptionSpells/Padding.h"

VOID create_ecrypted_binary_file(PBYTE pPayloadAddress, SIZE_T pPayloadSize)
{{
    FILE *file;
    file = fopen("encrypted.bin", "wb");
    fwrite(pPayloadAddress, pPayloadSize, 1, file);
    fclose(file);
}}

{Payload_Str}

int main()
{{
    PBYTE pPayloadAddress;
    SIZE_T pPayloadSize;  

    pPayloadSize = {len(Payload)};

    pPayloadAddress = (PBYTE)malloc(pPayloadSize);
    if (pPayloadAddress == NULL)
    {{
        printf("Memory allocation failed.\\n");
        return 1;
    }}
    memcpy(pPayloadAddress, Data_RawData, pPayloadSize);
    
    printf("Payload Size: %d\\n", pPayloadSize);

    //SIZE_T payloadSize = (SIZE_T)pPayloadSize;
    PaddBuffer(pPayloadAddress, pPayloadSize, &pPayloadAddress, &pPayloadSize);
    //pPayloadSize = (DWORD)payloadSize;

    {get_keyguard()[0]};

    size_t sKeySize = sizeof(OriginalKey);
    PBYTE pbKey = (PBYTE)OriginalKey;

    assert("ciaone");
    Encrypt(pPayloadAddress, pPayloadSize, pbKey, sKeySize, &pPayloadAddress, &pPayloadSize);

    create_ecrypted_binary_file(pPayloadAddress, pPayloadSize);
    
    free(pPayloadAddress);
            
    return 0;
}}
""")
    
    f.close()
    subprocess.run(f"g++ --static -g -w -o Cast\\encryptor Cast\\encryptor.cpp Cast\\TinyAES.c ")
    result = subprocess.run(["Cast\\encryptor.exe"])
    if result.returncode != 0:
        print("Error: encryptor.exe did not execute successfully.")
        #os.remove("Cast\\encryptor.cpp")
        #os.remove("Cast\\encryptor.exe")
    os.remove("Cast\\encryptor.cpp")
    os.remove("Cast\\encryptor.exe")
    return

def read_encrypted_payload():
    with open("encrypted.bin", "rb") as file:
        content = file.read()
        payload = "static const unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), content)) + "};"
        return payload
