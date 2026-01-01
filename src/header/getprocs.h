#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>

struct ProcInfo {
    std::wstring szExeFile;
    DWORD th32ProcessID;
    DWORD cntThreads;
    DWORD dwSize;
    LONG pcPriClassBase;
    DWORD th32ParentProcessID;
};

HANDLE GetProcessSnap();
bool GetFirstProcess(HANDLE snap, PROCESSENTRY32W& pe);
bool GetNextProcess(HANDLE snap, PROCESSENTRY32W& pe);
std::vector<ProcInfo> GetProcVector(HANDLE snap, PROCESSENTRY32W& pe);
