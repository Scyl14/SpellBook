import os

def build_encryptor (Encryption, Payload):
    f = open ("encryptor.cpp", "a")
    f.write(f"""
#include <iostream>
#include <string> 
#include <Windows.h>
#include <stdio.h>
#include "Libraries/{Encryption}"

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
    char bKey[] = {{ 0x00, 0x11, 0x22 }};
    size_t sKeySize = sizeof(bKey);
    PBYTE pbKey = (PBYTE)bKey;

    Encrypt(pPayloadAddress, pPayloadSize, pbKey, sKeySize);
    create_ecrypted_binary_file(pPayloadAddress, pPayloadSize);
            
    return 0;
}}
""")
    f.close()
    os.system(f"C:\\msys64\\mingw64\\bin\\g++ --static -w -o encryptor encryptor.cpp -mwindows")
    os.system("encryptor.exe")
    os.remove("encryptor.cpp")
    os.remove("encryptor.exe")
    return

def read_encrypted_payload():
    with open("encrypted.bin", "rb") as file:
        content = file.read()
        payload = "unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), content)) + "};"
        return payload