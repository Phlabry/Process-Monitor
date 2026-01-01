#pragma once
#include "../header/sortprocs.h"
#include "../header/getprocs.h"

void DisplayProcs(KeyList key = KeyList::NAME, bool asc = false);
static void ForceViewportTop(HANDLE hOut);
static void RefreshDisplay(KeyList key, bool asc);
void CycleDisplay();