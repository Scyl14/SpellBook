#pragma once
#include <stdio.h>
#include <Windows.h>


//VirtualAllocEx
typedef LPVOID (WINAPI* fnVirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
extern fnVirtualAllocEx pVirtualAllocEx = (fnVirtualAllocEx)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "VirtualAllocEx");

//VirtualAlloc
typedef LPVOID (WINAPI* fnVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
extern fnVirtualAlloc pVirtualAlloc = (fnVirtualAlloc)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "VirtualAlloc");

//VirtualProtectEx
typedef BOOL (WINAPI* fnVirtualProtectEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
extern fnVirtualProtectEx pVirtualProtectEx = (fnVirtualProtectEx)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "VirtualProtectEx");

//VirtualProtect
typedef BOOL (WINAPI* fnVirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
extern fnVirtualProtect pVirtualProtect = (fnVirtualProtect)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "VirtualProtect");

//CreateThread
typedef HANDLE (WINAPI* fnCreateThread)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
extern fnCreateThread pCreateThread = (fnCreateThread)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "CreateThread");

//CreateRemoteThread
typedef HANDLE (WINAPI* fnCreateRemoteThread)(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
extern fnCreateRemoteThread pCreateRemoteThread = (fnCreateRemoteThread)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "CreateRemoteThread");

//GetThreadContext
typedef BOOL (WINAPI* fnGetThreadContext)(HANDLE hThread, LPCONTEXT lpContext);
extern fnGetThreadContext pGetThreadContext = (fnGetThreadContext)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "GetThreadContext");

//SetThreadContext
typedef BOOL (WINAPI* fnSetThreadContext)(HANDLE hThread, const CONTEXT* lpContext);
extern fnSetThreadContext pSetThreadContext = (fnSetThreadContext)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "SetThreadContext");

//ResumeThread
typedef DWORD (WINAPI* fnResumeThread)(HANDLE hThread);
extern fnResumeThread pResumeThread = (fnResumeThread)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "ResumeThread");

//WriteProcessMemory
typedef BOOL (WINAPI* fnWriteProcessMemory)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);
extern fnWriteProcessMemory pWriteProcessMemory = (fnWriteProcessMemory)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "WriteProcessMemory");

//OpenProcess
typedef HANDLE (WINAPI* fnOpenProcess)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
extern fnOpenProcess pOpenProcess = (fnOpenProcess)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "OpenProcess");
