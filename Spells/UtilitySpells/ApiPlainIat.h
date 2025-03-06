#pragma once
#include <stdio.h>
#include <Windows.h>

extern decltype(VirtualAllocEx)* pVirtualAllocEx = VirtualAllocEx;
extern decltype(VirtualAlloc)* pVirtualAlloc = VirtualAlloc;
extern decltype(VirtualProtectEx)* pVirtualProtectEx = VirtualProtectEx;
extern decltype(VirtualProtect)* pVirtualProtect = VirtualProtect;
extern decltype(CreateThread)* pCreateThread = CreateThread;
extern decltype(CreateRemoteThread)* pCreateRemoteThread = CreateRemoteThread;
extern decltype(GetThreadContext)* pGetThreadContext = GetThreadContext;
extern decltype(SetThreadContext)* pSetThreadContext = SetThreadContext;
extern decltype(ResumeThread)* pResumeThread = ResumeThread;
extern decltype(WriteProcessMemory)* pWriteProcessMemory = WriteProcessMemory;
extern decltype(OpenProcess)* pOpenProcess = OpenProcess;
extern decltype(QueueUserAPC)* pQueueUserAPC = QueueUserAPC;