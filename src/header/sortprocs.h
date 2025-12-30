// src/header/sortprocs.h
#pragma once
#include "getprocs.h"
#include <vector>

enum KeyList{
    NAME,
    ID,
    THREADS,
    DWSIZE,
    PRIORITY,
    PARENTID,
};

void SortProcs(std::vector<ProcInfo> &procs, KeyList k, bool asc);
