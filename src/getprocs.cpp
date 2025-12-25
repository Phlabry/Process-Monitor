// getprocs.cpp
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <iomanip>

using namespace std;

HANDLE GetProcessSnap();
PROCESSENTRY32 GetProcessEntry();

HANDLE GetProcessSnap() {
    HANDLE hProcessSnap; // Snapshot of processes
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE){
        perror("Failed to take a snapshot");
        GetLastError();
    }

    return hProcessSnap;
}

bool GetFirstProcess(HANDLE snap, PROCESSENTRY32& pe) {
    pe.dwSize = sizeof(pe);
    return Process32First(snap, &pe);
}

bool GetNextProcess(HANDLE snap, PROCESSENTRY32& pe) {
    return Process32Next(snap, &pe);
}