#pragma once
#include "CompTimeApiHash.h"
#include <stdio.h>
#include <Windows.h>
#define CTIME_HASHA( API ) constexpr auto API##_Rotr32A = HashStringDjb2A((const char*) #API);
#define CTIME_HASHW( API ) constexpr auto API##_Rotr32W = HashStringDjb2W((const wchar_t*) L#API);

CTIME_HASHA(VirtualAllocEx);
CTIME_HASHA(VirtualAlloc);
CTIME_HASHA(VirtualProtectEx);
CTIME_HASHA(VirtualProtect);
CTIME_HASHA(CreateThread);
CTIME_HASHA(CreateRemoteThread);
CTIME_HASHA(CreateProcessW);
CTIME_HASHA(GetThreadContext);
CTIME_HASHA(SetThreadContext);
CTIME_HASHA(ResumeThread);
CTIME_HASHA(WriteProcessMemory);
CTIME_HASHA(OpenProcess);
CTIME_HASHA(QueueUserAPC);

//VirtualAllocEx
typedef LPVOID (WINAPI* fnVirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
extern fnVirtualAllocEx pVirtualAllocEx = (fnVirtualAllocEx)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), VirtualAllocEx_Rotr32A);

//VirtualAlloc
typedef LPVOID (WINAPI* fnVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
extern fnVirtualAlloc pVirtualAlloc = (fnVirtualAlloc)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), VirtualAlloc_Rotr32A);

//VirtualProtectEx
typedef BOOL (WINAPI* fnVirtualProtectEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
extern fnVirtualProtectEx pVirtualProtectEx = (fnVirtualProtectEx)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), VirtualProtectEx_Rotr32A);

//VirtualProtect
typedef BOOL (WINAPI* fnVirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
extern fnVirtualProtect pVirtualProtect = (fnVirtualProtect)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), VirtualProtect_Rotr32A);

//CreateThread
typedef HANDLE (WINAPI* fnCreateThread)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
extern fnCreateThread pCreateThread = (fnCreateThread)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), CreateThread_Rotr32A);

//CreateRemoteThread
typedef HANDLE (WINAPI* fnCreateRemoteThread)(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
extern fnCreateRemoteThread pCreateRemoteThread = (fnCreateRemoteThread)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), CreateRemoteThread_Rotr32A);

//CreateProcessW
typedef BOOL (WINAPI* fnCreateProcessW)(LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
extern fnCreateProcessW pCreateProcessW = (fnCreateProcessW)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), CreateProcessW_Rotr32A);

//GetThreadContext
typedef BOOL (WINAPI* fnGetThreadContext)(HANDLE hThread, LPCONTEXT lpContext);
extern fnGetThreadContext pGetThreadContext = (fnGetThreadContext)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), GetThreadContext_Rotr32A);

//SetThreadContext
typedef BOOL (WINAPI* fnSetThreadContext)(HANDLE hThread, const CONTEXT* lpContext);
extern fnSetThreadContext pSetThreadContext = (fnSetThreadContext)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), SetThreadContext_Rotr32A);

//ResumeThread
typedef DWORD (WINAPI* fnResumeThread)(HANDLE hThread);
extern fnResumeThread pResumeThread = (fnResumeThread)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), ResumeThread_Rotr32A);

//WriteProcessMemory
typedef BOOL (WINAPI* fnWriteProcessMemory)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);
extern fnWriteProcessMemory pWriteProcessMemory = (fnWriteProcessMemory)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), WriteProcessMemory_Rotr32A);

//OpenProcess
typedef HANDLE (WINAPI* fnOpenProcess)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
extern fnOpenProcess pOpenProcess = (fnOpenProcess)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), OpenProcess_Rotr32A);

//QueueUserAPC
typedef DWORD  (WINAPI* fnQueueUserAPC)(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData);
extern fnQueueUserAPC pQueueUserAPC = (fnQueueUserAPC)GetProcAddressH(GetModuleHandleA("KERNEL32.DLL"), QueueUserAPC_Rotr32A);