// src/source/sortprocs.cpp
#include "../header/getprocs.h"
#include "../header/sortprocs.h"
#include <vector>
#include <algorithm>

using namespace std;

void SortProcs(vector<ProcInfo> &procs, KeyList k, bool asc) {
    auto cmp = [&](const ProcInfo& a, const ProcInfo& b) {
        switch (k) {
            case NAME:
                return asc ? a.szExeFile < b.szExeFile : a.szExeFile > b.szExeFile;
            case ID:
                return asc ? a.th32ProcessID < b.th32ProcessID : a.th32ProcessID > b.th32ProcessID;
            case THREADS:
                return asc ? a.cntThreads < b.cntThreads : a.cntThreads > b.cntThreads;
            case DWSIZE:
                return asc ? a.dwSize < b.dwSize : a.dwSize > b.dwSize;
            case PRIORITY:
                return asc ? a.pcPriClassBase < b.pcPriClassBase : a.pcPriClassBase > b.pcPriClassBase;
            case PARENTID:
                return asc ? a.th32ParentProcessID < b.th32ParentProcessID : a.th32ParentProcessID > b.th32ParentProcessID;
        }
        return false;
    };

    std::sort(procs.begin(), procs.end(), cmp);
    return;
}
