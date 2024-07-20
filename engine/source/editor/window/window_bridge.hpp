#include <windows.h>

void createWindowsMenu(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreatePopupMenu();

    AppendMenu(hSubMenu, MF_STRING, 1, "Option 1");
    AppendMenu(hSubMenu, MF_STRING, 2, "Option 2");
    AppendMenu(hSubMenu, MF_STRING, 3, "Option 3");

    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "Menu");

    SetMenu(hwnd, hMenu);
}