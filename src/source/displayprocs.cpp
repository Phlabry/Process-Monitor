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

using namespace std;

void DisplayProcs(KeyList key, bool asc) {
    HANDLE snap = GetProcessSnap();
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    std::vector<ProcInfo> procs = GetProcVector(snap, pe32);
    int len = procs.size();

    SortProcs(procs, key, asc);

    const wchar_t* baseHeaders[5] = {
        L"PROCESS NAME",
        L"ID",
        L"THREADS",
        L"PARENT ID",
        L"PRIORITY"
    };

    const KeyList headerKeys[5] = {
        NAME,
        ID,
        THREADS,
        PARENTID,
        PRIORITY
    };

    std::wstring headers[5];
    for (int i = 0; i < 5; i++) {
        headers[i] = baseHeaders[i];

        if (headerKeys[i] == key) {
            headers[i] = L"> " + headers[i] + (asc ? L" <↑" : L" <↓");
        }
    }

    int namew = headers[0].size();
    int idw = headers[1].size();
    int threadsw = headers[2].size();
    int parentw = headers[3].size();
    int priow = headers[4].size();

    for (int i = 0; i < len; i++) {
        if (procs[i].szExeFile.size() > namew) namew = procs[i].szExeFile.size();

        int cur = 0;

        cur = std::to_wstring(procs[i].th32ProcessID).size();
        if (cur > idw) idw = cur;

        cur = std::to_wstring(procs[i].cntThreads).size();
        if (cur > threadsw) threadsw = cur;

        cur = std::to_wstring(procs[i].th32ParentProcessID).size();
        if (cur > parentw) parentw = cur;

        cur = std::to_wstring(procs[i].pcPriClassBase).size();
        if (cur > priow) priow = cur;
    }

    const int pad = 2;
    namew += pad; idw += pad; threadsw += pad; parentw += pad; priow += pad;

    std::wostringstream out;

    out << L"Total Process Count: " << len << L"\n";

    out << std::left
        << std::setw(namew) << headers[0]
        << std::setw(idw) << headers[1]
        << std::setw(threadsw) << headers[2]
        << std::setw(parentw) << headers[3]
        << std::setw(priow) << headers[4]
        << L"\n";

    for (int i = 0; i < len; i++) {
        out << std::left
            << std::setw(namew) << procs[i].szExeFile
            << std::right
            << std::setw(idw) << procs[i].th32ProcessID
            << std::setw(threadsw) << procs[i].cntThreads
            << std::setw(parentw) << procs[i].th32ParentProcessID
            << std::setw(priow) << procs[i].pcPriClassBase
            << L"\n";
    }

    std::wcout << out.str();

    CloseHandle(snap);
}

static void RefreshDisplay(KeyList key, bool asc) {
    std::wostringstream capture;
    std::wstreambuf* oldBuf = std::wcout.rdbuf(capture.rdbuf());
    DisplayProcs(key, asc);
    std::wcout.rdbuf(oldBuf);

    std::wstring frame = capture.str();

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

    const COORD savedCursor = csbi.dwCursorPosition;
    const SMALL_RECT savedWindow = csbi.srWindow;

    DWORD cellCount = (DWORD)csbi.dwSize.X * (DWORD)csbi.dwSize.Y;
    DWORD written = 0;
    COORD origin = { 0, 0 };
    FillConsoleOutputCharacterW(hOut, L' ', cellCount, origin, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, origin, &written);

    SHORT row = savedWindow.Top;
    SHORT col = 0;

    const SHORT bufW = csbi.dwSize.X;
    const SHORT bufH = csbi.dwSize.Y;

    size_t i = 0;
    while (i < frame.size() && row < bufH) {
        size_t lineEnd = frame.find(L'\n', i);
        if (lineEnd == std::wstring::npos) lineEnd = frame.size();

        std::wstring line = frame.substr(i, lineEnd - i);
        if ((SHORT)line.size() > bufW) line.resize(bufW);

        DWORD charsWritten = 0;
        COORD pos = { col, row };
        WriteConsoleOutputCharacterW(hOut, line.c_str(), (DWORD)line.size(), pos, &charsWritten);

        if ((SHORT)line.size() < bufW) {
            DWORD restWritten = 0;
            COORD restPos = { (SHORT)line.size(), row };
            FillConsoleOutputCharacterW(hOut, L' ', (DWORD)(bufW - (SHORT)line.size()), restPos, &restWritten);
            FillConsoleOutputAttribute(hOut, csbi.wAttributes, (DWORD)(bufW - (SHORT)line.size()), restPos, &restWritten);
        }

        row++;
        i = (lineEnd < frame.size()) ? (lineEnd + 1) : lineEnd;
    }

    SetConsoleWindowInfo(hOut, TRUE, &savedWindow);
    SetConsoleCursorPosition(hOut, savedCursor);
}

void CycleDisplay() {
    InitKeyActions();
    UIState st;
    
    static KeyList columns[] = {NAME, ID, THREADS, PARENTID, PRIORITY, DWSIZE};
    const int colCount = (sizeof(columns) / sizeof(columns[0]));
    RefreshDisplay(columns[st.selectedIndex], st.asc);
    
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE) {
        GetLastError();
        return;
    }
    
    while (st.running) {
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
                RefreshDisplay(columns[st.selectedIndex], st.asc);
            }
            
            if (!GetNumberOfConsoleInputEvents(hIn, &numEvents)) {
                break;
            }
        }
        
        Sleep(1000);
    }
}