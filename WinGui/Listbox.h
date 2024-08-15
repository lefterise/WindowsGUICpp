#pragma once
#include "Window.h"
#include "Control.h"
#include "Font.h"

class Listbox {
public:

    Listbox(Window& window, int x, int y, int w, int h)
    {
        hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_HASSTRINGS | WS_VSCROLL,
            x, y, w, h, window.hwnd, (HMENU)window.getNextId(), (HINSTANCE)GetWindowLongPtrW(window.hwnd, GWLP_HINSTANCE), NULL);

        if (hwnd == NULL) {
            throw std::exception("Listbox creation failed");
        }
    }

    void addItem(const wchar_t* item) {
        SendMessageW(hwnd, LB_ADDSTRING, 0, (LPARAM)item);
    }

    void setCommand(Window& window, std::function<void(int e)>&& action) {
        int id = GetDlgCtrlID(hwnd);
        window.setMenuCommand(id, std::move(action));
    }

    long getSelectedIndex() {
        long selIndex = SendMessageW(hwnd, LB_GETCURSEL, 0, 0);
        return selIndex;
    }

    void setSelectedIndex(int index) {
        SendMessageW(hwnd, LB_SETCURSEL, index, 0);
    }

    std::wstring getItem(int index) {
        std::wstring str;
        long length = SendMessageW(hwnd, LB_GETTEXTLEN, index, NULL);
        if (length == LB_ERR) throw std::exception("Out of bounds");
        str.resize(length);
        SendMessageW(hwnd, LB_GETTEXT, index, (LPARAM)str.data());
        return str;
    }

    void setFont(Font& font) {
        SendMessageW(hwnd, WM_SETFONT, (WPARAM)font.hFont, TRUE);
    }

    operator HWND() const {
        return hwnd;
    }

private:
    HWND hwnd = 0;
};