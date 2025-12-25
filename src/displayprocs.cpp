// displayprocs.cpp
#include "getprocs.cpp"
#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

void DisplayProcs() {
    HANDLE snap = GetProcessSnap();
    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(snap == INVALID_HANDLE_VALUE || !Process32First(snap, &pe32)){
        GetLastError();
        CloseHandle(snap);
        return;
    }

    std::ostringstream out;

    // Header
    out <<
        "PROCESS NAME" << setw(25) <<
        "ID" << setw(25) <<
        "THREADS" << setw(25) <<
        "PARENT ID" << setw(25) <<
        "PRIORITY"
        << endl;

    int procCount = 0;

    cout << snap << endl;
    while (Process32Next(snap, &pe32)) {
        out << setw(25)
            << pe32.szExeFile << setw(25)
            << pe32.th32ProcessID << setw(25)
            << pe32.cntThreads << setw(25)
            << pe32.th32ParentProcessID << setw(25)
            << pe32.pcPriClassBase
            << endl;
        procCount++;
    }

    out << "Total Process Count: " << procCount << endl;

    cout << out.str();

    CloseHandle(snap);
}

void CycleDisplay() {
    while (true){
        system("cls");
        DisplayProcs();
        Sleep(5000);
    }
}