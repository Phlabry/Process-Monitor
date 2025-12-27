// src/source/getprocs.cpp
#include "../header/getprocs.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

HANDLE GetProcessSnap() {
    HANDLE hProcessSnap; // Snapshot of processes
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE){
        perror("Failed to take a snapshot");
        GetLastError();
    }

    return hProcessSnap;
}

bool GetFirstProcess(HANDLE snap, PROCESSENTRY32W& pe) {
    pe.dwSize = sizeof(pe);
    return Process32FirstW(snap, &pe);
}

bool GetNextProcess(HANDLE snap, PROCESSENTRY32W& pe) {
    return Process32NextW(snap, &pe);
}

vector<ProcInfo> GetProcVector(HANDLE snap, PROCESSENTRY32W& pe){
    vector<ProcInfo> procs;
    if(GetFirstProcess(snap, pe)){
        do {
            ProcInfo p;
            p.szExeFile = pe.szExeFile;
            p.th32ProcessID = pe.th32ProcessID;
            p.cntThreads = pe.cntThreads;
            p.dwSize = pe.dwSize;
            p.pcPriClassBase = pe.pcPriClassBase;
            p.th32ParentProcessID = pe.th32ParentProcessID;

            procs.push_back(move(p));
        } while(GetNextProcess(snap, pe));
    } else {
        perror("GetProcVector has Failed");
        GetLastError();
        return {};
    }
    
    CloseHandle(snap);
    return procs;
}