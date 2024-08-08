#pragma once
#include "Window.h"
#include "Control.h"
#include "Font.h"

class Listbox {
public:

    Listbox(Window& window, int x, int y, int w, int h)
    : id(window.getNextId())
    {
        hwnd = CreateWindowExA(WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_HASSTRINGS | WS_VSCROLL,
            x, y, w, h, window.hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Listbox creation failed");
        }
    }

    void addItem(const char* item) {
        SendMessageA(hwnd, LB_ADDSTRING, 0, (LPARAM)item);
    }

    void setCommand(Window& window, std::function<void(int e)>&& action) {
        window.setMenuCommand(id, std::move(action));
    }

    long getSelectedIndex() {
        long selIndex = SendMessageA(hwnd, LB_GETCURSEL, 0, 0);
        return selIndex;
    }

    void setSelectedIndex(int index) {
        SendMessageA(hwnd, LB_SETCURSEL, index, 0);
    }

    std::string getItem(int index) {
        std::string str;
        long length = SendMessageA(hwnd, LB_GETTEXTLEN, index, NULL);
        if (length == LB_ERR) throw std::exception("Out of bounds");
        str.resize(length);
        SendMessageA(hwnd, LB_GETTEXT, index, (LPARAM)str.data());
        return str;
    }

    void setFont(Font& font) {
        SendMessage(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
    }

private:
    size_t id;
    HWND hwnd = 0;
};