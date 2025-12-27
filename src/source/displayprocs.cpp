// src/source/displayprocs.cpp
#include "../header/sortprocs.h"
#include "../header/getprocs.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <sstream>
#include <string>

using namespace std;

void DisplayProcs(keylist key = keylist::name, bool asc = false) {
    HANDLE snap = GetProcessSnap();
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    vector<ProcInfo> procs = GetProcVector(snap, pe32);
    int len = (int)procs.size();

    SortProcs(procs, key, asc);

    int namew = (int)wcslen(L"PROCESS NAME");
    int idw = (int)wcslen(L"ID");
    int threadsw = (int)wcslen(L"THREADS");
    int parentw = (int)wcslen(L"PARENT ID");
    int priow = (int)wcslen(L"PRIORITY");

    for (int i = 0; i < len; i++) {
        // Name
        if ((int)procs[i].szExeFile.size() > namew) {
            namew = (int)procs[i].szExeFile.size();
        }

        // Numbers: compare digit lengths
        int cur;

        cur = (int)to_wstring(procs[i].th32ProcessID).size();
        if (cur > idw) idw = cur;

        cur = (int)to_wstring(procs[i].cntThreads).size();
        if (cur > threadsw) threadsw = cur;

        cur = (int)to_wstring(procs[i].th32ParentProcessID).size();
        if (cur > parentw) parentw = cur;

        cur = (int)to_wstring(procs[i].pcPriClassBase).size();
        if (cur > priow) priow = cur;
    }

    const int pad = 2;
    namew += pad; idw += pad; threadsw += pad; parentw += pad; priow += pad;

    std::wostringstream out;

    // Header 
    out << left
        << setw(namew) << L"PROCESS NAME"
        << setw(idw) << L"ID"
        << setw(threadsw)<< L"THREADS"
        << setw(parentw) << L"PARENT ID"
        << setw(priow) << L"PRIORITY"
        << endl;

    // Rows
    for (int i = 0; i < len; i++) {
        out << left
            << setw(namew) << procs[i].szExeFile
            << right
            << setw(idw) << procs[i].th32ProcessID
            << setw(threadsw) << procs[i].cntThreads
            << setw(parentw) << procs[i].th32ParentProcessID
            << setw(priow) << procs[i].pcPriClassBase
            << endl;
    }

    out << L"Total Process Count: " << len << endl;

    wcout << out.str();
}

void CycleDisplay() {
    while (true){
        system("cls");
        DisplayProcs();
        Sleep(100000);
    }
}
