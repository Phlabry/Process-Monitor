// src/source/displayprocs.cpp
#include "../header/displayprocs.h"
#include "../header/getprocs.h"
#include "../header/sortprocs.h"
#include "../header/keyactions.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>

// maybe sorting through terminal input. make it executable through terminal and feed paramters for cycleprocs through there.

using namespace std;

void DisplayProcs(KeyList key, bool asc) {
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
        if ((int)procs[i].szExeFile.size() > namew) {
            namew = (int)procs[i].szExeFile.size();
        }

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

    CloseHandle(snap);
    return;
}

// ADD AN INDICATOR FOR SELECTED ROW AND ASC MARKER
// BUFFER FOR THE OUTPUT
// MOUSE POSITION AT START AFTER EACH REFRESH
void CycleDisplay() {
    InitKeyActions();
    UIState st;

    static KeyList columns[] = {NAME, ID, THREADS, PARENTID, PRIORITY, DWSIZE};
    const int colCount = (int)(sizeof(columns) / sizeof(columns[0]));

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE) {
        GetLastError();
        return;
    }

    while (st.running) {
        system("cls");
        DisplayProcs(columns[st.selectedIndex], st.asc);

        DWORD numEvents = 0;
        if (!GetNumberOfConsoleInputEvents(hIn, &numEvents)) {
            GetLastError();
            break;
        }

        while (numEvents > 0) {
            INPUT_RECORD rec;
            DWORD readCount = 0;

            if (!ReadConsoleInputW(hIn, &rec, 1, &readCount)) {
                st.running = false;
                break;
            }

            if (readCount == 1 && rec.EventType == KEY_EVENT) {
                const KEY_EVENT_RECORD& kev = rec.Event.KeyEvent;
                HandleKeyEvent(kev, st, colCount);
            }

            if (!GetNumberOfConsoleInputEvents(hIn, &numEvents)) {
                break;
            }
        }

        Sleep(10000);
    }
}