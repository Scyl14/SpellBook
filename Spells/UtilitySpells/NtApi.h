#pragma once
#include <Windows.h>
#include <stdio.h>

LPVOID pVirtualAlloc( LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
    NTSTATUS status = 0x00;
    HANDLE hProcess = GetCurrentProcess();
    LPVOID pAddress = lpAddress;

    // allocating memory
    SET_SYSCALL(g_Nt.NtAllocateVirtualMemory);
    if ((STATUS = RunSyscall(hProcess, &pAddress, 0, &dwSize, flAllocationType, flProtect)) != 0x00 || lpAddress == NULL) {
        printf("[!] NtAllocateVirtualMemory Failed With Error: 0x%0.8X \n", STATUS);
        return NULL;
    }

    return pAddress;

}