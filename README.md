# Process-Monitor

A minimal `top`-style process viewer for Windows, written against the Win32 API directly — no
dependencies beyond the Windows SDK.

Enumerates every running process via a ToolHelp32 snapshot and renders a live, sortable table
in the console. Columns resize themselves to the widest value present, so output stays aligned
regardless of process-name length.

```
PROCESS NAME        ID      > THREADS <↑   PARENT ID   PRIORITY
explorer.exe        4821    142             1204        8
chrome.exe          9130    58              4821        8
...
```

## Controls

| Key | Action |
|---|---|
| `←` / `→` | Move the sort column |
| `↑` / `↓` | Toggle ascending / descending |
| `Q` | Quit |

The active sort column is marked inline in the header (`> THREADS <↑`).

## How it works

| File | Role |
|---|---|
| [`getprocs.cpp`](src/source/getprocs.cpp) | `CreateToolhelp32Snapshot` + `Process32FirstW`/`Process32NextW` walk, collected into `vector<ProcInfo>` |
| [`sortprocs.cpp`](src/source/sortprocs.cpp) | Single comparator lambda switching on a `KeyList` enum, so one function covers all six columns in both directions |
| [`keyactions.cpp`](src/source/keyactions.cpp) | Key handling as an `unordered_map<WORD, ActionFn>` dispatch table rather than a `switch` — registered once in `InitKeyActions` |
| [`displayprocs.cpp`](src/source/displayprocs.cpp) | Redraw loop, dynamic column widths, header decoration |

Process names are held as `std::wstring` throughout (`PROCESSENTRY32W`), so processes with
non-ASCII names display correctly instead of being mangled through the ANSI variants.

## Build

```bash
g++ -std=c++20 src/main.cpp src/source/*.cpp -o src/main.exe
```

Or use the bundled VS Code task (`Ctrl+Shift+B`). Windows-only — depends on `windows.h`,
`tlhelp32.h`, and the console API.
