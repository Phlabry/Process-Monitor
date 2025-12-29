// src/header/sortprocs.h
#pragma once
#include "getprocs.h"
#include <vector>

enum keylist{
    name,
    id,
    threads,
    dwsize,
    prio,
    parentpid,
};

void SortProcs(std::vector<ProcInfo> &procs, keylist k, bool asc);
