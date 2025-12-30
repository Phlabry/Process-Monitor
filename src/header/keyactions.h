// keyactions.h
#pragma once
#include <windows.h>   
#include "sortprocs.h" 

struct UIState {
    int selectedIndex = 0;
    bool asc = false;
    bool running = true;
};

void InitKeyActions();
void HandleKeyEvent(const KEY_EVENT_RECORD& key, UIState& st, int columnCount);
