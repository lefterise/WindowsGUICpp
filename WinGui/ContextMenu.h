#pragma once
#include "Window.h"

class ContextMenu {
public:
    ContextMenu()
    {
        hMenu = CreatePopupMenu();
    }

    ContextMenu(const ContextMenu&) = delete; //Prevent copy because the first destructor of any copy that is called it will destroy the menu

    ContextMenu& addItem(Window& window, std::wstring caption, std::function<void(int e)>&& action) {
        auto id = window.getNextId();
        AppendMenuW(hMenu, MF_STRING, id, caption.c_str());
        window.setMenuCommand(id, std::move(action));
        return *this;
    }

    void track(Window& window, int x, int y) {
        TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN, x, y, 0, window.hwnd, NULL);        
    }

    operator HMENU() const {
        return hMenu;
    }


    ~ContextMenu() {
        DestroyMenu(hMenu);
    }
    HMENU hMenu;
};