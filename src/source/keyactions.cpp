// keyactions.cpp
#include "../header/keyactions.h"
#include <unordered_map>

typedef void (*ActionFn)(UIState& st, int columnCount);

static void ActLeft(UIState& st, int columnCount) {
    st.selectedIndex = (st.selectedIndex - 1 + columnCount) % columnCount;
}

static void ActRight(UIState& st, int columnCount) {
    st.selectedIndex = (st.selectedIndex + 1) % columnCount;
}

static void ActToggleAsc(UIState& st, int) {
    st.asc = !st.asc;
}

static void ActQuit(UIState& st, int) {
    st.running = false;
}

static std::unordered_map<WORD, ActionFn> g_actions;

void InitKeyActions() {
    g_actions[VK_LEFT] = &ActLeft;
    g_actions[VK_RIGHT] = &ActRight;
    g_actions[VK_UP] = &ActToggleAsc;
    g_actions[VK_DOWN] = &ActToggleAsc;
    g_actions['Q'] = &ActQuit;
}

void HandleKeyEvent(const KEY_EVENT_RECORD& key, UIState& st, int columnCount) {
    if (!key.bKeyDown) return;

    auto it = g_actions.find(key.wVirtualKeyCode);
    if (it != g_actions.end()) {
        it->second(st, columnCount);
    }
}
