// src/source/sortprocs.cpp
#include "../header/getprocs.h"
#include "../header/sortprocs.h"
#include <vector>
#include <algorithm>

using namespace std;

vector<ProcInfo> SortProcs(vector<ProcInfo> &procs, keylist k, bool asc) {
    auto cmp = [&](const ProcInfo& a, const ProcInfo& b) {
        switch (k) {
            case 0:
                return asc ? a.szExeFile < b.szExeFile : a.szExeFile > b.szExeFile;
            case 1:
                return asc ? a.th32ProcessID < b.th32ProcessID : a.th32ProcessID > b.th32ProcessID;
            case 2:
                return asc ? a.cntThreads < b.cntThreads : a.cntThreads > b.cntThreads;
            case 3:
                return asc ? a.dwSize < b.dwSize : a.dwSize > b.dwSize;
            case 4:
                return asc ? a.pcPriClassBase < b.pcPriClassBase : a.pcPriClassBase > b.pcPriClassBase;
            case 5:
                return asc ? a.th32ParentProcessID < b.th32ParentProcessID : a.th32ParentProcessID > b.th32ParentProcessID;
        }
        return false;
    };

    std::sort(procs.begin(), procs.end(), cmp);
    return procs;
}
