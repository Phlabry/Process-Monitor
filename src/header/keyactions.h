#pragma once
#include "sortprocs.h"
#include <windows.h>

struct UIState {
    int selectedIndex = 0;
    bool asc = false;
    bool running = true;
};

void InitKeyActions();
void HandleKeyEvent(const KEY_EVENT_RECORD& key, UIState& st, int columnCount);
