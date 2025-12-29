// src/source/sortprocs.cpp
#include "../header/getprocs.h"
#include "../header/sortprocs.h"
#include <vector>
#include <algorithm>

using namespace std;

void SortProcs(vector<ProcInfo> &procs, keylist k, bool asc) {
    auto cmp = [&](const ProcInfo& a, const ProcInfo& b) {
        switch (k) {
            case name:
                return asc ? a.szExeFile < b.szExeFile : a.szExeFile > b.szExeFile;
            case id:
                return asc ? a.th32ProcessID < b.th32ProcessID : a.th32ProcessID > b.th32ProcessID;
            case threads:
                return asc ? a.cntThreads < b.cntThreads : a.cntThreads > b.cntThreads;
            case dwsize:
                return asc ? a.dwSize < b.dwSize : a.dwSize > b.dwSize;
            case prio:
                return asc ? a.pcPriClassBase < b.pcPriClassBase : a.pcPriClassBase > b.pcPriClassBase;
            case parentpid:
                return asc ? a.th32ParentProcessID < b.th32ParentProcessID : a.th32ParentProcessID > b.th32ParentProcessID;
        }
        return false;
    };

    std::sort(procs.begin(), procs.end(), cmp);
    return;
}
